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
    uint16_t result;

    processor_init(&p, 1);

    pmem = &p.pmem;
    dmem = &p.dmem;

    datamem_write_reg(dmem, 0, 2);
    datamem_write_reg(dmem, 1, 2);

    // ADD r0, r1
    instr = 0x010c;

    progmem_write_addr(pmem, 0, instr);

    processor_fetch(&p);
    processor_exec(&p);

    result = datamem_read_reg(dmem, 0);


    //check result
    assert_int_equal(result, 4);
    //check program counter increment
    assert_int_equal(p.pc, 1);
}
