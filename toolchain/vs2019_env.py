"""Portable Visual Studio 2019 (MSVC v142 / 14.29) x64 developer environment.

Python equivalent of "vcvars64.bat" for the toolset installed by get_msvc2019.py.

    import vs2019_env
    env = vs2019_env.get_env()              # dict for subprocess.run(..., env=env)

    python toolchain/vs2019_env.py          # print the detected toolset
    python toolchain/vs2019_env.py cl       # run a command inside the environment

Optional overrides: SD_VCTOOLS_VERSION (e.g. 14.29.30133), SD_WINSDK_VERSION (e.g. 10.0.19041.0).
"""

import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

TOOLCHAIN_DIR = Path(__file__).resolve().parent
MSVC_ROOT = TOOLCHAIN_DIR / "msvc2019"
VC_ROOT = MSVC_ROOT / "VC"
SDK_ROOT = MSVC_ROOT / "Windows Kits" / "10"
CMAKE_EXE = TOOLCHAIN_DIR / "cmake" / "bin" / "cmake.exe"
NINJA_EXE = TOOLCHAIN_DIR / "ninja" / "ninja.exe"
MSBUILD_EXE = MSVC_ROOT / "MSBuild" / "Current" / "Bin" / "amd64" / "MSBuild.exe"
TOOLCHAIN_INFO = MSVC_ROOT / "toolchain_info.json"

# Variables from a Developer Prompt / get_env() that would steer MSBuild away from its own instance.
_DEV_PROMPT_VARS = {
    "INCLUDE", "LIB", "LIBPATH", "VCINSTALLDIR", "VCTOOLSINSTALLDIR", "VCTOOLSVERSION", "VCTOOLSREDISTDIR",
    "WINDOWSSDKDIR", "WINDOWSSDKLIBVERSION", "WINDOWSSDKBINPATH", "WINDOWSSDKVERBINPATH", "UCRTVERSION",
    "UNIVERSALCRTSDKDIR", "VSCMD_ARG_TGT_ARCH", "VSCMD_ARG_HOST_ARCH", "VSCMD_VER", "PLATFORM",
    "VISUALSTUDIOVERSION",
}


class ToolchainError(RuntimeError):
    pass


def _version_key(name):
    return tuple(int(part) for part in name.split("."))


def _newest_version_dir(parent, must_contain):
    if not parent.is_dir():
        return None
    candidates = [
        child.name
        for child in parent.iterdir()
        if child.is_dir() and re.fullmatch(r"\d+(\.\d+)+", child.name) and (child / must_contain).exists()
    ]
    return max(candidates, key=_version_key) if candidates else None


def detect():
    """Return the toolset layout as a dict of paths/versions, or raise ToolchainError."""
    if not (VC_ROOT / "Tools" / "MSVC").is_dir():
        raise ToolchainError(
            f"Portable MSVC not found under {MSVC_ROOT}. "
            f'Run once while online: python "{TOOLCHAIN_DIR / "get_msvc2019.py"}" --accept-license'
        )

    vc_version = os.environ.get("SD_VCTOOLS_VERSION") or _newest_version_dir(
        VC_ROOT / "Tools" / "MSVC", Path("bin/Hostx64/x64/cl.exe"))
    sdk_version = os.environ.get("SD_WINSDK_VERSION") or _newest_version_dir(
        SDK_ROOT / "Include", Path("um/windows.h"))
    redist_version = _newest_version_dir(VC_ROOT / "Redist" / "MSVC", Path("x64/Microsoft.VC142.CRT"))
    if not vc_version or not sdk_version:
        raise ToolchainError(f"Could not find an MSVC toolset and Windows SDK under {MSVC_ROOT}")

    vc_tools = VC_ROOT / "Tools" / "MSVC" / vc_version
    sdk_bin = SDK_ROOT / "bin" / sdk_version / "x64"
    layout = {
        "vc_version": vc_version,
        "sdk_version": sdk_version,
        "redist_version": redist_version,
        "vc_tools": vc_tools,
        "vc_bin": vc_tools / "bin" / "Hostx64" / "x64",
        "sdk_include": SDK_ROOT / "Include" / sdk_version,
        "sdk_lib": SDK_ROOT / "Lib" / sdk_version,
        "sdk_bin": sdk_bin,
        "redist_x64": (VC_ROOT / "Redist" / "MSVC" / redist_version / "x64") if redist_version else None,
    }
    required = [
        layout["vc_bin"] / "cl.exe",
        layout["sdk_include"] / "um" / "windows.h",
        layout["sdk_lib"] / "um" / "x64" / "kernel32.lib",
        layout["sdk_lib"] / "ucrt" / "x64" / "ucrt.lib",
        sdk_bin / "rc.exe",
        sdk_bin / "mt.exe",
    ]
    missing = [str(path) for path in required if not path.exists()]
    if missing:
        raise ToolchainError("Toolchain incomplete, missing:\n  " + "\n  ".join(missing))
    return layout


def get_env(base=None):
    """Return a copy of `base` (default: os.environ) with the VS 2019 x64 environment applied."""
    layout = detect()
    env = dict(os.environ if base is None else base)
    vc_tools = layout["vc_tools"]
    sdk_version = layout["sdk_version"]
    inc = layout["sdk_include"]
    lib = layout["sdk_lib"]

    env.update({
        "VSCMD_ARG_HOST_ARCH": "x64",
        "VSCMD_ARG_TGT_ARCH": "x64",
        "VSCMD_VER": "16.11",
        "VisualStudioVersion": "16.0",
        "Platform": "x64",
        "VCINSTALLDIR": f"{VC_ROOT}\\",
        "VCToolsInstallDir": f"{vc_tools}\\",
        "VCToolsVersion": layout["vc_version"],
        "VCToolsRedistDir": f"{layout['redist_x64'].parent}\\" if layout["redist_x64"] else "",
        "WindowsSdkDir": f"{SDK_ROOT}\\",
        "WindowsSDKVersion": f"{sdk_version}\\",
        "WindowsSDKLibVersion": f"{sdk_version}\\",
        "WindowsSdkBinPath": f"{SDK_ROOT / 'bin'}\\",
        "WindowsSdkVerBinPath": f"{SDK_ROOT / 'bin' / sdk_version}\\",
        "UniversalCRTSdkDir": f"{SDK_ROOT}\\",
        "UCRTVersion": sdk_version,
        "INCLUDE": ";".join(str(p) for p in [
            vc_tools / "include", inc / "ucrt", inc / "shared", inc / "um", inc / "winrt", inc / "cppwinrt"]),
        "LIB": ";".join(str(p) for p in [vc_tools / "lib" / "x64", lib / "ucrt" / "x64", lib / "um" / "x64"]),
        "LIBPATH": ";".join(str(p) for p in [
            vc_tools / "lib" / "x64",
            vc_tools / "lib" / "x86" / "store" / "references",
            SDK_ROOT / "UnionMetadata" / sdk_version,
            SDK_ROOT / "References" / sdk_version]),
    })

    tool_paths = [str(p) for p in [
        layout["vc_bin"], layout["sdk_bin"], layout["sdk_bin"] / "ucrt", CMAKE_EXE.parent, NINJA_EXE.parent,
    ] if p.exists()]
    # Prepend our tools; drop existing copies so repeated calls do not grow PATH.
    ours = {p.lower() for p in tool_paths}
    rest = [p for p in env.get("PATH", "").split(os.pathsep) if p and p.rstrip("\\").lower() not in ours]
    env["PATH"] = os.pathsep.join(tool_paths + rest)
    return env


def instance_version():
    """4-part VS 2019 build version of the portable instance (CMAKE_GENERATOR_INSTANCE needs it)."""
    if not MSBUILD_EXE.exists() or not TOOLCHAIN_INFO.exists():
        raise ToolchainError(
            f"Portable MSBuild not found at {MSBUILD_EXE}. "
            f'Run once while online: python "{TOOLCHAIN_DIR / "get_msvc2019.py"}" --accept-license')
    return json.loads(TOOLCHAIN_INFO.read_text(encoding="utf-8"))["vs_instance_version"]


def get_msbuild_env(base=None):
    """Environment for CMake's "Visual Studio 16 2019" generator and MSBuild with the portable instance.

    MSBuild derives compiler/include/lib paths from its own location; only the Windows SDK lookups,
    which normally read the registry, are redirected here. Without them a registered SDK (e.g. 10.0.26100
    under Program Files) would silently be used instead of the portable 10.0.19041.
    """
    layout = detect()
    env = {k: v for k, v in (os.environ if base is None else base).items() if k.upper() not in _DEV_PROMPT_VARS}
    kits = f"{SDK_ROOT}\\"
    env.update({
        "WindowsSdkDir_10": kits,
        "UniversalCRTSdkDir_10": kits,
        "UCRTContentRoot": kits,
        "CMAKE_WINDOWS_KITS_10_DIR": str(SDK_ROOT),
        # CMake's try_compile projects ignore CMAKE_SYSTEM_VERSION but honour this variable.
        "WindowsSDKVersion": layout["sdk_version"],
    })
    return env


_VCVARSALL_TEMPLATE = r"""@echo off
rem Generated by toolchain\vs2019_env.py for the portable VS 2019 toolset (x64 host and target only).
rem nvcc requires VC\Auxiliary\Build\vcvarsall.bat to exist next to the MSVC tree, otherwise it fails
rem with "Host compiler targets unsupported OS". Calling it also sets up the x64 build environment.
for %%I in ("%~dp0..\..") do set "VCINSTALLDIR=%%~fI\"
for %%I in ("%~dp0..\..\..\Windows Kits\10") do set "WindowsSdkDir=%%~fI\"
set "VCToolsVersion={vc_version}"
set "VCToolsInstallDir=%VCINSTALLDIR%Tools\MSVC\{vc_version}\"
set "WindowsSDKVersion={sdk_version}\"
set "WindowsSDKLibVersion={sdk_version}\"
set "UniversalCRTSdkDir=%WindowsSdkDir%"
set "UCRTVersion={sdk_version}"
set "INCLUDE=%VCToolsInstallDir%include;%WindowsSdkDir%Include\{sdk_version}\ucrt;%WindowsSdkDir%Include\{sdk_version}\shared;%WindowsSdkDir%Include\{sdk_version}\um;%WindowsSdkDir%Include\{sdk_version}\winrt;%WindowsSdkDir%Include\{sdk_version}\cppwinrt"
set "LIB=%VCToolsInstallDir%lib\x64;%WindowsSdkDir%Lib\{sdk_version}\ucrt\x64;%WindowsSdkDir%Lib\{sdk_version}\um\x64"
set "LIBPATH=%VCToolsInstallDir%lib\x64"
set "PATH=%VCToolsInstallDir%bin\Hostx64\x64;%WindowsSdkDir%bin\{sdk_version}\x64;%PATH%"
set "VSCMD_ARG_HOST_ARCH=x64"
set "VSCMD_ARG_TGT_ARCH=x64"
set "Platform=x64"
exit /b 0
"""


def ensure_vcvarsall():
    """Create VC\\Auxiliary\\Build\\vcvarsall.bat, which nvcc insists on finding (see template)."""
    layout = detect()
    target = VC_ROOT / "Auxiliary" / "Build" / "vcvarsall.bat"
    content = _VCVARSALL_TEMPLATE.format(vc_version=layout["vc_version"], sdk_version=layout["sdk_version"])
    content = content.replace("\n", "\r\n")
    if not target.exists() or target.read_text(encoding="ascii", errors="replace") != content.replace("\r\n", "\n"):
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(content.encode("ascii"))
    return target


def which(program, env):
    """Resolve `program` against env["PATH"] (CreateProcess would search the parent's PATH instead)."""
    return shutil.which(program, path=env.get("PATH")) or program


def describe():
    layout = detect()
    return (f"MSVC {layout['vc_version']} (x64->x64), Windows SDK {layout['sdk_version']}, "
            f"CMake/Ninja from {TOOLCHAIN_DIR}")


if __name__ == "__main__":
    try:
        print(f"[vs2019_env] {describe()}")
        if len(sys.argv) > 1:
            run_env = get_env()
            sys.exit(subprocess.call([which(sys.argv[1], run_env)] + sys.argv[2:], env=run_env))
    except ToolchainError as error:
        sys.exit(f"[vs2019_env] {error}")
