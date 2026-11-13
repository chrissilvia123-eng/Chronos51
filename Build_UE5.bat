@echo off
REM Chronos 51 — UE5 Build Script
REM Run from: E:\Chronos51\05_UE5_Project\Chronos51\

set PROJECT_PATH=E:\Chronos51\05_UE5_Project\Chronos51\Chronos51.uproject
set UE5_PATH=C:\Program Files\Epic Games\UE_5.3
set ENGINE_BIN=%UE5_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
set ENGINE_BUILD_BAT=%UE5_PATH%\Engine\Build\BatchFiles\Build.bat

echo ==========================================
echo Chronos 51 — UE5 Build Script
echo ==========================================
echo Project: %PROJECT_PATH%
echo Engine:  %UE5_PATH%
echo ==========================================

REM Check UE5 installation
if not exist "%ENGINE_BUILD_BAT%" (
    echo ERROR: UE5 not found at %UE5_PATH%
    echo Please install UE5.3+ or update UE5_PATH in this script
    pause
    exit /b 1
)

echo.
echo [1/4] Generating Visual Studio Solution...
"%UE5_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="%PROJECT_PATH%" -game -engine -progress
if errorlevel 1 (
    echo ERROR: Failed to generate project files
    pause
    exit /b 1
)

echo.
echo [2/4] Building Development Editor (Win64)...
"%ENGINE_BUILD_BAT%" Chronos51Editor Win64 Development -Project="%PROJECT_PATH%" -WaitMutex -FromMsBuild
if errorlevel 1 (
    echo ERROR: Build failed
    pause
    exit /b 1
)

echo.
echo [3/4] Building Development Client (Win64)...
"%ENGINE_BUILD_BAT%" Chronos51 Win64 Development -Project="%PROJECT_PATH%" -WaitMutex -FromMsBuild
if errorlevel 1 (
    echo ERROR: Client build failed
    pause
    exit /b 1
)

echo.
echo [4/4] Verifying Build Output...
set EDITOR_EXE=%UE5_PATH%\Engine\Binaries\Win64\UnrealEditor.exe
set GAME_EXE=E:\Chronos51\05_UE5_Project\Chronos51\Binaries\Win64\Chronos51.exe

if exist "%EDITOR_EXE%" (
    echo OK: UnrealEditor.exe found
) else (
    echo WARNING: UnrealEditor.exe not found at expected location
)

if exist "%GAME_EXE%" (
    echo OK: Chronos51.exe built
) else (
    echo WARNING: Chronos51.exe not found (may need client target)
)

echo.
echo ==========================================
echo BUILD COMPLETE
echo ==========================================
echo.
echo To open in UE5 Editor:
echo   "%EDITOR_EXE%" "%PROJECT_PATH%"
echo.
echo To run PIE from command line:
echo   "%EDITOR_EXE%" "%PROJECT_PATH%" -game -log
echo.
pause