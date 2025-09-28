# Test configuration for B-tree library

# Collect test source files
set(TEST_SOURCES
    test_tas_lock.cpp
)

# Create test executable
add_executable(multi_threads_tests ${TEST_SOURCES})

# Link libraries
target_link_libraries(multi_threads_tests
    multi_threads
    GTest::gtest
    GTest::gtest_main
    Threads::Threads
)

# Include directories
target_include_directories(multi_threads_tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Compiler features
target_compile_features(multi_threads_tests PRIVATE cxx_std_20)
target_compile_options(multi_threads_tests PRIVATE -std=gnu++20)

# Add tests to CTest
include(GoogleTest)
gtest_discover_tests(multi_threads_tests
    DISCOVERY_TIMEOUT 20
    PROPERTIES
        LABELS "unit;threading;locks"
        TIMEOUT 60
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)