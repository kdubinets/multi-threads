# Multi-Threads

A research project exploring multi-threading synchronization primitives and lock-free data structures based on "The Art of Multiprocessor Programming". This project implements various locking mechanisms and provides performance benchmarking tools.

## Features

- **TAS Lock** (Test-And-Set): Basic spin-lock implementation
- **TTAS Lock** (Test-Test-And-Set): Optimized spin-lock with reduced contention
- **Performance Benchmarking**: Compare lock implementations against std::mutex and no-lock baseline
- **Testing**: Unit tests with GTest integration

## Requirements

- **Compiler**: Clang (C++20 support required)
- **Build System**: CMake 3.14+
- **Testing**: Google Test framework
- **OS**: macOS (tested) / Linux (untested) /Windows (untested)

## Building

### Quick Start
```bash
# Build the project
./build.sh

# Build and run tests
./build.sh test

# Release build
./build.sh release

# Clean build
./build.sh clean
```

### Manual Build
```bash
mkdir build && cd build
CC=clang CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(sysctl -n hw.ncpu)  # macOS
```

## Running

### Unit Tests
```bash
# Using build script
./build.sh test

# Using CTest directly
cd build && ctest --verbose
```

### Performance Benchmarks
```bash
cd build
./perf/perf_driver <benchmark_name> <threads> <iterations> [test_runs]
```

**Available benchmarks:**
- `tas_lock` - Test-And-Set lock
- `ttas_lock` - Test-Test-And-Set lock  
- `mutex` - Standard mutex
- `no_lock` - Baseline (no synchronization)

**Example:**
```bash
# Compare 4-thread performance with 1M iterations
./perf/perf_driver tas_lock 4 1000000 10
./perf/perf_driver ttas_lock 4 1000000 10
./perf/perf_driver mutex 4 1000000 10
```

### Batch Performance Testing
```bash
# Compare all locks across different thread counts
for threads in 1 2 4 8 16 32; do
    for test in tas_lock ttas_lock mutex no_lock; do
        ./build/perf/perf_driver $test $threads 1000000 30
    done
done
```

## Project Structure

```
├── include/           # Public headers
├── src/              # Source implementations  
├── tests/            # Unit tests
├── perf/             # Performance benchmarking
├── build/            # Build artifacts
└── build.sh          # Build script
```