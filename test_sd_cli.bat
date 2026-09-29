@echo off
rem Test sd-cli.exe: generate one 512x512 image with FLUX.2 klein 4B.
rem   test_sd_cli.bat
rem   test_sd_cli.bat "a red fox in fresh snow"
cd /d "%~dp0"

set "PROMPT=%~1"
if "%PROMPT%"=="" set "PROMPT=a lovely cat sitting on a windowsill at sunset, photorealistic"
if not exist outputs mkdir outputs

echo Prompt: %PROMPT%
echo Generating (this takes a few minutes on a CPU) ...
dist\bin\sd-cli.exe ^
  --diffusion-model models\diffusion_models\flux-2-klein-4b-Q8_0.gguf ^
  --llm models\text_encoders\qwen_3_4b-Q8_0.gguf ^
  --vae models\vae\flux2-vae-F16.gguf ^
  --cfg-scale 1.0 --steps 4 -W 512 -H 512 --diffusion-fa -s 42 ^
  -p "%PROMPT%" ^
  -o outputs\sd_cli_test.png

if errorlevel 1 (
  echo.
  echo sd-cli.exe FAILED
) else (
  echo.
  echo OK - image saved to outputs\sd_cli_test.png
)
pause
