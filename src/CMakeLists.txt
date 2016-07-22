

configure_file(
    "cavrr_config.h.in"
    "${PROJECT_BINARY_DIR}/cavrr_config.h"
  )

include_directories ("${PROJECT_SOURCE_DIR}/util")
add_subdirectory (util)
include_directories("${PROJECT_BINARY_DIR}")

add_executable(cavrr cavrr.c)
add_library(memory memory.c)
add_library(instruction instruction_decoder.c instruction_str.c)
add_library(processor processor.c)
add_library(operation operation.c)
add_dependencies(processor instruction memory operation)
target_link_libraries( cavrr list intelhex bitutil processor memory instruction operation)

install(TARGETS cavrr DESTINATION bin)
