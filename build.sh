#!/bin/bash

# Build script for Multi Threads project
# Usage: ./build.sh [clean|test|release]

set -e

PROJECT_ROOT=$(pwd)
BUILD_DIR="build"

# Parse arguments
BUILD_TYPE="Debug"
CLEAN=false
RUN_TESTS=false

for arg in "$@"; do
    case $arg in
        clean)
            CLEAN=true
            ;;
        test)
            RUN_TESTS=true
            ;;
        release)
            BUILD_TYPE="Release"
            ;;
        *)
            echo "Unknown argument: $arg"
            echo "Usage: $0 [clean|test|release]"
            exit 1
            ;;
    esac
done

# Clean build directory if requested
if [ "$CLEAN" = true ]; then
    echo "Cleaning build directory..."
    rm -rf $BUILD_DIR
fi

# Create and enter build directory
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Configure with CMake
echo "Configuring project with CMake..."
CC=clang CXX=clang++ cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..

# Build the project
echo "Building project..."
# Use sysctl -n hw.ncpu for macOS, nproc for Linux
if command -v nproc >/dev/null 2>&1; then
    make -j$(nproc)
else
    make -j$(sysctl -n hw.ncpu)
fi

# Copy compile_commands.json to root
echo "Copying compile_commands.json to root directory..."
cp compile_commands.json ..

# Run tests if requested
if [ "$RUN_TESTS" = true ]; then
    echo "Running tests..."
    ctest
fi

echo "Build completed successfully!"
echo "Executables:"
echo "  Library: $BUILD_DIR/src/libmulti_threads.so"
echo "  Tests: $BUILD_DIR/tests/multi_threads_tests"
echo "  Performance driver: $BUILD_DIR/perf/perf_driver"