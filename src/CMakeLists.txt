# Create the multi_threads shared library

# Collect source files
set(MTHREADS_SOURCES
    tas_lock.cpp
)

# Create shared library
add_library(multi_threads SHARED ${MTHREADS_SOURCES})

# Set library properties
set_target_properties(multi_threads PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME "multi_threads"
)

# Include directories
target_include_directories(multi_threads PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Compiler features
target_compile_features(multi_threads PUBLIC cxx_std_20)
target_compile_options(multi_threads PRIVATE -std=gnu++20)

target_compile_options(multi_threads PUBLIC -std=gnu++20)

# Installation rules
install(TARGETS multi_threads
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)