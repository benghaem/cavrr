#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include "push.h"
#include "../../processor.h"

void instr_push_test_decode(void **state) {
    uint16_t instr;
    enum instruction decoded;

    instr = 0x1f92;

    decoded = instruction_decode_bytes(instr);

    assert_int_equal(decoded, PUSH);
}

void instr_push_test_exec(void **state) {
    struct processor p;
    struct progmem* pmem;
    struct datamem* dmem;
    uint16_t instr1;
    uint16_t instr2;
    uint16_t result1;
    uint16_t result2;

    processor_init(&p, 1);

    pmem = &p.pmem;
    dmem = &p.dmem;

    datamem_write_reg(dmem, 1, 2);
    datamem_write_reg(dmem, 31, 4);
    // push r1
    instr1 = 0x1f92;
    // push r31
    instr2 = 0xff93;

    progmem_write_addr(pmem, 0, instr1);
    progmem_write_addr(pmem, 1, instr2);

    processor_fetch(&p);
    processor_exec(&p);

    result1 = datamem_read_addr(dmem, 0, processor_sp_read(&p) + 1);

    processor_fetch(&p);
    processor_exec(&p);

    result2 = datamem_read_addr(dmem, 0, processor_sp_read(&p) + 1);

    //check result
    assert_int_equal(result1, 2);
    assert_int_equal(result2, 4);
    //check program counter increment
    assert_int_equal(p.pc, 2);
    //check sp decrement
    assert_int_equal(DATAMEM_SIZE-3, processor_sp_read(&p));
}
