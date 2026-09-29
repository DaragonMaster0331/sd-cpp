@echo off
rem ---------------------------------------------------------------------------
rem  Tests the executables in dist\bin (sd-cli.exe, sd-server.exe + runtime DLLs).
rem
rem    test_dist.bat          quick test, about 1 minute: files, --help/--version,
rem                           server start, API and embedded WebUI
rem    test_dist.bat full     also generates real images with sd-cli and with the
rem                           server (256x256; several minutes each on a CPU)
rem
rem  Results and logs go to outputs\test\. Exit code = number of failed checks.
rem ---------------------------------------------------------------------------
setlocal EnableExtensions EnableDelayedExpansion
cd /d "%~dp0"

set "BIN=%~dp0dist\bin"
set "DIFF=%~dp0models\diffusion_models\flux-2-klein-4b-Q8_0.gguf"
set "LLM=%~dp0models\text_encoders\qwen_3_4b-Q8_0.gguf"
set "VAE=%~dp0models\vae\flux2-vae-F16.gguf"
set "OUT=%~dp0outputs\test"
set "PORT=12345"
set "URL=http://127.0.0.1:%PORT%"
set "TITLE=sd-server-dist-test"
set /a PASS=0, FAIL=0

if not exist "%OUT%" mkdir "%OUT%"
where curl >nul 2>&1 || (echo curl.exe not found ^(it ships with Windows 10 and later^). & exit /b 1)

echo.
echo === [1] Files in dist\bin and models ===
for %%F in (sd-cli.exe sd-server.exe VCRUNTIME140.dll VCRUNTIME140_1.dll MSVCP140.dll MSVCP140_CODECVT_IDS.dll VCOMP140.DLL) do (
    if exist "%BIN%\%%F" (call :pass "%%F") else (call :fail "%%F is missing")
)
for %%F in ("%DIFF%" "%LLM%" "%VAE%") do (
    if exist "%%~F" (call :pass "%%~nxF") else (call :fail "%%~nxF is missing - run: python convert_to_gguf.py")
)

echo.
echo === [2] sd-cli.exe ===
"%BIN%\sd-cli.exe" --version > "%OUT%\sd-cli_version.txt" 2>&1
findstr /c:"stable-diffusion.cpp version" "%OUT%\sd-cli_version.txt" >nul && (call :pass "sd-cli --version") || call :fail "sd-cli --version (see outputs\test\sd-cli_version.txt)"
"%BIN%\sd-cli.exe" --help > "%OUT%\sd-cli_help.txt" 2>&1
findstr /c:"--diffusion-model" "%OUT%\sd-cli_help.txt" >nul && (call :pass "sd-cli --help") || call :fail "sd-cli --help"

echo.
echo === [3] sd-server.exe ===
"%BIN%\sd-server.exe" --help > "%OUT%\sd-server_help.txt" 2>&1
findstr /c:"--listen-port" "%OUT%\sd-server_help.txt" >nul && (call :pass "sd-server --help") || call :fail "sd-server --help"

echo   starting sd-server on port %PORT% (log: outputs\test\sd-server.log) ...
start "%TITLE%" /min cmd /c ""%BIN%\sd-server.exe" --diffusion-model "%DIFF%" --llm "%LLM%" --vae "%VAE%" --cfg-scale 1.0 --steps 4 -W 256 -H 256 --diffusion-fa --listen-ip 127.0.0.1 --listen-port %PORT% > "%OUT%\sd-server.log" 2>&1"

set /a WAITED=0
:wait_server
curl -s -m 2 -o nul "%URL%/sdcpp/v1/capabilities" && goto server_up
set /a WAITED+=2
if %WAITED% geq 180 goto server_timeout
ping -n 3 127.0.0.1 >nul
goto wait_server

:server_timeout
call :fail "sd-server did not answer within 180 s (see outputs\test\sd-server.log)"
goto stop_server

:server_up
call :pass "sd-server is up after about %WAITED% s"
curl -s -m 10 "%URL%/sdcpp/v1/capabilities" > "%OUT%\capabilities.json"
findstr /c:"flux-2-klein" "%OUT%\capabilities.json" >nul && (call :pass "capabilities report the FLUX.2 klein model") || call :fail "capabilities (see outputs\test\capabilities.json)"
curl -s -m 10 "%URL%/" > "%OUT%\webui.html"
findstr /c:"sdcpp-webui-theme" "%OUT%\webui.html" >nul && (call :pass "embedded WebUI is served at /") || call :fail "WebUI not embedded - run: python build_frontend.py, then python build.py --sln"
curl -s -m 10 "%URL%/v1/models" | findstr /c:"sd-cpp-local" >nul && (call :pass "OpenAI-compatible /v1/models") || call :fail "/v1/models"

if /i not "%~1"=="full" goto stop_server

echo.
echo === [4] Image generation through the server (256x256, 4 steps) ===
echo   this takes several minutes on a CPU ...
> "%OUT%\request.json" echo {"prompt":"a red fox in fresh snow, golden hour, photorealistic","size":"256x256","n":1,"seed":7}
curl -s -m 3600 -H "Content-Type: application/json" -d @"%OUT%\request.json" "%URL%/v1/images/generations" > "%OUT%\server_generation.json"
findstr /c:"b64_json" "%OUT%\server_generation.json" >nul && (call :pass "server generated an image (outputs\test\server_generation.json)") || call :fail "server generation (see outputs\test\server_generation.json and sd-server.log)"

:stop_server
taskkill /fi "WINDOWTITLE eq %TITLE%*" /t /f >nul 2>&1
echo   sd-server stopped

if /i not "%~1"=="full" goto summary

echo.
echo === [5] Image generation with sd-cli (256x256, 4 steps) ===
echo   this takes several minutes on a CPU ...
if exist "%OUT%\sd-cli_test.png" del "%OUT%\sd-cli_test.png"
"%BIN%\sd-cli.exe" --diffusion-model "%DIFF%" --llm "%LLM%" --vae "%VAE%" --cfg-scale 1.0 --steps 4 -W 256 -H 256 --diffusion-fa -s 42 -p "a lovely cat sitting on a windowsill at sunset, photorealistic" -o "%OUT%\sd-cli_test.png" -v > "%OUT%\sd-cli_generation.log" 2>&1
if exist "%OUT%\sd-cli_test.png" (call :pass "sd-cli wrote outputs\test\sd-cli_test.png") else (call :fail "sd-cli generation (see outputs\test\sd-cli_generation.log)")
findstr /c:"AVX2 = 1" "%OUT%\sd-cli_generation.log" >nul && (call :pass "CPU backend uses AVX2") || call :fail "AVX2 not active (see outputs\test\sd-cli_generation.log)"

:summary
echo.
echo ===========================================
echo   Passed: %PASS%   Failed: %FAIL%
echo ===========================================
if /i not "%~1"=="full" echo   Run "test_dist.bat full" to also test real image generation.
endlocal & exit /b %FAIL%

:pass
set /a PASS+=1
echo   [PASS] %~1
exit /b 0

:fail
set /a FAIL+=1
echo   [FAIL] %~1
exit /b 0
