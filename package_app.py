"""Assemble a self-contained, portable SD-Studio folder for offline PCs (nothing to install).

    python package_app.py                          -> dist/SD-Studio-portable
    python package_app.py --dest E:\\SD-Studio      e.g. a USB drive
    python package_app.py --no-cuda                CPU engine only (saves ~800 MB)

Layout:
    SD-Studio.exe, WebView2Loader.dll, MSVC runtime DLLs
    engine/cpu/     sd-server.exe, sd-cli.exe + DLLs          (from dist/bin)
    engine/cuda/    same, NVIDIA CUDA build + CUDA DLLs        (from dist/bin-cuda, if built)
    models/         the three GGUF files (hard-linked when on the same drive, else copied)

The target PC needs 64-bit Windows 10/11 with the WebView2 runtime (built into Windows 11 and updated
Windows 10; otherwise put the WebView2 "Fixed Version" runtime in WebView2Runtime/), a CPU with AVX2,
and for the GPU engine an NVIDIA GPU (RTX 20 or newer) with driver 575 or newer.
"""

import argparse
import os
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
DIST = ROOT / "dist"
MODEL_FILES = [
    Path("diffusion_models") / "flux-2-klein-4b-Q8_0.gguf",
    Path("text_encoders") / "qwen_3_4b-Q8_0.gguf",
    Path("vae") / "flux2-vae-F16.gguf",
]

README = """SD-Studio - offline FLUX.2 [klein] 4B image generation (stable-diffusion.cpp)

Start:      double-click SD-Studio.exe
Sign-in:    first start creates the administrator; the administrator adds users on the Account tab.
            Accounts: %LOCALAPPDATA%\\SD-Studio\\users.json (password hashes only). Turn off: [auth] enabled=0
            in settings.ini. Lost the only admin password: close the app and delete users.json.
Engine:     "Engine" picker in the header - Auto (GPU if available) / GPU (NVIDIA CUDA) / CPU
Settings:   %LOCALAPPDATA%\\SD-Studio\\settings.ini   (engine mode, default size, extra engine arguments)
Engine log: %LOCALAPPDATA%\\SD-Studio\\logs\\engine.log

Requirements: 64-bit Windows 10/11, CPU with AVX2, 16 GB RAM. GPU engine: NVIDIA RTX 20 or newer + driver 575 or newer
(12 GB VRAM holds all models; for 8 GB set  cuda_args=--backend all=cuda0,te=cpu  under [engine]).
WebView2 runtime: built into Windows 11 and updated Windows 10. For PCs without it, extract the
WebView2 "Fixed Version" runtime (x64) into the WebView2Runtime folder next to SD-Studio.exe.

Command line (no UI):  engine\\cpu\\sd-cli.exe or engine\\cuda\\sd-cli.exe
  --diffusion-model models\\diffusion_models\\flux-2-klein-4b-Q8_0.gguf --llm models\\text_encoders\\qwen_3_4b-Q8_0.gguf
  --vae models\\vae\\flux2-vae-F16.gguf --cfg-scale 1.0 --steps 4 -W 1024 -H 1024 --diffusion-fa -p "a cat" -o out.png
"""


def copy_tree_files(source, target, skip=()):
    target.mkdir(parents=True, exist_ok=True)
    for item in source.iterdir():
        if item.is_file() and item.name not in skip:
            shutil.copy2(item, target / item.name)


def link_or_copy(source, target):
    target.parent.mkdir(parents=True, exist_ok=True)
    if target.exists():
        if target.stat().st_size == source.stat().st_size:
            return "kept"
        target.unlink()
    try:
        os.link(source, target)
        return "hard-linked"
    except OSError:
        shutil.copy2(source, target)
        return "copied"


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--dest", type=Path, default=DIST / "SD-Studio-portable")
    parser.add_argument("--no-cuda", action="store_true", help="leave out the NVIDIA CUDA engine")
    parser.add_argument("--no-models", action="store_true", help="leave out the models (copy them later)")
    args = parser.parse_args()

    app, cpu, cuda = DIST / "SD-Studio", DIST / "bin", DIST / "bin-cuda"
    if not (app / "SD-Studio.exe").exists() or not (cpu / "sd-server.exe").exists():
        sys.exit("Build first: python build.py   (and python build.py --cuda for the GPU engine)")
    dest = args.dest.resolve()
    print(f"Packaging SD-Studio -> {dest}")

    copy_tree_files(app, dest)
    copy_tree_files(cpu, dest / "engine" / "cpu")
    print("  app + CPU engine")
    if not args.no_cuda:
        if (cuda / "sd-server.exe").exists():
            copy_tree_files(cuda, dest / "engine" / "cuda")
            print("  CUDA engine")
        else:
            print("  CUDA engine not built (python build.py --cuda) - package is CPU-only")
    elif (dest / "engine" / "cuda").exists():
        shutil.rmtree(dest / "engine" / "cuda")

    if not args.no_models:
        for relative in MODEL_FILES:
            source = ROOT / "models" / relative
            if not source.exists():
                sys.exit(f"missing model: {source} (run: python convert_to_gguf.py)")
            how = link_or_copy(source, dest / "models" / relative)
            print(f"  models/{relative.as_posix()} ({how})")
    (dest / "README.txt").write_text(README, encoding="utf-8")

    total = sum(f.stat().st_size for f in dest.rglob("*") if f.is_file())
    print(f"Done: {dest}  ({total / 1024 ** 3:.2f} GB)")


if __name__ == "__main__":
    main()
