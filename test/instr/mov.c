#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "mov.h"
#include "../../processor.h"

void instr_mov_test_decode(void **state) {
    uint16_t instr;
    enum instruction decoded;

    instr = 0x322c;

    decoded = instruction_decode_bytes(instr);

    assert_int_equal(decoded, MOV);
}

void instr_mov_test_exec(void **state) {
    struct processor p;
    struct progmem* pmem;
    struct datamem* dmem;
    uint16_t instr;
    uint16_t result;

    processor_init(&p, 1);

    pmem = &p.pmem;
    dmem = &p.dmem;

    //write to r2, r3
    datamem_write_reg(dmem, 2, 5);
    datamem_write_reg(dmem, 3, 2);

    // MOV r3, r2
    instr = 0x322c;

    progmem_write_addr(pmem, 0, instr);

    processor_fetch(&p);
    processor_exec(&p);

    result = datamem_read_reg(dmem, 3);


    //check result
    assert_int_equal(result, 5);
    //check program counter increment
    assert_int_equal(p.pc, 1);
}
