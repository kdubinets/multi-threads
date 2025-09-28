#!/usr/bin/env cmake -P

# CTest script for automated testing of Multi Threads project
# Usage: ctest -S run_tests.cmake

# Set the source and binary directories
set(CTEST_SOURCE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
set(CTEST_BINARY_DIRECTORY "${CTEST_SOURCE_DIRECTORY}/build")

# Set the site name and build name
site_name(CTEST_SITE)
set(CTEST_BUILD_NAME "Multi-Threads-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

# Set the build configuration
if(NOT DEFINED CTEST_BUILD_CONFIGURATION)
    set(CTEST_BUILD_CONFIGURATION "Debug")
endif()

# Set the generator
if(NOT DEFINED CTEST_CMAKE_GENERATOR)
    set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
endif()

# Configure the build
set(CTEST_CONFIGURE_COMMAND "${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=${CTEST_BUILD_CONFIGURATION}")
set(CTEST_CONFIGURE_COMMAND "${CTEST_CONFIGURE_COMMAND} -DCMAKE_C_COMPILER=clang")
set(CTEST_CONFIGURE_COMMAND "${CTEST_CONFIGURE_COMMAND} -DCMAKE_CXX_COMPILER=clang++")
set(CTEST_CONFIGURE_COMMAND "${CTEST_CONFIGURE_COMMAND} \"${CTEST_SOURCE_DIRECTORY}\"")

# Set build command with parallel jobs
set(CTEST_BUILD_COMMAND "make -j")

# Start the testing
ctest_start("Experimental")

# Update from repository (optional)
# ctest_update()

# Configure the project
ctest_configure()

# Build the project
ctest_build()

# Run tests
ctest_test(RETURN_VALUE test_result)

# Optional: Run memory checks with valgrind (uncomment if valgrind is available)
# find_program(VALGRIND_COMMAND valgrind)
# if(VALGRIND_COMMAND)
#     set(CTEST_MEMORYCHECK_COMMAND ${VALGRIND_COMMAND})
#     ctest_memcheck()
# endif()

# Optional: Generate coverage report (uncomment if gcov is available)
# find_program(GCOV_COMMAND gcov)
# if(GCOV_COMMAND)
#     set(CTEST_COVERAGE_COMMAND ${GCOV_COMMAND})
#     ctest_coverage()
# endif()

# Submit results (uncomment if using CDash)
# ctest_submit()

# Exit with test result
if(test_result)
    message(FATAL_ERROR "Tests failed!")
endif()

message(STATUS "All tests passed successfully!")