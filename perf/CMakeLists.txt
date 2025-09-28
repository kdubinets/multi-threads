# Performance benchmarks for B-tree library

# Collect performance test source files
set(PERF_SOURCES
    perf_driver.cpp
)

# Create performance test executable
add_executable(perf_driver ${PERF_SOURCES})

# Link libraries
target_link_libraries(perf_driver
    multi_threads
    Threads::Threads
)

# Include directories
target_include_directories(perf_driver PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Compiler features
target_compile_features(perf_driver PRIVATE cxx_std_20)

# Set optimization flags for performance testing
target_compile_options(perf_driver PRIVATE
    $<$<CONFIG:Release>:-O3 -march=native>
    $<$<CONFIG:Debug>:-O0 -g>
    -std=gnu++20
)