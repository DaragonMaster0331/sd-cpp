# stable-diffusion.cpp · FLUX.2 [klein] 4B · offline build (VS 2019)

[stable-diffusion.cpp](https://github.com/leejet/stable-diffusion.cpp) (commit `3f8527a`) compiled with the
Visual Studio 2019 C++ toolset (MSVC 19.29 / v142, Windows SDK 10.0.19041) as a **CPU engine** and an
**NVIDIA CUDA engine** (CUDA 12.9), plus **SD-Studio**, a native desktop app (Win32 + WebView2) with the same
UI. Runs FLUX.2 [klein] 4B from local GGUF files; nothing contacts the network at run time.
All scripts are Python 3 (standard library only), except the `test_*.bat` test files.

## SD-Studio desktop app

```
dist\SD-Studio\SD-Studio.exe             # development layout (uses dist\bin, dist\bin-cuda, models\)
python package_app.py                    # standalone copy for offline PCs -> dist\SD-Studio-portable (9 GB)
python package_app.py --dest E:\SD-Studio --no-cuda
```

- One window, no browser, no console. It starts `sd-server.exe` hidden on `127.0.0.1` (port 17861), shows the
  same WebUI, and stops the engine when it closes (Windows job object: the engine cannot outlive the app).
- **Engine** picker in the header (and Settings): *Auto (GPU if available)* / *GPU (NVIDIA CUDA)* / *CPU*.
  Auto uses the CUDA engine when an NVIDIA driver is installed. Switching restarts the engine (loading page).
  The header chip shows what is running, e.g. `GPU · NVIDIA GeForce RTX 4070` or `CPU · AMD Ryzen 9 5950X`.
- Defaults: 1024×1024 on GPU, 512×512 on CPU; 4 steps, CFG 1.0 (klein 4B is step-distilled).
- `%LOCALAPPDATA%\SD-Studio\settings.ini` — optional overrides:

  ```ini
  [engine]
  mode=auto                 ; auto | cuda | cpu
  cuda_args=--backend all=cuda0,te=cpu   ; e.g. 8 GB GPUs: keep the text encoder on the CPU
  threads=0
  [defaults]
  width=1024
  height=1024
  [models]
  dir=D:\models             ; default: models\ next to the app, or two levels up
  ```
- Engine log: `%LOCALAPPDATA%\SD-Studio\logs\engine.log` (also shown on the error page if the engine stops).
- Needs the WebView2 runtime: built into Windows 11 and updated Windows 10. For an offline PC without it,
  extract the WebView2 "Fixed Version" runtime (x64) into `WebView2Runtime\` next to `SD-Studio.exe`.
- Loading/error pages follow the UI language and theme; the title bar follows the theme (Windows 11).

## User sign-in

SD-Studio starts the engine with sign-in enabled (`sd-server --auth-file %LOCALAPPDATA%\SD-Studio\users.json`).

- **First start:** a setup screen creates the administrator (only possible from the PC itself).
  The administrator adds, resets and deletes users on the **Account** tab; every user can change their own password.
- **Sessions:** HttpOnly, SameSite=Strict cookie, 12 h or 30 days with "Keep me signed in". Sessions survive engine
  restarts (e.g. CPU/GPU switch). A password change or admin reset signs out that user's other sessions.
- **Protection:** every API route (`/sdcpp`, `/v1`, `/sdapi`) needs a session; only the page and the sign-in endpoints
  are public. Cross-site writes are rejected, CORS is limited to the server's own origin, and 5 wrong passwords
  lock that account for 60 s (an admin reset lifts it). Users see only their own jobs; administrators see all.
- **Storage:** `users.json` keeps salted PBKDF2-HMAC-SHA256 hashes (600,000 iterations) and the session signing key,
  never passwords. It lives in the Windows user's profile, so other Windows users cannot read it.
- API clients: `POST /sdcpp/v1/auth/login` → use the `sdcpp_session` cookie or `Authorization: Bearer <token>`.
- Turn it off in `settings.ini`: `[auth]` `enabled=0`. The plain server keeps its old behaviour unless started with
  `--auth-file` (`python start_flux2_klein.py --auth-file users.json`).
- Forgot the only administrator password: close SD-Studio and delete `users.json` (all accounts are removed and the
  setup screen appears again).

## GPU (NVIDIA CUDA)

`dist\bin-cuda\` holds the CUDA build of `sd-cli.exe` / `sd-server.exe` with the CUDA runtime DLLs (~1 GB,
mostly cuBLAS). Native code for RTX 30 / 40 / 50 (sm_86, sm_89, sm_120a) and PTX for RTX 20 (sm_75, compiled
by the driver on first use). Needs an NVIDIA driver 575 or newer; nothing else to install.

- VRAM: the models need ~8.2 GB; 12 GB+ holds everything. 8 GB: `--backend all=cuda0,te=cpu`.
- `--sage-attn` instead of `--diffusion-fa` enables SageAttention on RTX 30 and newer.
- Without an NVIDIA driver the CUDA build still starts (nvcuda.dll is delay-loaded) and runs on the CPU.
- Built and checked on this machine (no GPU): it starts, falls back to CPU and produces images;
  the GPU path itself has to be tried on an NVIDIA PC.

## Run the server directly

```
python start_flux2_klein.py                  # starts sd-server and opens http://127.0.0.1:1234/
python start_flux2_klein.py --port 8080 --width 1024 --height 1024 --no-browser
```

Server defaults: 4 steps, CFG 1.0 (klein 4B is step-distilled), 512×512, flash attention, one thread per physical core.

## Test `dist\bin`

Simple (double-click or run from a command prompt):

```
test_sd_cli.bat                          # sd-cli.exe: one 512×512 image -> outputs\sd_cli_test.png
test_sd_cli.bat "a red fox in fresh snow"
test_sd_server.bat                       # sd-server.exe: starts the server, opens http://127.0.0.1:1234/
test_sd_server.bat nobrowser             # (close the window or Ctrl+C to stop)
```

Automated checks:

```
test_dist.bat          # ~1 min: files, --help/--version, server start, API and embedded WebUI
test_dist.bat full     # + a 256×256 image from the server and from sd-cli (several minutes each on CPU)
```

Uses its own port (12345) and stops its server afterwards; results and logs go to `outputs\test\`.
The exit code is the number of failed checks.

## Build

```
python build.py --sln              # Visual Studio 2019 solution, built with MSBuild (Release|x64): CPU engine + SD-Studio
python build.py --sln --no-build   # only (re)generate the solution
python build.py                    # same sources with the Ninja generator (faster configure)
python build.py --cuda             # NVIDIA CUDA engine (Ninja; ~30 min here) -> dist\bin-cuda
python build.py --cuda --sln --no-build   # VS 2019 solution with CUDA -> build-vs2019-cuda\stable-diffusion.sln
python build.py --frontend         # rebuild the WebUI first, then compile and embed it
python convert_to_gguf.py          # safetensors -> GGUF (add --force to redo)
python package_app.py              # portable SD-Studio folder with both engines and the models
```

Builds copy the runtime DLLs (MSVC, and CUDA for the GPU engine) next to the executables and stage them in
`dist\bin`, `dist\bin-cuda` and `dist\SD-Studio`. Once `toolchain\` exists everything works offline.
One-time downloads (online), all without admin rights:

```
python toolchain\get_msvc2019.py --accept-license   # VS 2019 compiler, MSBuild, Windows SDK
python toolchain\get_cuda.py --accept-eula          # CUDA 12.9 (nvcc, cudart, cuBLAS, VS integration)
python toolchain\get_webview2_sdk.py                # WebView2 SDK for SD-Studio
```

`toolchain\vs2019_env.py` also writes `VC\Auxiliary\Build\vcvarsall.bat`: nvcc refuses any MSVC without that
file ("Host compiler targets unsupported OS"), which portable toolsets do not have by default.
The CUDA solution is generated and its MSBuild CUDA integration works (CMake's CUDA check compiles through it),
but a full build of it was not run here: MSBuild compiles `.cu` files one at a time, so `build.py --cuda`
(Ninja, parallel) is the practical way to build the GPU engine.

### The solution

`stable-diffusion.cpp\build-vs2019\stable-diffusion.sln` — CMake-generated (`Visual Studio 16 2019`, `v142`, `x64`),
projects `sd-studio` (desktop app, `examples\desktop`), `sd-cli`, `sd-server`, `stable-diffusion`, `ggml`,
`ggml-cpu`, `webp`, `webm`, … . It targets the
portable VS 2019 instance in `toolchain\msvc2019` (MSBuild 16.11 + VC++ v142 targets, no admin install,
no registry changes). `build.py` sets the environment that points MSBuild at the portable Windows SDK;
it is needed because this machine also has a registered SDK 10.0.26100.

To open it in a regular Visual Studio 2019 IDE: the projects pin `v142` and SDK `10.0.19041.0`, and the
CMake re-run step uses absolute paths to `toolchain\cmake`. On another machine, regenerate with
`python build.py --sln --no-build` there, or run CMake with `-G "Visual Studio 16 2019" -A x64`.
Decline any retarget prompt (e.g. from VS 2022 to v143).

## Layout

| Path | Contents |
|---|---|
| `dist\SD-Studio\` | `SD-Studio.exe` desktop app + `WebView2Loader.dll` + MSVC runtime DLLs |
| `dist\SD-Studio-portable\` | standalone package from `package_app.py` (app, `engine\cpu`, `engine\cuda`, `models\`) |
| `dist\bin\` | CPU engine: `sd-server.exe`, `sd-cli.exe` + MSVC runtime DLLs (no VC++ redistributable needed) |
| `dist\bin-cuda\` | CUDA engine: same + `cudart64_12.dll`, `cublas64_12.dll`, `cublasLt64_12.dll` |
| `models\diffusion_models\flux-2-klein-4b-Q8_0.gguf` | diffusion model, Q8_0 (4.17 GB) |
| `models\text_encoders\qwen_3_4b-Q8_0.gguf` | Qwen3-4B text encoder, Q8_0 (4.27 GB) |
| `models\vae\flux2-vae-F16.gguf` | FLUX.2 VAE, F16 (168 MB) |
| `models\download\` | original safetensors from `Comfy-Org/flux2-klein-4B` (Apache-2.0); can be deleted |
| `toolchain\` | portable VS 2019 instance (MSVC, MSBuild, SDK), CUDA 12.9, WebView2 SDK, CMake 3.31, Ninja, pnpm |
| `stable-diffusion.cpp\` | source; WebUI in `examples\server\frontend`, desktop app in `examples\desktop`; build dirs `build\`, `build-cuda\` (Ninja), `build-vs2019\` (.sln) |

## WebUI

- Dark theme by default: **Dracula**, switchable to One Dark Pro, GitHub Dark, Tokyo Night,
  Catppuccin Mocha, Nord, Monokai and VS Code Dark Modern (header picker or Settings tab).
- 12 languages, auto-detected from the browser and switchable in the header: English, 日本語,
  简体中文, 繁體中文, Español, Français, Deutsch, Italiano, Português (Brasil), Русский, Tiếng Việt.
  Dictionaries: `frontend\src\i18n\locales\*.ts` (`en.ts` is the source of truth).
- Locale-appropriate Windows fonts for Korean/Japanese/Chinese; no web fonts or CDNs.
- Embedded in `dist\bin\sd-server.exe` and served at `http://127.0.0.1:1234/`. Screenshots: `outputs\ui_screenshots\`.
- To change it: edit `frontend\src\`, then `python build_frontend.py --offline` (packages are already
  in `node_modules`) and `python build.py --sln` to re-embed. Stop a running server first (the exe is locked).

## Source changes

- `src\model_loader.cpp` (`tensor_should_be_converted`): integer tensors are no longer converted.
  Without it, `sd-cli -M convert --type f16` aborts on the FLUX.2 VAE's `bn.num_batches_tracked` (I64).
- `examples\desktop\` (new) and one guarded `add_subdirectory(desktop)` in `examples\CMakeLists.txt`
  (only when `SD_WEBVIEW2_DIR` is set).
- WebUI (`examples\server\frontend\src`): themes, i18n, and `lib\desktop.ts` (engine picker; inert in a browser).
- Sign-in: `examples\server\auth.h/.cpp` (new: accounts, PBKDF2, signed sessions, auth endpoints), `--auth-file` option
  and pre-routing check in `main.cpp` / `runtime.*`, job owner in `async_jobs.h` / `routes_sdcpp.cpp`;
  WebUI `lib\auth.ts`, `components\AuthScreen.vue`, `components\AccountPanel.vue`.

## Performance note

On this VM a 512×512 image takes ~10 min (text encode ~90 s, 4 steps ~480 s, VAE decode ~50 s).
An FMA micro-benchmark shows the VM delivers only ~2.5 cores of real throughput (230 GFLOPS at 16 threads
vs ~100 GFLOPS single-thread); a dedicated 16-core machine should be roughly 6–8× faster.
