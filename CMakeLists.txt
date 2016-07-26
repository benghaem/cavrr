cmake_minimum_required (VERSION 3.0)
project (cavrr)

option(COMPILE_IHEX "Compile test programs to ihex" OFF)

enable_testing()
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_C_FLAGS "-O2 -Wall -Wextra -Wno-unused-parameter -pedantic")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (cavrr_VERSION_MAJOR 0)
set (cavrr_VERSION_MINOR 1)
set (cavrr_VERSION_PATCH 0)

add_subdirectory(src)
add_subdirectory(test)
