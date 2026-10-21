@echo off
REM ============================================================================
REM  build.bat  —  One-click build script for AcademiCLI on Windows
REM  Supports MSVC (Visual Studio / Build Tools) and MinGW g++
REM ============================================================================

echo.
echo  ============================================================
echo   AcademiCLI  -  Academic Management System  Build Script
echo  ============================================================
echo.

if not exist build\bin mkdir build\bin
if not exist build\obj mkdir build\obj

REM Check if cl is available directly
where cl >nul 2>&1
if %ERRORLEVEL% equ 0 goto :build_msvc

REM Check for Visual Studio Build Tools / IDE vcvars64.bat
if exist "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
    goto :build_msvc
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    goto :build_msvc
)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
    goto :build_msvc
)

REM Fallback to g++
where g++ >nul 2>&1
if %ERRORLEVEL% equ 0 goto :build_gcc

echo  [ERROR] Neither MSVC (cl.exe) nor MinGW (g++.exe) was found on PATH.
echo          Please ensure Visual Studio Build Tools or MinGW is installed.
pause
exit /b 1

:build_msvc
echo  Building with MSVC compiler (cl.exe)...
cl /std:c++17 /EHsc /utf-8 /O2 /Iinclude src\*.cpp /Fe:build\bin\AcademiCLI.exe /Fo:build\obj\
if %ERRORLEVEL% neq 0 goto :error
goto :success

:build_gcc
echo  Building with MinGW GCC (g++)...
g++ -std=c++17 -Wall -O2 -Iinclude -c src\Student.cpp       -o build\obj\Student.o
g++ -std=c++17 -Wall -O2 -Iinclude -c src\FileManager.cpp   -o build\obj\FileManager.o
g++ -std=c++17 -Wall -O2 -Iinclude -c src\MenuController.cpp -o build\obj\MenuController.o
g++ -std=c++17 -Wall -O2 -Iinclude -c src\main.cpp          -o build\obj\main.o
g++ -std=c++17 -o build\bin\AcademiCLI.exe build\obj\Student.o build\obj\FileManager.o build\obj\MenuController.o build\obj\main.o
if %ERRORLEVEL% neq 0 goto :error
goto :success

:success
echo.
echo  [SUCCESS] Build complete!
echo  Executable -^> build\bin\AcademiCLI.exe
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
