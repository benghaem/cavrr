#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "instr/add.h"
#include "instr/mov.h"
#include "instr/push.h"
#include "instr/rjmp.h"
#include "instr/stdz.h"
#include "instr/ldi.h"
#include "general/test_memory.h"

int main(void) {
    const struct CMUnitTest instr_tests[] = {
        cmocka_unit_test(instr_add_test_decode),
        cmocka_unit_test(instr_add_test_exec),
        cmocka_unit_test(instr_mov_test_decode),
        cmocka_unit_test(instr_mov_test_exec),
        cmocka_unit_test(instr_push_test_decode),
        cmocka_unit_test(instr_push_test_exec),
        cmocka_unit_test(instr_rjmp_test_decode),
        cmocka_unit_test(instr_rjmp_test_exec),
        cmocka_unit_test(instr_stdz_test_decode),
        cmocka_unit_test(instr_stdz_test_exec_no_q),
        cmocka_unit_test(instr_ldi_test_decode),
        cmocka_unit_test(instr_ldi_test_exec),
        cmocka_unit_test(test_memory_dmem_rw_addr16)
    };
    return cmocka_run_group_tests(instr_tests, NULL, NULL);
}
