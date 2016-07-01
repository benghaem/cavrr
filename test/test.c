#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "instr/add.h"
#include "instr/mov.h"


int main(void) {
    const struct CMUnitTest instr_tests[] = {
        cmocka_unit_test(instr_add_test_decode),
        cmocka_unit_test(instr_add_test_exec),
        cmocka_unit_test(instr_mov_test_decode),
        cmocka_unit_test(instr_mov_test_exec)
    };
    return cmocka_run_group_tests(instr_tests, NULL, NULL);
}
