@echo off
setlocal enabledelayedexpansion

set "PROJECT_DIR=%~dp0Orbit"
set "OUTPUT_NAME=orbit.exe"

cd /d "%PROJECT_DIR%"

set "SOURCES=main.cc draw.cc Planet.cc Point.cc Vector.cc"

REM Try to find a C++ compiler
set "COMPILER="

REM Try g++ first (MSYS2/MinGW)
where g++ >nul 2>&1
if %ERRORLEVEL% == 0 (
    set "COMPILER=g++"
    set "LIBS=-lfreeglut -lglu32 -lopengl32"
    goto :compile
)

REM Try cl (Visual Studio compiler)
where cl >nul 2>&1
if %ERRORLEVEL% == 0 (
    set "COMPILER=cl"
    set "LIBS=freeglut.lib glu32.lib opengl32.lib"
    goto :compile_vs
)

REM Try MSYS2 paths
if exist "C:\msys64\ucrt64\bin\g++.exe" (
    set "COMPILER=C:\msys64\ucrt64\bin\g++.exe"
    set "LIBS=-lfreeglut -lglu32 -lopengl32"
    goto :compile
)

if exist "C:\msys64\mingw64\bin\g++.exe" (
    set "COMPILER=C:\msys64\mingw64\bin\g++.exe"
    set "LIBS=-lfreeglut -lglu32 -lopengl32"
    goto :compile
)

echo Error: No C++ compiler found!
echo.
echo Please install one of the following:
echo   1. MSYS2: https://www.msys2.org/
echo      Then install: pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-freeglut
echo   2. Visual Studio with C++ development tools
echo   3. WSL (Windows Subsystem for Linux)
echo.
pause
exit /b 1

:compile
echo Compiling Orbit simulation with %COMPILER%...
%COMPILER% -std=c++11 -Wall -Wextra -O2 %SOURCES% %LIBS% -o %OUTPUT_NAME%
if %ERRORLEVEL% neq 0 (
    echo Compilation failed!
    pause
    exit /b 1
)
goto :run

:compile_vs
echo Compiling Orbit simulation with Visual Studio compiler...
REM Visual Studio requires setting up the environment first
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
if %ERRORLEVEL% neq 0 (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
)
if %ERRORLEVEL% neq 0 (
    echo Could not find Visual Studio environment. Please run from Developer Command Prompt.
    pause
    exit /b 1
)
%COMPILER% /EHsc /std:c++11 %SOURCES% %LIBS% /Fe:%OUTPUT_NAME%
if %ERRORLEVEL% neq 0 (
    echo Compilation failed!
    pause
    exit /b 1
)
goto :run

:run
if not exist "%OUTPUT_NAME%" (
    echo Error: Executable not found!
    pause
    exit /b 1
)

echo Build complete. Launching %OUTPUT_NAME%...
"%OUTPUT_NAME%"
pause

