"""Build stable-diffusion.cpp (CPU, AVX2, OpenMP) with the portable Visual Studio 2019 toolset.

Fully offline once toolchain/ exists. The server's WebUI is embedded when
stable-diffusion.cpp/examples/server/frontend/dist/gen_index_html.h is present
(create it with build_frontend.py, or pass --frontend here).

    python build.py                 Ninja build -> stable-diffusion.cpp/build, staged in dist/bin
    python build.py --sln           Visual Studio 2019 solution -> stable-diffusion.cpp/build-vs2019/stable-diffusion.sln,
                                    built with MSBuild (Release|x64), staged in dist/bin
    python build.py --sln --no-build   only generate the .sln (e.g. to open it in the VS 2019 IDE)
    python build.py --frontend      build the WebUI first, then compile
    python build.py --clean -j 16   fresh build with 16 jobs

    python build.py --cuda          NVIDIA GPU engine (CUDA 12.x from toolchain/cuda, see toolchain/get_cuda.py)
                                    -> stable-diffusion.cpp/build-cuda, staged in dist/bin-cuda
    python build.py --cuda --sln --no-build   VS 2019 solution with CUDA -> build-vs2019-cuda/stable-diffusion.sln
                                    (building it works too, but MSBuild compiles .cu files one at a time)
"""

import argparse
import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "toolchain"))
import vs2019_env  # noqa: E402

SOURCE_DIR = ROOT / "stable-diffusion.cpp"
FRONTEND_HEADER = SOURCE_DIR / "examples" / "server" / "frontend" / "dist" / "gen_index_html.h"
DIST_BIN = ROOT / "dist" / "bin"
DIST_BIN_CUDA = ROOT / "dist" / "bin-cuda"
DIST_APP = ROOT / "dist" / "SD-Studio"
WEBVIEW2_SDK = ROOT / "toolchain" / "webview2"  # from toolchain/get_webview2_sdk.py
DESKTOP_FILES = ("SD-Studio.exe", "WebView2Loader.dll")
CUDA_ROOT = ROOT / "toolchain" / "cuda"
# Native code for RTX 30 (86), RTX 40 (89) and RTX 50 (120a); PTX for Turing (RTX 20) and newer GPUs.
DEFAULT_CUDA_ARCHS = "75-virtual;86-real;89-real;120a-real"

# Single static CPU backend, portable AVX2 (Haswell / Zen2+), no runtime dispatch.
COMMON_CMAKE_ARGS = [
    "-DGGML_NATIVE=OFF",
    "-DGGML_BACKEND_DL=OFF",
    "-DGGML_CPU_ALL_VARIANTS=OFF",
    "-DGGML_SSE42=ON",
    "-DGGML_AVX=ON",
    "-DGGML_AVX2=ON",
    "-DGGML_BMI2=ON",
    "-DGGML_FMA=ON",
    "-DGGML_F16C=ON",
    "-DGGML_AVX512=OFF",
    "-DGGML_OPENMP=ON",
    "-DGGML_CCACHE=OFF",
    "-DSD_BUILD_SHARED_LIBS=OFF",
    "-DSD_WEBP=ON",
    "-DSD_WEBM=ON",
    # PNPM_EXECUTABLE=OFF: CMake never searches for pnpm and embeds the pre-built header if it exists.
    "-DSD_SERVER_BUILD_FRONTEND=ON",
    "-DPNPM_EXECUTABLE=OFF",
]


def cmake_path(path):
    # CMake cache values must not contain backslashes (they are parsed as escapes).
    return str(path).replace("\\", "/")


def run(cmd, env, cwd=None):
    shown = " ".join(f'"{c}"' if re.search(r"\s", str(c)) else str(c) for c in cmd)
    print(f">> {shown}", flush=True)
    result = subprocess.run([str(c) for c in cmd], env=env, cwd=cwd)
    if result.returncode != 0:
        sys.exit(f"command failed with exit code {result.returncode}: {cmd[0]}")


def find_cuda():
    """Newest portable CUDA toolkit under toolchain/cuda (installed by toolchain/get_cuda.py)."""
    candidates = [d for d in CUDA_ROOT.glob("*") if (d / "bin" / "nvcc.exe").exists()] if CUDA_ROOT.exists() else []
    if not candidates:
        sys.exit(f"No CUDA toolkit in {CUDA_ROOT}. Run once while online: python toolchain/get_cuda.py --accept-eula")
    return max(candidates, key=lambda d: tuple(int(x) for x in d.name.split(".")))


def cuda_cmake_args(cuda, archs):
    return [
        "-DSD_CUDA=ON",
        f"-DCUDAToolkit_ROOT={cmake_path(cuda)}",
        f"-DCMAKE_CUDA_ARCHITECTURES={archs}",
        # ggml-cuda links the driver API (nvcuda.dll, installed with the NVIDIA driver). Delay-loading it
        # lets the GPU engine start on a PC without the driver; it then reports no CUDA device and runs on CPU.
        "-DCMAKE_EXE_LINKER_FLAGS=/DELAYLOAD:nvcuda.dll delayimp.lib",
    ]


def desktop_cmake_args(cuda):
    """SD-Studio (examples/desktop) is built with the CPU configuration only; it runs either engine."""
    if cuda or not (WEBVIEW2_SDK / "include" / "WebView2.h").exists():
        return []
    return [f"-DSD_WEBVIEW2_DIR={cmake_path(WEBVIEW2_SDK)}"]


def install_cuda_msbuild_integration(cuda):
    """Make the CUDA MSBuild customizations visible to the portable VS 2019 instance (for the .sln)."""
    source = cuda / "visual_studio_integration" / "MSBuildExtensions"
    targets = [
        vs2019_env.MSVC_ROOT / "MSBuild" / "Microsoft" / "VC" / "v160" / "BuildCustomizations",
        # CMake expects this layout when a standalone toolkit is passed with -T cuda=<path>.
        cuda / "extras" / "visual_studio_integration" / "MSBuildExtensions",
    ]
    for target in targets:
        target.mkdir(parents=True, exist_ok=True)
        for item in source.iterdir():
            shutil.copy2(item, target / item.name)


def build_ninja(args, env, layout, cuda=None):
    build_dir = SOURCE_DIR / ("build-cuda" if cuda else "build")
    if args.clean and build_dir.exists():
        print(f"Removing {build_dir}")
        shutil.rmtree(build_dir)

    configure = [
        vs2019_env.CMAKE_EXE, "-S", SOURCE_DIR, "-B", build_dir, "-G", "Ninja",
        f"-DCMAKE_MAKE_PROGRAM={cmake_path(vs2019_env.NINJA_EXE)}",
        f"-DCMAKE_BUILD_TYPE={args.config}",
        f"-DCMAKE_C_COMPILER={cmake_path(layout['vc_bin'] / 'cl.exe')}",
        f"-DCMAKE_CXX_COMPILER={cmake_path(layout['vc_bin'] / 'cl.exe')}",
        f"-DCMAKE_RC_COMPILER={cmake_path(layout['sdk_bin'] / 'rc.exe')}",
        f"-DCMAKE_MT={cmake_path(layout['sdk_bin'] / 'mt.exe')}",
    ] + COMMON_CMAKE_ARGS
    if cuda:
        configure += cuda_cmake_args(cuda, args.cuda_arch) + [
            f"-DCMAKE_CUDA_COMPILER={cmake_path(cuda / 'bin' / 'nvcc.exe')}",
        ]
    run(configure + desktop_cmake_args(cuda) + args.cmake_arg, env)
    run([vs2019_env.CMAKE_EXE, "--build", build_dir, "-j", str(args.jobs)], env)
    return build_dir / "bin"


def build_sln(args, layout, cuda=None):
    """Generate the VS 2019 solution against the portable (unregistered) VS instance and build it."""
    build_dir = SOURCE_DIR / ("build-vs2019-cuda" if cuda else "build-vs2019")
    if args.clean and build_dir.exists():
        print(f"Removing {build_dir}")
        shutil.rmtree(build_dir)
    env = vs2019_env.get_msbuild_env()
    toolset = "v142,host=x64"
    if cuda:
        install_cuda_msbuild_integration(cuda)
        toolset += f",cuda={cuda}"
        env["PATH"] = f"{cuda / 'bin'};{env.get('PATH', '')}"

    configure = [
        vs2019_env.CMAKE_EXE, "-S", SOURCE_DIR, "-B", build_dir,
        "-G", "Visual Studio 16 2019",
        # Plain "x64": ggml compares CMAKE_GENERATOR_PLATFORM literally, so "x64,version=..." would
        # silently drop the AVX2 build. The SDK version is pinned via CMAKE_SYSTEM_VERSION instead.
        "-A", "x64",
        f"-DCMAKE_SYSTEM_VERSION={layout['sdk_version']}",
        # host=x64: only the x64-hosted compiler is installed.
        "-T", toolset,
        f"-DCMAKE_GENERATOR_INSTANCE={cmake_path(vs2019_env.MSVC_ROOT)},version={vs2019_env.instance_version()}",
    ] + COMMON_CMAKE_ARGS
    if cuda:
        configure += cuda_cmake_args(cuda, args.cuda_arch)
    run(configure + desktop_cmake_args(cuda) + args.cmake_arg, env)

    solution = build_dir / "stable-diffusion.sln"
    print(f"Solution: {solution}")
    if args.no_build:
        return None
    run([vs2019_env.MSBUILD_EXE, solution, f"/p:Configuration={args.config}", "/p:Platform=x64",
         f"/m:{args.jobs}", "/nologo", "/v:minimal", "/clp:Summary"], env)
    return build_dir / "bin" / args.config


def dll_imports(dumpbin, binary, env):
    output = subprocess.run([str(dumpbin), "/nologo", "/dependents", str(binary)],
                            env=env, capture_output=True, text=True).stdout
    return [line.strip() for line in output.splitlines() if re.fullmatch(r"[\w.\-]+\.dll", line.strip(), re.I)]


def copy_runtime_dlls(bin_dir, env, layout, extra_dirs=()):
    """Copy the runtime DLLs the executables import (MSVC, and CUDA for the GPU engine),
    so no VC++ redistributable or CUDA toolkit is needed on the target PC. nvcuda.dll is
    never copied: it belongs to the NVIDIA driver."""
    if not layout["redist_x64"]:
        print("warning: no MSVC redist folder found; runtime DLLs not copied")
        return []
    redist_dirs = [d for d in layout["redist_x64"].iterdir()
                   if d.is_dir() and re.fullmatch(r"Microsoft\.VC\d+\.(CRT|OpenMP)", d.name)] + list(extra_dirs)
    dumpbin = layout["vc_bin"] / "dumpbin.exe"

    queue = list(bin_dir.glob("*.exe"))
    seen, copied = set(), []
    while queue:
        binary = queue.pop()
        for dll in dll_imports(dumpbin, binary, env):
            if dll.lower() in seen:
                continue
            seen.add(dll.lower())
            source = next((d / dll for d in redist_dirs if (d / dll).exists()), None)
            if source:
                target = bin_dir / dll
                shutil.copy2(source, target)
                copied.append(dll)
                queue.append(target)
    return copied


def stage(bin_dir, dist_dir):
    dist_dir.mkdir(parents=True, exist_ok=True)
    for item in bin_dir.iterdir():
        if item.suffix.lower() in (".exe", ".dll") and item.name not in DESKTOP_FILES:
            shutil.copy2(item, dist_dir / item.name)
    print(f"Staged executables and runtime DLLs in {dist_dir}")
    for item in sorted(dist_dir.iterdir()):
        print(f"  {item.name:<28} {item.stat().st_size:>14,}")


def stage_desktop(bin_dir, env, layout):
    """dist/SD-Studio: the desktop app plus the DLLs it imports. From there it finds the engines
    in dist/bin and dist/bin-cuda and the models in models/ (package_app.py makes a standalone copy)."""
    app = bin_dir / "SD-Studio.exe"
    if not app.exists():
        return
    DIST_APP.mkdir(parents=True, exist_ok=True)
    shutil.copy2(app, DIST_APP / app.name)
    for dll in dll_imports(layout["vc_bin"] / "dumpbin.exe", app, env):
        if (bin_dir / dll).exists():
            shutil.copy2(bin_dir / dll, DIST_APP / dll)
    print(f"Staged the SD-Studio desktop app in {DIST_APP}")
    for item in sorted(DIST_APP.iterdir()):
        if item.is_file():
            print(f"  {item.name:<28} {item.stat().st_size:>14,}")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--config", default="Release", choices=["Release", "RelWithDebInfo", "MinSizeRel", "Debug"])
    parser.add_argument("-j", "--jobs", type=int, default=32)
    parser.add_argument("--sln", action="store_true", help="use a Visual Studio 2019 solution + MSBuild instead of Ninja")
    parser.add_argument("--no-build", action="store_true", help="with --sln: only generate the solution")
    parser.add_argument("--cuda", action="store_true", help="build the NVIDIA CUDA engine (dist/bin-cuda)")
    parser.add_argument("--cuda-arch", default=DEFAULT_CUDA_ARCHS,
                        help=f"CMAKE_CUDA_ARCHITECTURES (default: {DEFAULT_CUDA_ARCHS})")
    parser.add_argument("--clean", action="store_true", help="delete the build directory first")
    parser.add_argument("--frontend", action="store_true", help="run build_frontend.py before compiling")
    parser.add_argument("--cmake-arg", action="append", default=[], metavar="ARG",
                        help="extra CMake configure argument (repeatable), e.g. --cmake-arg=-DSD_WEBM=OFF")
    args = parser.parse_args()

    try:
        layout = vs2019_env.detect()
        env = vs2019_env.get_env()
    except vs2019_env.ToolchainError as error:
        sys.exit(str(error))
    print(f"[vs2019_env] {vs2019_env.describe()}")

    if args.frontend:
        run([sys.executable, ROOT / "build_frontend.py"], env=None)
    if FRONTEND_HEADER.exists():
        print(f"WebUI header present, embedding it: {FRONTEND_HEADER}")
    else:
        print("WebUI header not present: sd-server is built without the embedded WebUI (run build_frontend.py)")

    cuda = None
    if args.cuda:
        cuda = find_cuda()
        vs2019_env.ensure_vcvarsall()  # nvcc refuses a host compiler without VC\Auxiliary\Build\vcvarsall.bat
        env["PATH"] = f"{cuda / 'bin'};{env['PATH']}"
        print(f"CUDA {cuda.name} from {cuda}, architectures {args.cuda_arch}")

    if args.sln:
        bin_dir = build_sln(args, layout, cuda)
        if bin_dir is None:
            return
    else:
        bin_dir = build_ninja(args, env, layout, cuda)
    copied = copy_runtime_dlls(bin_dir, env, layout, extra_dirs=[cuda / "bin"] if cuda else [])
    print(f"Runtime DLLs copied to {bin_dir}: {', '.join(copied) or 'none'}")
    stage(bin_dir, DIST_BIN_CUDA if cuda else DIST_BIN)
    if not cuda:
        stage_desktop(bin_dir, env, layout)


if __name__ == "__main__":
    main()
