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
#include "general/test_operation.h"

int main(void) {

    int failed = 0;

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
    };

    const struct CMUnitTest oper_tests[] = {
        cmocka_unit_test(test_operation_op_get_reg_direct),
        cmocka_unit_test(test_operation_op_get_reg_direct_2),
        cmocka_unit_test(test_operation_op_get_reg_imm),
        cmocka_unit_test(test_operation_op_get_reg16_imm),
        cmocka_unit_test(test_operation_op_get_io_direct),
        cmocka_unit_test(test_operation_op_get_rel_addr),
        cmocka_unit_test(test_operation_op_get_rel_addr_sreg),
    };


    failed += cmocka_run_group_tests(instr_tests, NULL, NULL);
    failed += cmocka_run_group_tests(oper_tests, NULL, NULL);

    return failed;
}
