include_directories("${PROJECT_SOURCE_DIR}/src")

add_subdirectory(instr)
add_subdirectory(integration)
add_subdirectory(general)

set(all_test_libs ${all_test_libs} instr_test)
set(all_test_libs ${all_test_libs} integr_test)
set(all_test_libs ${all_test_libs} gen_test)

add_executable(test_cavrr test.c)
add_test(NAME test_cavrr COMMAND test_cavrr)
target_link_libraries(test_cavrr cmocka ${all_test_libs} processor memory instruction)
