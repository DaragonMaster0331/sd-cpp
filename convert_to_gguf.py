"""Convert the downloaded FLUX.2 [klein] 4B safetensors to GGUF with sd-cli's convert mode.

    diffusion model  bf16 -> Q8_0   (near-lossless, fast on AVX2)
    text encoder     bf16 -> Q8_0   (Qwen3-4B)
    VAE              f32  -> F16    (quantizing the VAE visibly hurts images, so it stays 16-bit)

Runs fully offline.

    python convert_to_gguf.py
    python convert_to_gguf.py --diffusion-type q4_K --force
"""

import argparse
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent
SD_CLI = ROOT / "dist" / "bin" / "sd-cli.exe"
SOURCE = ROOT / "models" / "download" / "flux2-klein-4B" / "split_files"
MODELS = ROOT / "models"


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--diffusion-type", default="q8_0")
    parser.add_argument("--text-encoder-type", default="q8_0")
    parser.add_argument("--vae-type", default="f16")
    parser.add_argument("--force", action="store_true", help="overwrite existing .gguf files")
    args = parser.parse_args()

    if not SD_CLI.exists():
        sys.exit(f"sd-cli.exe not found at {SD_CLI} (run: python build.py)")

    jobs = [
        (SOURCE / "diffusion_models" / "flux-2-klein-4b.safetensors",
         MODELS / "diffusion_models" / f"flux-2-klein-4b-{args.diffusion_type.upper()}.gguf", args.diffusion_type),
        (SOURCE / "text_encoders" / "qwen_3_4b.safetensors",
         MODELS / "text_encoders" / f"qwen_3_4b-{args.text_encoder_type.upper()}.gguf", args.text_encoder_type),
        (SOURCE / "vae" / "flux2-vae.safetensors",
         MODELS / "vae" / f"flux2-vae-{args.vae_type.upper()}.gguf", args.vae_type),
    ]

    for source, target, weight_type in jobs:
        if not source.exists():
            sys.exit(f"missing source model: {source}")
        if target.exists() and not args.force:
            print(f"exists, skipping (use --force to redo): {target.name}")
            continue
        target.parent.mkdir(parents=True, exist_ok=True)
        print(f"Converting {source.name} -> {target.name} [{weight_type}]", flush=True)
        started = time.time()
        result = subprocess.run([str(SD_CLI), "-M", "convert", "-m", str(source), "-o", str(target),
                                 "--type", weight_type])
        if result.returncode != 0:
            target.unlink(missing_ok=True)  # the converter preallocates, so a failed run leaves a full-size file
            sys.exit(f"conversion failed for {source.name} (exit code {result.returncode})")
        size_gb = target.stat().st_size / 1024 ** 3
        print(f"  done in {time.time() - started:.0f}s, {size_gb:.2f} GB")


if __name__ == "__main__":
    main()
