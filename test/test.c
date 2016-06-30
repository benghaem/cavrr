#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "instr/add.h"


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(instr_add_test_decode),
        cmocka_unit_test(instr_add_test_exec)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
