cmake_minimum_required (VERSION 3.0)
project (cavrr)

enable_testing()

set (CMAKE_C_FLAGS "-Wall -Werror -Wextra -Wno-unused-parameter")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (cavrr_VERSION_MAJOR 0)
set (cavrr_VERSION_MINOR 1)
set (cavrr_VERSION_PATCH 0)

add_subdirectory(src)
add_subdirectory(test)


