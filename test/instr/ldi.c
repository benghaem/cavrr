#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "ldi.h"
#include "processor.h"

void instr_ldi_test_decode(void **state) {
    uint16_t instr;
    enum instruction decoded;

    instr = 0x00e0;

    decoded = instruction_decode_bytes(instr);

    assert_int_equal(decoded, LDI);
}

void instr_ldi_test_exec(void **state) {
    struct processor p;
    struct progmem* pmem;
    struct datamem* dmem;

    /* ldi r16, 0 | ldi r31, 1 | ldi r17, 255 | ldi r30, 255 */
    uint16_t instr[4] = {0x00e0, 0xf1e0, 0x1fef, 0xefef};
    uint16_t result[4] = {0};
    int i;


    processor_init(&p);

    pmem = &p.pmem;
    dmem = &p.dmem;

    for(i = 0; i < 4; i++){
        progmem_write_addr(pmem, i, instr[i]);
    }

    /* Write a value to r16 so we can overwrite it */
    datamem_write_reg(dmem, 16, 2);

    processor_step(&p, 4);


    result[0] = datamem_read_reg(dmem, 16);
    result[1] = datamem_read_reg(dmem, 31);
    result[2] = datamem_read_reg(dmem, 17);
    result[3] = datamem_read_reg(dmem, 30);

    //check result
    assert_int_equal(result[0], 0);
    assert_int_equal(result[1], 1);
    assert_int_equal(result[2], 255);
    assert_int_equal(result[3], 255);
    //check program counter increment
    assert_int_equal(p.pc, 4);
}
