@echo off
rem Test sd-server.exe: start the FLUX.2 klein 4B server and open the WebUI in the browser.
rem Close this window or press Ctrl+C to stop the server.
rem   test_sd_server.bat
rem   test_sd_server.bat nobrowser
cd /d "%~dp0"

echo Starting sd-server at http://127.0.0.1:1234/ ...
if /i not "%~1"=="nobrowser" start "" /min cmd /c "ping -n 6 127.0.0.1 >nul & start http://127.0.0.1:1234/"

dist\bin\sd-server.exe ^
  --diffusion-model models\diffusion_models\flux-2-klein-4b-Q8_0.gguf ^
  --llm models\text_encoders\qwen_3_4b-Q8_0.gguf ^
  --vae models\vae\flux2-vae-F16.gguf ^
  --cfg-scale 1.0 --steps 4 -W 512 -H 512 --diffusion-fa ^
  --listen-ip 127.0.0.1 --listen-port 1234

echo.
echo sd-server.exe stopped (exit code %errorlevel%)
pause
