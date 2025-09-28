#!/bin/bash

# Quick test runner for Multi Threads project
# Usage: ./run_tests.sh [verbose|parallel|memcheck]

set -e

PROJECT_ROOT=$(pwd)
BUILD_DIR="build"

# Parse arguments
VERBOSE=false
PARALLEL=false
MEMCHECK=false

for arg in "$@"; do
    case $arg in
        verbose|-v)
            VERBOSE=true
            ;;
        parallel|-p)
            PARALLEL=true
            ;;
        memcheck|-m)
            MEMCHECK=true
            ;;
        *)
            echo "Unknown argument: $arg"
            echo "Usage: $0 [verbose|parallel|memcheck]"
            exit 1
            ;;
    esac
done

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Build directory not found. Please run ./build.sh first."
    exit 1
fi

cd $BUILD_DIR

# Prepare CTest arguments
CTEST_ARGS=""

if [ "$VERBOSE" = true ]; then
    CTEST_ARGS="$CTEST_ARGS --verbose"
fi

if [ "$PARALLEL" = true ]; then
    # Use sysctl -n hw.ncpu for macOS, nproc for Linux
    if command -v nproc >/dev/null 2>&1; then
        CTEST_ARGS="$CTEST_ARGS --parallel $(nproc)"
    else
        CTEST_ARGS="$CTEST_ARGS --parallel $(sysctl -n hw.ncpu)"
    fi
fi

if [ "$MEMCHECK" = true ]; then
    if command -v valgrind >/dev/null 2>&1; then
        echo "Running memory check with valgrind..."
        CTEST_ARGS="$CTEST_ARGS -T memcheck"
    else
        echo "Warning: valgrind not found. Memory check disabled."
    fi
fi

# Run tests
echo "Running tests with CTest..."
echo "Arguments: $CTEST_ARGS"

if [ -n "$CTEST_ARGS" ]; then
    ctest $CTEST_ARGS
else
    ctest
fi

echo "Tests completed successfully!"

# Show test results summary
echo ""
echo "Test Results Summary:"
echo "===================="
ctest --quiet

# Show available test targets
echo ""
echo "Available test labels:"
echo "====================="
ctest --print-labels