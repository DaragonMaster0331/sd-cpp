"""Start the local, fully offline FLUX.2 [klein] 4B image server and open the WebUI.

Nothing here touches the network: models are local GGUF files and the WebUI is embedded in sd-server.exe.

    python start_flux2_klein.py
    python start_flux2_klein.py --port 8080 --width 1024 --height 1024 --no-browser
"""

import argparse
import os
import subprocess
import sys
import time
import urllib.request
import webbrowser
from pathlib import Path

ROOT = Path(__file__).resolve().parent
BIN = ROOT / "dist" / "bin"
MODELS = ROOT / "models"
DIFFUSION = MODELS / "diffusion_models" / "flux-2-klein-4b-Q8_0.gguf"
LLM = MODELS / "text_encoders" / "qwen_3_4b-Q8_0.gguf"
VAE = MODELS / "vae" / "flux2-vae-F16.gguf"


def physical_cores():
    try:
        output = subprocess.run(
            ["powershell", "-NoProfile", "-Command",
             "(Get-CimInstance Win32_Processor | Measure-Object -Property NumberOfCores -Sum).Sum"],
            capture_output=True, text=True, timeout=30).stdout.strip()
        return int(output)
    except (OSError, ValueError, subprocess.SubprocessError):
        return max(1, (os.cpu_count() or 2) // 2)


def wait_until_up(url, server, timeout_s=300):
    deadline = time.time() + timeout_s
    while time.time() < deadline and server.poll() is None:
        try:
            with urllib.request.urlopen(url, timeout=2):
                return True
        except OSError:
            time.sleep(0.5)
    return False


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--listen-ip", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=1234)
    parser.add_argument("--threads", type=int, default=0, help="0 = number of physical cores")
    # CPU-only: 512x512 keeps a 4-step image to minutes; klein supports up to ~1024x1024.
    parser.add_argument("--width", type=int, default=512)
    parser.add_argument("--height", type=int, default=512)
    parser.add_argument("--steps", type=int, default=4, help="klein 4B is step-distilled: 4 steps, CFG 1.0")
    parser.add_argument("--no-browser", action="store_true")
    parser.add_argument("--auth-file", help="enable user sign-in; accounts are stored in this JSON file (e.g. users.json)")
    args = parser.parse_args()

    server_exe = BIN / "sd-server.exe"
    missing = [str(p) for p in (server_exe, DIFFUSION, LLM, VAE) if not p.exists()]
    if missing:
        sys.exit("Missing files:\n  " + "\n  ".join(missing))

    threads = args.threads if args.threads > 0 else physical_cores()
    command = [
        str(server_exe),
        "--diffusion-model", str(DIFFUSION),
        "--llm", str(LLM),
        "--vae", str(VAE),
        "--cfg-scale", "1.0",
        "--steps", str(args.steps),
        "-W", str(args.width),
        "-H", str(args.height),
        "--diffusion-fa",
        "-t", str(threads),
        "--listen-ip", args.listen_ip,
        "--listen-port", str(args.port),
    ]
    if args.auth_file:
        command += ["--auth-file", str(Path(args.auth_file).resolve())]

    url = f"http://{args.listen_ip}:{args.port}/"
    print(f"Starting sd-server (FLUX.2 klein 4B, {threads} threads) at {url}")
    print("Press Ctrl+C to stop.", flush=True)
    server = subprocess.Popen(command, cwd=BIN)
    try:
        if wait_until_up(url, server) and not args.no_browser:
            webbrowser.open(url)
        server.wait()
    except KeyboardInterrupt:
        print("Stopping sd-server...")
        server.terminate()
        server.wait(timeout=30)
    sys.exit(server.returncode or 0)


if __name__ == "__main__":
    main()
