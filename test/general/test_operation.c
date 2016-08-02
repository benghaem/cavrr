#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "test_operation.h"
#include "operation.h"

void test_operation_op_get_reg_direct(void **state){
    struct operation op_max, op_min;
    uint8_t r;
    /*
     * ---- | ---r | rrrr | ----
     * rrrr | ---- | ---- | ---r
     */
    op_max.bits = 0xF001;
    op_min.bits = 0x0FFE;

    op_get_reg_direct(&op_max, &r);
    assert_int_equal(r, 0x1F);

    op_get_reg_direct(&op_min, &r);
    assert_int_equal(r, 0x00);

    return;
}


void test_operation_op_get_reg_direct_2(void **state){
    struct operation op_max, op_min;
    uint8_t r, d;
    /*
     * ---- | --rd | dddd | rrrr
     * dddd | rrrr | ---- | --rd
     */
    op_max.bits = 0xFF03;
    op_min.bits = 0x00FC;


    op_get_reg_direct_2(&op_max, &d, &r);
    assert_int_equal(r, 0x1F);
    assert_int_equal(d, 0x1F);

    op_get_reg_direct_2(&op_min, &d, &r);
    assert_int_equal(r, 0x00);
    assert_int_equal(d, 0x00);

    return;
}


void test_operation_op_get_reg_direct_2_short(void **state){
    struct operation op_max, op_min;
    uint8_t r, d;
    /*
     * ---- | ---- | dddd | rrrr
     * dddd | rrrr | ---- | ----
     */
    op_max.bits = 0xFF00;
    op_min.bits = 0x00FF;


    op_get_reg_direct_2_short(&op_max, &d, &r);
    assert_int_equal(r, 0x1E);
    assert_int_equal(d, 0x1E);

    op_get_reg_direct_2_short(&op_min, &d, &r);
    assert_int_equal(r, 0x00);
    assert_int_equal(d, 0x00);

    return;
}


void test_operation_op_get_reg_imm(void **state){
    struct operation op_max, op_min;
    uint8_t d, K;

    /*
     * ---- | KKKK | dddd | KKKK
     * dddd | KKKK | ---- | KKKK
     * 16<= d <= 31
     */
    op_max.bits = 0xFF0F;
    op_min.bits = 0x00F0;

    op_get_reg_imm(&op_max, &d, &K);
    assert_int_equal(K,0xFF);
    assert_int_equal(d,31);

    op_get_reg_imm(&op_min, &d, &K);
    assert_int_equal(K,0x00);
    assert_int_equal(d,16);

}


void test_operation_op_get_reg16_imm(void **state){
    struct operation op_max, op_min;
    uint8_t d, K;

    /*
     * ---- | ---- | KKdd | KKKK
     * KKdd | KKKK | ---- | ----
     * d = 24,26,28,30
     */
    op_max.bits = 0xFF00;
    op_min.bits = 0x00FF;

    op_get_reg16_imm(&op_max, &d, &K);
    assert_int_equal(K,0x3F);
    assert_int_equal(d,30);

    op_get_reg16_imm(&op_min, &d, &K);
    assert_int_equal(K,0x00);
    assert_int_equal(d,24);

    return;
}


void test_operation_op_get_reg_displacement(void **state){
    struct operation op_max, op_min, op_group_extra;
    uint8_t d, q;

    /* --q- | qq-d | dddd | -qqq
     * dddd | -qqq | --q- | qq-d
     */


    op_max.bits = 0xF72D;
    op_min.bits = 0x08D2;

    op_group_extra.bits = 0x0020;

    op_get_reg_displacement(&op_group_extra, &d, &q);
    assert_int_equal(q,0x20);
    assert_int_equal(d,0);

    op_group_extra.bits = 0x0700;

    op_get_reg_displacement(&op_group_extra, &d, &q);
    assert_int_equal(q,7);
    assert_int_equal(d,0);

    op_get_reg_displacement(&op_max, &d, &q);
    assert_int_equal(q,63);
    assert_int_equal(d,31);

    op_get_reg_displacement(&op_min, &d, &q);
    assert_int_equal(q,0x00);
    assert_int_equal(d,0);

    return;
}


void test_operation_op_get_io_direct(void **state){
    struct operation op_max, op_min;
    uint8_t A, d;

    /*
     * ---- | -AAd | dddd | AAAA
     * dddd | AAAA | ---- | -AAd
     */

    op_max.bits = 0xFF07;
    op_min.bits = 0x00F8;

    op_get_io_direct(&op_max, &A, &d);
    assert_int_equal(A,0x3F);
    assert_int_equal(d,0x1F);

    op_get_io_direct(&op_min, &A, &d);
    assert_int_equal(A,0x00);
    assert_int_equal(d,0x00);

    return;

}
void test_operation_op_get_rel_addr(void **state){
    struct operation op_max, op_min;
    int16_t K;
    /*
     * ---- | KKKK | KKKK | KKKK
     * KKKK | KKKK | ---- | KKKK
     */

    /*
     * K is in 2's complement thus
     * 1000 0000 0000 = -2048
     * 0111 1111 1111 =  2047
     */

    op_max.bits = 0xFF07;
    op_min.bits = 0x0008;

    op_get_rel_addr(&op_max, &K);
    assert_int_equal(K,2047);

    op_get_rel_addr(&op_min, &K);
    assert_int_equal(K,-2048);

    return;

}
void test_operation_op_get_rel_addr_sreg(void **state){ struct operation op_max, op_min;
    int8_t k;
    int s;
    /*
     * ---- | --kk | kkkk | ksss
     * kkkk | ksss | ---- | --kk
     */

    /* k here is in two's complement thus
     * 1000000 = -64
     * 0111111 = 63
     *
     * we want max to be:
     * k = 63 s = 7
     * ---- | --01 | 1111 | 1111
     * 1111 | 1111 | ---- | --01
     * and min to be:
     * k = -64 s = 0
     * ---- | --10 | 0000 | 0000
     * 0000 | 0000 | ---- | --10
     */

    op_max.bits = 0xFF01;
    op_min.bits = 0x00FE;

    op_get_rel_addr_sreg(&op_max, &k, &s);
    assert_int_equal(s,7);
    assert_int_equal(k,63);

    op_get_rel_addr_sreg(&op_min, &k, &s);
    assert_int_equal(s,0);
    assert_int_equal(k,-64);


}

void test_operation_op_get_sreg(void **state){
    struct operation op_max, op_min;
    int s;
    /*
     * ---- | ---- | -sss | ----
     * -sss | ---- | ---- | ----
     */
    op_max.bits = 0x7000;
    op_min.bits = 0x0000;

    op_get_sreg(&op_max, &s);
    assert_int_equal(s,7);

    op_get_sreg(&op_min, &s);
    assert_int_equal(s,0);

}
