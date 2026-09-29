#!/usr/bin/env python3
r"""
Portable Visual Studio 2019 (MSVC v142 / 14.29) + MSBuild 16.11 + VC++ MSBuild
targets (v160) + Windows SDK + CMake + Ninja, installed without admin rights and
without touching the registry.

Based on the approach of mmozeiko's portable-msvc.py gist
(https://gist.github.com/mmozeiko/7f3162ec2988e81e56d5c4e22cde9977), but pinned
to the Visual Studio 2019 release channel (https://aka.ms/vs/16/release/channel).

  channel -> Microsoft.VisualStudio.Manifests.VisualStudio payload (vsman)
          -> package list -> VSIX payloads (MSVC, MSBuild, VC targets)
                           + MSI/CAB payloads (Windows SDK)

The output folder is laid out like a Visual Studio 2019 instance:
  <out>\VC\Tools\MSVC\14.29.30133\...            compiler, libs, headers
  <out>\VC\Redist\MSVC\14.29.30133\x64\...       vcruntime/msvcp/vcomp DLLs
  <out>\VC\Auxiliary\Build\Microsoft.VCToolsVersion*.{txt,props}
  <out>\MSBuild\Current\Bin\[amd64\]MSBuild.exe  MSBuild 16.11 (+ Tracker.exe)
  <out>\MSBuild\Microsoft\VC\v160\...            VC++ targets, x64 + v142 toolset
  <out>\Windows Kits\10\...                      Windows SDK / UCRT
so it can be used as an *unregistered* instance by CMake's
"Visual Studio 16 2019" generator (CMAKE_GENERATOR_INSTANCE=<out>,version=...).
The MSBuild layer is included by default (skip with --no-msbuild).

VSIX files are zip archives; only their Contents/ subtree is extracted (*.bat
files are skipped). Windows SDK MSIs are extracted with an administrative
install (msiexec /a ... TARGETDIR=...), which does not need elevation. If that
fails, lessmsi (downloaded from GitHub) is used as a fallback.

All payload sha256 hashes from the manifest are verified. Already downloaded
files whose hash matches are reused, and payloads already extracted into the
output (recorded in <out>\\.extracted_payloads.json) are not extracted again,
so the script is cheap to re-run. Use --force-extract to re-extract everything.

Usage (defaults are what was used for this machine):
  python get_msvc2019.py --accept-license
  python get_msvc2019.py --show-versions
  python get_msvc2019.py --accept-license --sdk-version 22621
"""

import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
import time
import urllib.parse
import urllib.request
import zipfile
import zlib
from pathlib import Path

HERE = Path(__file__).resolve().parent

CHANNEL_URL = "https://aka.ms/vs/16/release/channel"
VS_MANIFEST_ITEM = "Microsoft.VisualStudio.Manifests.VisualStudio"

CMAKE_VERSION_DEFAULT = "3.31.12"
NINJA_VERSION_DEFAULT = "1.13.2"
LESSMSI_URL = "https://github.com/activescott/lessmsi/releases/download/v2.12.9/lessmsi-v2.12.9.zip"

HOST = "x64"
TARGET = "x64"

# MSBuild 16.11 needs .NET Framework 4.7.2+ (release key 461808).
NETFX_MIN_RELEASE = 461808

LEDGER_NAME = ".extracted_payloads.json"


def log(msg):
    print(msg, flush=True)


def fetch(url):
    req = urllib.request.Request(url, headers={"User-Agent": "get_msvc2019.py"})
    with urllib.request.urlopen(req) as res:
        return res.read()


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(1 << 20), b""):
            h.update(block)
    return h.hexdigest()


def download(url, dest: Path, sha256=None):
    """Download url to dest; reuse existing file if its hash matches."""
    dest.parent.mkdir(parents=True, exist_ok=True)
    if dest.exists() and sha256 and sha256_file(dest) == sha256.lower():
        log(f"  {dest.name} ... cached OK")
        return dest
    tmp = dest.with_suffix(dest.suffix + ".part")
    req = urllib.request.Request(url, headers={"User-Agent": "get_msvc2019.py"})
    with urllib.request.urlopen(req) as res, open(tmp, "wb") as f:
        total = int(res.headers.get("Content-Length") or 0)
        done = 0
        while True:
            block = res.read(1 << 20)
            if not block:
                break
            f.write(block)
            done += len(block)
            if total:
                print(f"\r  {dest.name} ... {done * 100 // total}%", end="", flush=True)
    print(flush=True)
    if sha256:
        got = sha256_file(tmp)
        if got != sha256.lower():
            tmp.unlink()
            sys.exit(f"ERROR: sha256 mismatch for {dest.name}: expected {sha256.lower()}, got {got}")
    os.replace(tmp, dest)
    return dest


def first(items, cond=lambda x: True):
    return next((item for item in items if cond(item)), None)


def get_msi_cabs(msi_bytes):
    """Super crappy MSI parser (same trick as the gist): cab names are 32 chars + '.cab'."""
    cabs = []
    index = 0
    while True:
        index = msi_bytes.find(b".cab", index + 4)
        if index < 0:
            return cabs
        name = msi_bytes[index - 32:index + 4].decode("ascii", errors="replace")
        if name not in cabs:
            cabs.append(name)


def file_crc32(path):
    crc = 0
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(1 << 20), b""):
            crc = zlib.crc32(block, crc)
    return crc & 0xFFFFFFFF


def extract_vsix(vsix: Path, output: Path, skip_suffixes=(".bat",)):
    with zipfile.ZipFile(vsix) as z:
        for info in z.infolist():
            name = info.filename
            if not name.startswith("Contents/") or name.endswith("/"):
                continue
            rel = urllib.parse.unquote(name[len("Contents/"):])
            if rel.lower().endswith(skip_suffixes):
                continue
            out = output / rel
            # Keep the payload's own timestamps (as the VS installer does): re-running this
            # script must not make headers/libs look newer than existing build outputs.
            mtime = time.mktime(info.date_time + (0, 0, -1))
            if out.exists() and out.stat().st_size == info.file_size and file_crc32(out) == info.CRC:
                # identical; not rewriting also avoids DLLs held open by e.g. mspdbsrv.exe
                if abs(out.stat().st_mtime - mtime) > 2:
                    try:
                        os.utime(out, (mtime, mtime))
                    except OSError:
                        pass
                continue
            out.parent.mkdir(parents=True, exist_ok=True)
            with z.open(info) as src, open(out, "wb") as dst:
                shutil.copyfileobj(src, dst)
            os.utime(out, (mtime, mtime))


def msiexec_admin(msi: Path, target: Path, logfile: Path):
    # Build the command line by hand: msiexec wants PROPERTY="value with spaces".
    # Keep TARGETDIR short: msiexec fails with error 1304 beyond MAX_PATH.
    cmd = f'msiexec.exe /a "{msi}" /quiet /qn /norestart TARGETDIR="{target}" /L*v "{logfile}"'
    return subprocess.call(cmd)


def lessmsi_extract(lessmsi_exe: Path, msi: Path, target: Path, downloads: Path):
    # lessmsi extracts to <outdir>\SourceDir\...; move that content into target.
    with tempfile.TemporaryDirectory(dir=downloads) as d:
        out = Path(d) / "out"
        out.mkdir()
        subprocess.check_call([str(lessmsi_exe), "x", str(msi), str(out) + "\\"])
        src = out / "SourceDir"
        if not src.exists():
            src = out
        for item in src.rglob("*"):
            if item.is_file():
                dst = target / item.relative_to(src)
                dst.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(item, dst)


def get_lessmsi(downloads: Path):
    exe = downloads / "lessmsi" / "lessmsi.exe"
    if not exe.exists():
        z = download(LESSMSI_URL, downloads / "lessmsi.zip")
        with zipfile.ZipFile(z) as zf:
            zf.extractall(downloads / "lessmsi")
    return exe


def check_netfx():
    """Read-only check of the .NET Framework 4.x release key (MSBuild 16.11 needs 4.7.2+)."""
    try:
        import winreg
        with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r"SOFTWARE\Microsoft\NET Framework Setup\NDP\v4\Full") as k:
            release = winreg.QueryValueEx(k, "Release")[0]
            version = winreg.QueryValueEx(k, "Version")[0]
    except OSError:
        log("WARNING: .NET Framework 4.x not found; MSBuild.exe needs .NET Framework 4.7.2 or later.")
        return None
    ok = release >= NETFX_MIN_RELEASE
    log(f".NET Framework {version} (release {release}) ... {'OK' if ok else 'TOO OLD for MSBuild 16.11 (need 4.7.2+)'}")
    return version


class Ledger:
    """Remembers which payloads (by sha256) have already been extracted into the output."""

    def __init__(self, output: Path, force: bool):
        self.path = output / LEDGER_NAME
        self.data = {}
        if self.path.exists() and not force:
            try:
                self.data = json.loads(self.path.read_text())
            except ValueError:
                self.data = {}

    def done(self, name, sha256):
        return self.data.get(name) == sha256.lower()

    def mark(self, name, sha256):
        self.data[name] = sha256.lower()
        self.path.write_text(json.dumps(self.data, indent=1, sort_keys=True))


def install_cmake_ninja(toolchain: Path, downloads: Path, cmake_version: str, ninja_version: str):
    # CMake: verify against Kitware's published SHA-256 list.
    cmake_dir = toolchain / "cmake"
    cmake_zip_name = f"cmake-{cmake_version}-windows-x86_64.zip"
    base = f"https://github.com/Kitware/CMake/releases/download/v{cmake_version}"
    if not (cmake_dir / "bin" / "cmake.exe").exists():
        sums = fetch(f"{base}/cmake-{cmake_version}-SHA-256.txt").decode()
        want = first(l.split()[0] for l in sums.splitlines() if l.strip().endswith(cmake_zip_name))
        if not want:
            sys.exit(f"ERROR: no sha256 for {cmake_zip_name}")
        z = download(f"{base}/{cmake_zip_name}", downloads / cmake_zip_name, want)
        log(f"Extracting CMake {cmake_version} -> {cmake_dir}")
        with tempfile.TemporaryDirectory(dir=downloads) as d:
            with zipfile.ZipFile(z) as zf:
                zf.extractall(d)
            inner = Path(d) / cmake_zip_name[:-4]
            if cmake_dir.exists():
                shutil.rmtree(cmake_dir)
            shutil.move(str(inner), str(cmake_dir))
    else:
        log(f"CMake already present in {cmake_dir}")

    # Ninja: no official checksum file; record the hash we got.
    ninja_dir = toolchain / "ninja"
    if not (ninja_dir / "ninja.exe").exists():
        z = download(f"https://github.com/ninja-build/ninja/releases/download/v{ninja_version}/ninja-win.zip",
                     downloads / f"ninja-win-{ninja_version}.zip")
        log(f"  ninja-win.zip sha256 = {sha256_file(z)}")
        ninja_dir.mkdir(parents=True, exist_ok=True)
        with zipfile.ZipFile(z) as zf:
            zf.extractall(ninja_dir)
    else:
        log(f"Ninja already present in {ninja_dir}")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--output", default=str(HERE / "msvc2019"), help="output folder (the portable VS instance)")
    ap.add_argument("--downloads", default=str(HERE / "_downloads"), help="download cache folder")
    ap.add_argument("--accept-license", action="store_true", help="accept the VS 2019 Build Tools license")
    ap.add_argument("--show-versions", action="store_true", help="show MSVC and Windows SDK versions in the VS 2019 channel")
    ap.add_argument("--msvc-version", default=None,
                    help="MSVC package version prefix, e.g. 14.29.16.11 (default: newest 14.29.16.x)")
    ap.add_argument("--sdk-version", default=None,
                    help="Windows SDK build, e.g. 19041 or 22621 (default: newest Windows 10 SDK)")
    ap.add_argument("--no-msbuild", action="store_true",
                    help="skip MSBuild + VC++ MSBuild targets (only needed for the Visual Studio generator / .sln)")
    ap.add_argument("--force-extract", action="store_true", help="re-extract payloads even if already extracted")
    ap.add_argument("--no-cmake-ninja", action="store_true", help="skip CMake/Ninja download")
    ap.add_argument("--cmake-version", default=CMAKE_VERSION_DEFAULT)
    ap.add_argument("--ninja-version", default=NINJA_VERSION_DEFAULT)
    args = ap.parse_args()

    output = Path(args.output).resolve()
    downloads = Path(args.downloads).resolve()

    log(f"Reading channel {CHANNEL_URL}")
    channel = json.loads(fetch(CHANNEL_URL))
    log(f"  {channel['info']['productName']} {channel['info']['productDisplayVersion']} ({channel['info']['buildVersion']})")
    vs_item = first(channel["channelItems"], lambda x: x["id"] == VS_MANIFEST_ITEM)
    vsman_url = vs_item["payloads"][0]["url"]
    vsman = json.loads(fetch(vsman_url))

    packages = {}
    for p in vsman["packages"]:
        packages.setdefault(p["id"].lower(), []).append(p)

    # MSVC toolsets: packages named microsoft.vc.<ver>.tools.hostx64.targetx64.base
    msvc = {}
    for pid in packages:
        if pid.startswith("microsoft.vc.") and pid.endswith(f".tools.host{HOST}.target{TARGET}.base"):
            ver = pid[len("microsoft.vc."):-len(f".tools.host{HOST}.target{TARGET}.base")]
            if all(part.isdigit() for part in ver.split(".")):
                msvc[ver] = packages[pid][0]["version"]
    sdks = {}
    for pid in packages:
        for prefix in ("microsoft.visualstudio.component.windows10sdk.", "microsoft.visualstudio.component.windows11sdk."):
            if pid.startswith(prefix) and pid[len(prefix):].isdigit():
                sdks[pid[len(prefix):]] = (pid, "10" if "windows10sdk" in prefix else "11")

    def vkey(s):
        return [int(x) for x in s.split(".")]

    if args.show_versions:
        for v in sorted(msvc, key=vkey):
            log(f"MSVC {v}  (package version {msvc[v]})")
        for v in sorted(sdks):
            log(f"Windows {sdks[v][1]} SDK {v}  ({sdks[v][0]})")
        return

    if args.msvc_version:
        msvc_ver = args.msvc_version.lower()
        if msvc_ver not in msvc:
            sys.exit(f"Unknown MSVC version {msvc_ver}; available: {sorted(msvc, key=vkey)}")
    else:
        msvc_ver = max((v for v in msvc if v.startswith("14.29")), key=vkey)

    if args.sdk_version:
        sdk_ver = args.sdk_version
        if sdk_ver not in sdks:
            sys.exit(f"Unknown SDK version {sdk_ver}; available: {sorted(sdks)}")
    else:
        sdk_ver = max(v for v in sdks if sdks[v][1] == "10")
    sdk_pid = sdks[sdk_ver][0]

    log(f"Selected MSVC {msvc_ver} (package {msvc[msvc_ver]}), Windows SDK {sdk_ver} ({sdk_pid})")

    tools = first(channel["channelItems"], lambda x: x["id"] == "Microsoft.VisualStudio.Product.BuildTools")
    resource = first(tools["localizedResources"], lambda x: x["language"].lower() == "en-us")
    license_url = resource["license"]
    log(f"Visual Studio 2019 Build Tools license: {license_url}")
    if not args.accept_license:
        accept = input("Do you accept the Visual Studio license? [Y/N] ")
        if not accept or accept[0].lower() != "y":
            return

    netfx = check_netfx() if not args.no_msbuild else None

    output.mkdir(parents=True, exist_ok=True)
    downloads.mkdir(parents=True, exist_ok=True)
    ledger = Ledger(output, args.force_extract)

    # ------------------------------------------------------------------ MSVC
    v = msvc_ver
    vsix_packages = [
        f"microsoft.vc.{v}.crt.headers.base",
        f"microsoft.vc.{v}.tools.host{HOST}.target{TARGET}.base",
        f"microsoft.vc.{v}.tools.host{HOST}.target{TARGET}.res.base",
        f"microsoft.vc.{v}.tools.host{HOST}.target{TARGET}.resources.base",   # 14.29.16.10 naming
        f"microsoft.vc.{v}.crt.{TARGET}.desktop.base",
        f"microsoft.vc.{v}.crt.{TARGET}.store.base",
        f"microsoft.vc.{v}.premium.tools.host{HOST}.target{TARGET}.base",
        f"microsoft.vc.{v}.prem.host{HOST}.target{TARGET}.res.base",
        f"microsoft.vc.{v}.crt.redist.{TARGET}.base",       # vcruntime/msvcp + Microsoft.VC142.OpenMP (vcomp140.dll)
    ]
    redist_pkg = f"microsoft.vc.{v}.crt.redist.{TARGET}.base"
    if redist_pkg not in packages:
        redist = first(packages.get(f"microsoft.visualcpp.crt.redist.{TARGET}", []))
        if redist:
            dep = first(redist.get("dependencies", {}), lambda d: d.lower().endswith(".base"))
            if dep:
                vsix_packages.append(dep.lower())

    # --------------------------------------------- MSBuild + VC++ MSBuild targets
    # Dependency closure of Microsoft.Component.MSBuild (minus NuGet/Roslyn/.NET Core
    # extras that C++ projects do not use) and of the VC.Tools.x86.x64 component's
    # MSBuild parts, x64 only. Microsoft.Build.Dependencies already carries
    # Tracker.exe/FileTracker*.dll, so Microsoft.Build.FileTracker.Msi is not needed.
    if not args.no_msbuild:
        vsix_packages += [
            "microsoft.build",                                  # MSBuild\Current\Bin\[amd64\]MSBuild.exe
            "microsoft.build.dependencies",                     # Tracker.exe, FileTracker*.dll, xaml rules
            "microsoft.visualstudio.vc.msbuild.base",           # MSBuild\Microsoft\VC\v160\*.props/targets, CPPTasks
            "microsoft.visualstudio.vc.msbuild.base.resources",
            f"microsoft.visualstudio.vc.msbuild.{TARGET}",      # Platforms\x64\Platform.*
            f"microsoft.visualstudio.vc.msbuild.{TARGET}.v142", # Platforms\x64\PlatformToolsets\v142
            "microsoft.visualcpp.tools.core.x86",               # VC\Auxiliary\Build\Microsoft.VCToolsVersion.default.*
            f"microsoft.vc.{v}.tools.core.props",               # VC\Auxiliary\Build\Microsoft.VCToolsVersion.v142.default.*
            f"microsoft.vc.{v}.props.{TARGET}",                 # VC\Tools\MSVC\<ver>\Auxiliary\Microsoft.VC.Paths.x64.props
            f"microsoft.vc.{v}.servicing",
            f"microsoft.vc.{v}.servicing.compilers",
            f"microsoft.vc.{v}.servicing.crtheaders",
        ]

    manifest_log = []
    for pkg in vsix_packages:
        if pkg not in packages:
            if not pkg.endswith(".resources.base") and not pkg.endswith(".res.base"):
                log(f"  {pkg} ... not in manifest, skipped")
            continue
        p = first(packages[pkg], lambda p: p.get("language") in (None, "en-US", "neutral"))
        if p is None:
            p = packages[pkg][0]
        for payload in p["payloads"]:
            filename = Path(payload["fileName"]).name
            if filename.lower() == "payload.vsix":   # generic name used by many packages
                filename = f"{p['id']}-{p.get('version')}.vsix"
            if ledger.done(filename, payload["sha256"]):
                log(f"  {filename} ... already extracted")
            else:
                f = download(payload["url"], downloads / filename, payload["sha256"])
                extract_vsix(f, output)
                ledger.mark(filename, payload["sha256"])
            manifest_log.append({"id": p["id"], "version": p.get("version"), "file": filename, "sha256": payload["sha256"]})

    # ------------------------------------------------------------ Windows SDK
    sdk_msis = [
        "Windows SDK for Windows Store Apps Tools-x86_en-us.msi",          # rc.exe, mt.exe, ...
        "Windows SDK for Windows Store Apps Headers-x86_en-us.msi",
        "Windows SDK for Windows Store Apps Headers OnecoreUap-x86_en-us.msi",
        "Windows SDK for Windows Store Apps Libs-x86_en-us.msi",
        "Universal CRT Headers Libraries and Sources-x86_en-us.msi",
        "Windows SDK Desktop Headers x64-x86_en-us.msi",
        "Windows SDK Desktop Headers x86-x86_en-us.msi",
        "Windows SDK OnecoreUap Headers x64-x86_en-us.msi",
        "Windows SDK OnecoreUap Headers x86-x86_en-us.msi",
        f"Windows SDK Desktop Libs {TARGET}-x86_en-us.msi",
    ]
    if not args.no_msbuild:
        sdk_msis += [
            # Windows Kits\10\SDKManifest.xml (UAP.props locates WindowsSdkDir through it)
            "Windows SDK for Windows Store Apps-x86_en-us.msi",
            # DesignTime\CommonConfiguration\Neutral\UAP\<ver>\UAP.props (WindowsSDK_IncludePath etc.,
            # and the VC targets' MSB8036 "SDK installed" check) + Platforms\UAP\<ver>\Platform.xml
            "Windows SDK for Windows Store Apps Metadata-x86_en-us.msi",
        ]
    sdk_root_pkg = packages[sdk_pid][0]
    sdk_pkg = packages[first(sdk_root_pkg["dependencies"]).lower()][0]
    log(f"Windows SDK package {sdk_pkg['id']} {sdk_pkg.get('version')}")

    msi_files = []
    cabs = []
    for name in sdk_msis:
        payload = first(sdk_pkg["payloads"], lambda p: p["fileName"] == f"Installers\\{name}")
        if payload is None:
            log(f"  {name} ... not in this SDK, skipped")
            continue
        manifest_log.append({"id": sdk_pkg["id"], "version": sdk_pkg.get("version"), "file": name, "sha256": payload["sha256"]})
        ledger_key = f"{sdk_pkg['id']}/{name}"
        if ledger.done(ledger_key, payload["sha256"]):
            log(f"  {name} ... already extracted")
            continue
        f = download(payload["url"], downloads / name, payload["sha256"])
        msi_files.append((f, ledger_key, payload["sha256"]))
        for cab in get_msi_cabs(f.read_bytes()):
            if cab not in cabs:
                cabs.append(cab)

    for cab in cabs:
        payload = first(sdk_pkg["payloads"], lambda p: p["fileName"] == f"Installers\\{cab}")
        if payload is None:
            continue  # false positive from the byte scan
        download(payload["url"], downloads / cab, payload["sha256"])
        manifest_log.append({"id": sdk_pkg["id"], "version": sdk_pkg.get("version"), "file": cab, "sha256": payload["sha256"]})

    if msi_files:
        log("Unpacking Windows SDK MSIs (msiexec /a, no elevation needed)...")
    lessmsi = None
    logs = downloads / "msilogs"
    logs.mkdir(exist_ok=True)
    for msi, ledger_key, sha in msi_files:
        rc = msiexec_admin(msi, output, logs / (msi.stem + ".log"))
        if rc != 0:
            log(f"  msiexec /a failed for {msi.name} (rc={rc}); falling back to lessmsi")
            lessmsi = lessmsi or get_lessmsi(downloads)
            lessmsi_extract(lessmsi, msi, output, downloads)
        else:
            log(f"  {msi.name} ... OK")
        copied = output / msi.name   # administrative install drops a copy of the MSI into TARGETDIR
        if copied.exists():
            copied.unlink()
        ledger.mark(ledger_key, sha)

    # ---------------------------------------------------------------- summary
    msvc_dirs = sorted((output / "VC" / "Tools" / "MSVC").glob("*"))
    redist_dirs = sorted((output / "VC" / "Redist" / "MSVC").glob("14.*"))
    sdk_inc = output / "Windows Kits" / "10" / "Include"
    sdk_dirs = sorted(d.name for d in sdk_inc.glob("10.*") if (d / "um" / "windows.h").exists())
    sdk_full = f"10.0.{sdk_ver}.0" if f"10.0.{sdk_ver}.0" in sdk_dirs else (sdk_dirs[-1] if sdk_dirs else None)

    def pkg_version(pid):
        p = first(packages.get(pid.lower(), []))
        return p.get("version") if p else None

    info = {
        "channel": CHANNEL_URL,
        "channel_version": channel["info"]["productDisplayVersion"],
        # 4-part VS build version of this channel = what an installed VS 16.11 instance reports;
        # use it for CMAKE_GENERATOR_INSTANCE=<out>,version=<this>
        "vs_instance_version": channel["info"]["buildVersion"],
        "vsman": vsman_url,
        "license": license_url,
        "msvc_package": msvc_ver,
        "VCToolsVersion": msvc_dirs[-1].name if msvc_dirs else None,
        "VCRedistVersion": redist_dirs[-1].name if redist_dirs else None,
        "WindowsSDKVersion": sdk_full,
        "msbuild": None if args.no_msbuild else {
            "Microsoft.Build": pkg_version("Microsoft.Build"),
            "Microsoft.VisualStudio.VC.MSBuild.Base": pkg_version("Microsoft.VisualStudio.VC.MSBuild.Base"),
            "netfx": netfx,
        },
        "payloads": manifest_log,
    }
    (output / "toolchain_info.json").write_text(json.dumps(info, indent=2))
    log(f"VCToolsVersion={info['VCToolsVersion']} VCRedistVersion={info['VCRedistVersion']} WindowsSDKVersion={sdk_full}")

    if not args.no_msbuild:
        vc_ver = info["VCToolsVersion"]
        expected = [
            "MSBuild/Current/Bin/MSBuild.exe",
            "MSBuild/Current/Bin/amd64/MSBuild.exe",
            "MSBuild/Current/Bin/Tracker.exe",
            "MSBuild/Microsoft/VC/v160/Microsoft.Cpp.Default.props",
            "MSBuild/Microsoft/VC/v160/Microsoft.Build.CPPTasks.Common.dll",
            f"MSBuild/Microsoft/VC/v160/Platforms/{TARGET}/PlatformToolsets/v142/Toolset.props",
            "VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt",
            "VC/Auxiliary/Build/Microsoft.VCToolsVersion.v142.default.props",
            f"VC/Tools/MSVC/{vc_ver}/bin/Host{HOST}/{TARGET}/cl.exe",
            "Windows Kits/10/SDKManifest.xml",
            f"Windows Kits/10/DesignTime/CommonConfiguration/Neutral/UAP/{sdk_full}/UAP.props",
            "Windows Kits/10/DesignTime/CommonConfiguration/Neutral/uCRT.props",
        ]
        missing = [e for e in expected if not (output / e).exists()]
        if missing:
            sys.exit("ERROR: VS instance layout incomplete, missing:\n  " + "\n  ".join(missing))
        log(f"VS instance layout OK: {output}  (version={info['vs_instance_version']}, "
            f"MSBuild {info['msbuild']['Microsoft.Build']})")

    if not args.no_cmake_ninja:
        install_cmake_ninja(output.parent, downloads, args.cmake_version, args.ninja_version)

    log("Done.")


if __name__ == "__main__":
    main()
