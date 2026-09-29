"""Download the Microsoft Edge WebView2 SDK (NuGet package Microsoft.Web.WebView2) for SD-Studio.

Only the C++ headers, the x64 loader (WebView2Loader.dll + import library) and the license are kept,
in toolchain/webview2/{include,x64}. Run once while online; builds are offline afterwards.

    python toolchain/get_webview2_sdk.py
    python toolchain/get_webview2_sdk.py --version 1.0.3405.78
"""

import argparse
import base64
import hashlib
import io
import json
import shutil
import sys
import urllib.request
import zipfile
from pathlib import Path

PACKAGE = "microsoft.web.webview2"
FLAT = f"https://api.nuget.org/v3-flatcontainer/{PACKAGE}"
REGISTRATION = f"https://api.nuget.org/v3/registration5-semver1/{PACKAGE}"
DEST = Path(__file__).resolve().parent / "webview2"


def fetch(url):
    with urllib.request.urlopen(url, timeout=120) as response:
        return response.read()


def newest_stable():
    versions = json.loads(fetch(f"{FLAT}/index.json"))["versions"]
    stable = [v for v in versions if "-" not in v]
    return max(stable, key=lambda v: tuple(int(x) for x in v.split(".")))


def expected_sha512(version):
    """packageHash (base64 SHA-512) published in the NuGet catalog for this version, or None."""
    try:
        leaf = json.loads(fetch(f"{REGISTRATION}/{version}.json"))
        catalog = json.loads(fetch(leaf["catalogEntry"]))
        return catalog.get("packageHash")
    except Exception:  # the catalog is best-effort; the package is still extracted
        return None


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--version", help="package version (default: newest stable)")
    args = parser.parse_args()

    version = args.version or newest_stable()
    print(f"Microsoft.Web.WebView2 {version}")
    data = fetch(f"{FLAT}/{version}/{PACKAGE}.{version}.nupkg")
    expected = expected_sha512(version)
    actual = base64.b64encode(hashlib.sha512(data).digest()).decode()
    if expected and expected != actual:
        sys.exit("SHA-512 mismatch against the NuGet catalog")
    print("  hash verified against the NuGet catalog" if expected else "  warning: catalog hash unavailable")

    if DEST.exists():
        shutil.rmtree(DEST)
    wanted = {
        "build/native/include/": DEST / "include",
        "build/native/x64/": DEST / "x64",
    }
    with zipfile.ZipFile(io.BytesIO(data)) as package:
        for name in package.namelist():
            for prefix, target in wanted.items():
                if name.startswith(prefix) and not name.endswith("/"):
                    out = target / name[len(prefix):]
                    out.parent.mkdir(parents=True, exist_ok=True)
                    out.write_bytes(package.read(name))
            if name.lower() == "license.txt":
                (DEST / "LICENSE.txt").write_bytes(package.read(name))
    (DEST / "version.txt").write_text(version + "\n", encoding="ascii")

    for required in ("include/WebView2.h", "x64/WebView2Loader.dll", "x64/WebView2Loader.dll.lib"):
        if not (DEST / required).exists():
            sys.exit(f"missing after extraction: {required}")
    print(f"Done: {DEST}")


if __name__ == "__main__":
    main()
