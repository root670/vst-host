::
:: Create Visual Studio solution and project files using CMake.
::
@echo off
setlocal

echo ========================
echo Creating 32-bit solution
echo ========================
rd /s /q build32
md build32
cmake -S . -B build32 -A Win32
if %ERRORLEVEL% neq 0 (exit /b 1)

echo ========================
echo Creating 64-bit solution
echo ========================
rd /s /q build64
md build64
cmake -S . -B build64 -A x64
if %ERRORLEVEL% neq 0 (exit /b 1)

endlocal