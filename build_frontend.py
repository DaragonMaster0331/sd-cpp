"""Build the dark-theme / multi-language WebUI and regenerate the header sd-server embeds
(stable-diffusion.cpp/examples/server/frontend/dist/gen_index_html.h).

The first run needs network access once for "pnpm install" (fills node_modules).
After that it works fully offline; pass --offline to forbid network use.

    python build_frontend.py
    python build_frontend.py --offline
    python build.py                      # then recompile to embed the new header
"""

import argparse
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
FRONTEND = ROOT / "stable-diffusion.cpp" / "examples" / "server" / "frontend"
PNPM = ROOT / "toolchain" / "pnpm" / "node_modules" / ".bin" / "pnpm.cmd"


def pnpm(*pnpm_args):
    command = [str(PNPM), "--ignore-workspace", *pnpm_args]
    print(">> pnpm " + " ".join(pnpm_args), flush=True)
    if subprocess.run(command, cwd=FRONTEND).returncode != 0:
        sys.exit(f"pnpm {' '.join(pnpm_args)} failed")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--offline", action="store_true", help="install only from the local pnpm store")
    parser.add_argument("--skip-type-check", action="store_true")
    args = parser.parse_args()

    if not PNPM.exists():
        sys.exit(f"pnpm not found at {PNPM}\n"
                 f'Install it once: npm install --prefix "{ROOT / "toolchain" / "pnpm"}" pnpm@10.15.1')

    if not (FRONTEND / "node_modules").exists():
        pnpm("install", "--frozen-lockfile", *(["--offline"] if args.offline else []))
    if not args.skip_type_check:
        pnpm("run", "type-check")
    pnpm("run", "build")
    pnpm("run", "build:header")

    for name in ("index.html", "gen_index_html.h"):
        path = FRONTEND / "dist" / name
        print(f"  {name:<18} {path.stat().st_size:>12,} bytes")


if __name__ == "__main__":
    main()
