#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd)"
PROJECT_DIR="${SCRIPT_DIR}/Orbita"
OUTPUT_NAME="orbit"

cd "${PROJECT_DIR}"

SOURCES=(
  main.cc
  draw.cc
  Planet.cc
  Point.cc
  Vector.cc
)

# Detect compiler
COMPILER=""
if command -v g++ &> /dev/null; then
    COMPILER="g++"
elif command -v clang++ &> /dev/null; then
    COMPILER="clang++"
elif [ -f "/usr/bin/g++" ]; then
    COMPILER="/usr/bin/g++"
elif [ -f "/mingw64/bin/g++.exe" ]; then
    COMPILER="/mingw64/bin/g++.exe"
elif [ -f "/ucrt64/bin/g++.exe" ]; then
    COMPILER="/ucrt64/bin/g++.exe"
else
    echo "Error: No C++ compiler found!" >&2
    echo "" >&2
    echo "Please install a C++ compiler:" >&2
    echo "  - On Windows: Install MSYS2 (https://www.msys2.org/) or Visual Studio" >&2
    echo "  - On Linux: sudo apt install build-essential freeglut3-dev" >&2
    echo "  - On macOS: xcode-select --install" >&2
    exit 1
fi

echo "Using compiler: ${COMPILER}"

# Detect platform and set libraries
PLATFORM="$(uname -s)"
case "${PLATFORM}" in
  Darwin)
    LIBS=("-framework" "OpenGL" "-framework" "GLUT")
    ;;
  Linux)
    LIBS=("-lglut" "-lGLU" "-lGL")
    ;;
  MINGW*|MSYS*|CYGWIN*)
    LIBS=("-lfreeglut" "-lglu32" "-lopengl32")
    ;;
  *)
    echo "Warning: Unsupported platform: ${PLATFORM}" >&2
    echo "Attempting with default OpenGL libraries..." >&2
    LIBS=("-lglut" "-lGLU" "-lGL")
    ;;
esac

echo "Compiling Orbit simulation..."
"${COMPILER}" -std=c++11 -Wall -Wextra -O2 "${SOURCES[@]}" "${LIBS[@]}" -o "${OUTPUT_NAME}"

if [ ! -f "${OUTPUT_NAME}" ] && [ ! -f "${OUTPUT_NAME}.exe" ]; then
    echo "Error: Compilation failed!" >&2
    exit 1
fi

EXECUTABLE="${OUTPUT_NAME}"
if [ -f "${OUTPUT_NAME}.exe" ]; then
    EXECUTABLE="${OUTPUT_NAME}.exe"
fi

echo "Build complete. Launching ${EXECUTABLE}..."
"./${EXECUTABLE}"

