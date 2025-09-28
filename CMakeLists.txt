cmake_minimum_required(VERSION 3.14)

# Project name and version
project(multi_threads VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler to clang
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Generate compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Include directories
include_directories(include)

# Find required packages
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# Enable testing and include CTest
enable_testing()
include(CTest)

# Configure CTest custom file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CTestCustom.cmake.in 
               ${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake @ONLY)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(perf)