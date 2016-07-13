#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "add.h"
#include "../../processor.h"

void instr_add_test_decode(void **state) {
    uint16_t instr;
    enum instruction decoded;
    // 0000 1111 0000 1100

    instr = 0x0f0c;

    decoded = instruction_decode_bytes(instr);

    assert_int_equal(decoded, ADD);
}

void instr_add_test_exec(void **state) {
    struct processor p;
    struct progmem* pmem;
    struct datamem* dmem;
    uint16_t instr;
    uint16_t result1, result2;

    processor_init(&p);

    pmem = &p.pmem;
    dmem = &p.dmem;

    datamem_write_reg(dmem, 0, 2);
    datamem_write_reg(dmem, 1, 2);
    datamem_write_reg(dmem, 31, 7);

    // ADD r0, r1
    instr = 0x010c;

    //ADD r1, r31
    // 0x1f0e

    progmem_write_addr(pmem, 0, instr);
    progmem_write_addr(pmem, 1, 0x1f0e);

    processor_fetch(&p);
    processor_exec(&p);

    processor_fetch(&p);
    processor_exec(&p);

    result1 = datamem_read_reg(dmem, 0);
    result2 = datamem_read_reg(dmem, 1);


    //check result
    assert_int_equal(result1, 4);
    assert_int_equal(result2, 9);
    //check program counter increment
    assert_int_equal(p.pc, 2);
}
