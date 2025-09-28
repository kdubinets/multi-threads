# CTest configuration for Multi Threads project
# This file configures CTest behavior for testing and continuous integration

# Project information
set(CTEST_PROJECT_NAME "multi_threads")
set(CTEST_NIGHTLY_START_TIME "01:00:00 UTC")

# Dashboard submission (optional - can be used with CDash)
# set(CTEST_DROP_METHOD "http")
# set(CTEST_DROP_SITE "my.cdash.org")
# set(CTEST_DROP_LOCATION "/submit.php?project=multi_threads")
# set(CTEST_DROP_SITE_CDASH TRUE)

# Test timeout settings
set(CTEST_TEST_TIMEOUT 300)  # 5 minutes per test

# Memory checking configuration
set(CTEST_MEMORYCHECK_COMMAND_OPTIONS "--tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=50 --track-fds=yes")

# Coverage options
set(CTEST_COVERAGE_COMMAND "gcov")

# Custom test labels
set_property(TEST PROPERTY LABELS "unit")

# Parallel testing
set(CTEST_BUILD_FLAGS "-j")
set(CTEST_PARALLEL_LEVEL 4)

# Custom CTest configuration
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CTestCustom.cmake.in 
               ${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake @ONLY)