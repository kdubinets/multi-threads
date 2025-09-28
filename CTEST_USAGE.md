# CTest Configuration Documentation

## Overview

This project includes comprehensive CTest configuration for automated testing of the multi-threading library. The configuration supports various testing scenarios including unit tests, memory checking, and continuous integration.

## Files Created

### Core Configuration Files
- `CTestConfig.cmake` - Main CTest configuration with project settings
- `CTestCustom.cmake.in` - Template for custom CTest behavior
- `run_tests.cmake` - Automated CTest script for CI/CD
- `run_tests.sh` - Convenient shell script for running tests

## Usage

### Basic Testing

#### Using the build script (recommended for development):
```bash
# Build and run tests
./build.sh test

# Clean build and run tests
./build.sh clean test
```

#### Using CTest directly:
```bash
cd build
ctest                    # Run all tests
ctest --verbose         # Run with detailed output
ctest --parallel        # Run tests in parallel
```

### Advanced Testing Options

#### Using the custom test runner:
```bash
./run_tests.sh                    # Basic test run
./run_tests.sh verbose           # Verbose output
./run_tests.sh parallel          # Parallel execution
./run_tests.sh memcheck          # Memory checking with valgrind
```

#### Manual CTest commands:
```bash
cd build

# Run specific tests by name
ctest -R TasLock

# Run tests with specific labels
ctest -L unit

# Run tests with timeout
ctest --timeout 120

# Generate test results in XML format
ctest --output-junit results.xml

# Show available test labels
ctest --print-labels
```

### Continuous Integration

#### Using the CTest script:
```bash
# Run full CI pipeline
ctest -S run_tests.cmake

# Run with specific configuration
ctest -S run_tests.cmake -DCTEST_BUILD_CONFIGURATION=Release
```

### Memory Checking

If valgrind is installed:
```bash
cd build
ctest -T memcheck           # Run memory checks
./run_tests.sh memcheck     # Use convenience script
```

### Coverage Analysis

If gcov is available:
```bash
cd build
# Build with coverage flags
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="--coverage" ..
make
ctest -T coverage
```

## Configuration Details

### Test Properties
- **Timeout**: Individual tests timeout after 60 seconds
- **Labels**: Tests are labeled as "unit", "threading", "locks"
- **Discovery**: GoogleTest integration with automatic test discovery
- **Parallel**: Tests can run in parallel for faster execution

### Customization Options

#### Timeout Settings
- Global timeout: 300 seconds (5 minutes) per test
- Individual test timeout: 60 seconds
- Can be overridden via CTest properties

#### Memory Checking
- Configured for valgrind with leak checking
- Custom suppressions can be added to `CTestCustom.cmake.in`

#### Coverage Exclusions
- External libraries excluded from coverage
- Test files excluded from coverage
- System headers excluded from coverage

### Integration with Build System

The CTest configuration is fully integrated with the CMake build system:
- `enable_testing()` and `include(CTest)` in main CMakeLists.txt
- GoogleTest integration with `gtest_discover_tests()`
- Automatic generation of `CTestCustom.cmake` from template

## Troubleshooting

### Common Issues

1. **Tests not found**: Ensure GoogleTest is properly installed and linked
2. **Timeout errors**: Increase timeout values in test configuration
3. **Memory check failures**: Install valgrind or disable memory checking
4. **Parallel execution issues**: Reduce parallel level or run tests sequentially

### Debug Information
```bash
# Show CTest configuration
ctest --debug

# Show detailed test information
ctest --extra-verbose

# Show CTest variables
cmake -LA | grep CTEST
```

## Best Practices

1. **Run tests frequently** during development using `./build.sh test`
2. **Use parallel execution** for faster feedback: `./run_tests.sh parallel`
3. **Memory check regularly** to catch leaks early: `./run_tests.sh memcheck`
4. **Label tests appropriately** for selective running
5. **Set reasonable timeouts** for long-running tests
6. **Use CI script** for automated environments: `ctest -S run_tests.cmake`

## Extension

To add new tests:
1. Add test source files to `tests/CMakeLists.txt`
2. Use GoogleTest framework for consistency
3. Apply appropriate labels for categorization
4. Consider timeout requirements for long-running tests

The CTest configuration will automatically discover and include new GoogleTest-based tests.