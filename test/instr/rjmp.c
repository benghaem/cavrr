#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include "rjmp.h"
#include "processor.h"

void instr_rjmp_test_decode(void **state) {
    uint16_t instr;
    enum instruction decoded;

    instr = 0x01c0;

    decoded = instruction_decode_bytes(instr);

    assert_int_equal(decoded, RJMP);
}

void instr_rjmp_test_exec(void **state) {
    struct processor p;
    struct progmem* pmem;
    uint16_t instr1;
    uint16_t instr2;
    uint16_t result1;
    uint16_t result2;

    processor_init(&p);

    pmem = &p.pmem;

    /*  rjmp  +6 with k = 5 */
    instr1 = 0x05c0;
    /*  rjmp k = -2 */
    instr2 = 0xfecf;

    progmem_write_addr(pmem, 0, instr1);
    progmem_write_addr(pmem, 6, instr2);

    processor_fetch(&p);
    processor_exec(&p);

    result1 = p.pc;

    /* first instruction should land here but in case it does not */
    processor_pc_update(&p,0x6);

    processor_fetch(&p);
    processor_exec(&p);

    result2 = p.pc;

    /* check result */
    assert_int_equal(result1, 6);
    assert_int_equal(result2, 5);
}
