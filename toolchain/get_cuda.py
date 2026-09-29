"""Download a portable CUDA 12.x toolkit (no installer, no admin) for building the GPU engine.

Uses NVIDIA's per-component redistributable archives
(https://developer.download.nvidia.com/compute/cuda/redist/) and merges them into
toolchain/cuda/<version>/ with the usual bin / include / lib/x64 / nvvm layout.
CUDA 12.x is the last series that supports the Visual Studio 2019 (MSVC 19.29) host compiler.

    python toolchain/get_cuda.py --accept-eula
    python toolchain/get_cuda.py --accept-eula --version 12.9.1

Using the toolkit means accepting the NVIDIA CUDA EULA:
https://docs.nvidia.com/cuda/eula/index.html
"""

import argparse
import hashlib
import json
import shutil
import sys
import urllib.request
import zipfile
from pathlib import Path

REDIST = "https://developer.download.nvidia.com/compute/cuda/redist/"
PLATFORM = "windows-x86_64"
COMPONENTS = [
    "cuda_nvcc",                  # nvcc, cicc, ptxas, nvvm, crt headers
    "cuda_cudart",                # runtime headers/libs + cudart64_12.dll
    "cuda_cccl",                  # thrust / cub / libcu++ headers
    "cuda_profiler_api",
    "libcublas",                  # cublas64_12.dll, cublasLt64_12.dll
    "visual_studio_integration",  # MSBuild customizations for the .sln build
]
TOOLCHAIN_DIR = Path(__file__).resolve().parent
EULA = "https://docs.nvidia.com/cuda/eula/index.html"


def fetch(url):
    with urllib.request.urlopen(url, timeout=120) as response:
        return response.read()


def download(url, target, sha256):
    if target.exists() and hashlib.sha256(target.read_bytes()).hexdigest() == sha256:
        print(f"  cached  {target.name}")
        return
    print(f"  fetch   {target.name}", flush=True)
    part = target.with_suffix(target.suffix + ".part")
    digest = hashlib.sha256()
    with urllib.request.urlopen(url, timeout=120) as response, open(part, "wb") as out:
        while True:
            chunk = response.read(1 << 20)
            if not chunk:
                break
            digest.update(chunk)
            out.write(chunk)
    if digest.hexdigest() != sha256:
        part.unlink()
        sys.exit(f"sha256 mismatch for {target.name}")
    part.replace(target)


def extract(archive, dest):
    """Extract a redist zip, dropping its top-level '<component>-windows-x86_64-<ver>-archive/' folder."""
    with zipfile.ZipFile(archive) as zf:
        for info in zf.infolist():
            parts = Path(info.filename).parts
            if len(parts) < 2 or info.is_dir():
                continue
            relative = Path(*parts[1:])
            if relative.parts[0] in ("LICENSE",) or relative.name == "LICENSE":
                relative = Path("licenses") / f"{parts[0].split('-windows')[0]}_LICENSE"
            out = dest / relative
            out.parent.mkdir(parents=True, exist_ok=True)
            with zf.open(info) as src, open(out, "wb") as dst:
                shutil.copyfileobj(src, dst)


def newest_12x():
    index = fetch(REDIST).decode("utf-8", "replace")
    import re
    versions = sorted({m for m in re.findall(r"redistrib_(12\.\d+\.\d+)\.json", index)},
                      key=lambda v: tuple(int(x) for x in v.split(".")))
    if not versions:
        sys.exit("no CUDA 12.x manifest found")
    return versions[-1]


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--version", help="CUDA 12.x release, e.g. 12.9.1 (default: newest 12.x)")
    parser.add_argument("--accept-eula", action="store_true", help=f"accept the NVIDIA CUDA EULA ({EULA})")
    args = parser.parse_args()
    if not args.accept_eula:
        sys.exit(f"Read the NVIDIA CUDA EULA ({EULA}) and re-run with --accept-eula")

    version = args.version or newest_12x()
    manifest = json.loads(fetch(f"{REDIST}redistrib_{version}.json"))
    dest = TOOLCHAIN_DIR / "cuda" / version
    downloads = TOOLCHAIN_DIR / "cuda" / "downloads"
    downloads.mkdir(parents=True, exist_ok=True)
    print(f"CUDA {version} -> {dest}")

    installed = {}
    for name in COMPONENTS:
        entry = manifest[name][PLATFORM]
        archive = downloads / Path(entry["relative_path"]).name
        download(REDIST + entry["relative_path"], archive, entry["sha256"])
        extract(archive, dest)
        installed[name] = manifest[name]["version"]

    nvcc = dest / "bin" / "nvcc.exe"
    if not nvcc.exists():
        sys.exit(f"nvcc.exe missing after extraction: {nvcc}")
    (dest / "cuda_info.json").write_text(json.dumps(
        {"version": version, "components": installed, "eula": EULA}, indent=2), encoding="utf-8")
    print(f"Done: {nvcc}")


if __name__ == "__main__":
    main()
