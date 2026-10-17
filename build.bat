@echo off
REM ============================================================================
REM  build.bat  —  One-click build script for AcademiCLI on Windows
REM  Requires g++ (MinGW-w64) on PATH
REM ============================================================================

echo.
echo  ============================================================
echo   AcademiCLI  -  Academic Management System  Build Script
echo  ============================================================
echo.

REM Check for g++
where g++ >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo  [ERROR] g++ not found on PATH.
    echo          Please install MinGW-w64 and add its bin/ folder to PATH.
    pause
    exit /b 1
)

REM Create output directories
if not exist build\obj   mkdir build\obj
if not exist build\bin   mkdir build\bin

echo  Compiling Student.cpp...
g++ -std=c++17 -Wall -O2 -Iinclude -c src\Student.cpp       -o build\obj\Student.o
if %ERRORLEVEL% neq 0 goto :error

echo  Compiling FileManager.cpp...
g++ -std=c++17 -Wall -O2 -Iinclude -c src\FileManager.cpp   -o build\obj\FileManager.o
if %ERRORLEVEL% neq 0 goto :error

echo  Compiling MenuController.cpp...
g++ -std=c++17 -Wall -O2 -Iinclude -c src\MenuController.cpp -o build\obj\MenuController.o
if %ERRORLEVEL% neq 0 goto :error

echo  Compiling main.cpp...
g++ -std=c++17 -Wall -O2 -Iinclude -c src\main.cpp          -o build\obj\main.o
if %ERRORLEVEL% neq 0 goto :error

echo  Linking...
g++ -std=c++17 -o build\bin\AcademiCLI.exe ^
    build\obj\Student.o ^
    build\obj\FileManager.o ^
    build\obj\MenuController.o ^
    build\obj\main.o
if %ERRORLEVEL% neq 0 goto :error

echo.
echo  [SUCCESS] Build complete!
echo  Executable → build\bin\AcademiCLI.exe
echo.

set /p LAUNCH="  Launch now? (y/N): "
if /i "%LAUNCH%"=="y" (
    build\bin\AcademiCLI.exe
)
goto :end

:error
echo.
echo  [ERROR] Build failed. Check the error messages above.
pause
exit /b 1

:end
pause
