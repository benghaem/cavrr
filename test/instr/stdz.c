#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "stdz.h"
#include "processor.h"
#include "IOREG.h"

void instr_stdz_test_decode(void **state) {
    uint16_t instr;
    enum instruction decoded;
    // 0000 1111 0000 1100

    instr = 0xa082;

    decoded = instruction_decode_bytes(instr);

    assert_int_equal(decoded, STD_Z);
}

void instr_stdz_test_exec_no_q(void **state) {
    struct processor p;
    struct progmem* pmem;
    struct datamem* dmem;
    uint16_t instr;
    uint16_t result1;
    uint16_t Z;

    processor_init(&p);

    pmem = &p.pmem;
    dmem = &p.dmem;

    datamem_write_reg(dmem, 10, 2);

    // st Z, r10
    instr = 0xa082;
    Z = PORTB + IOFILE_OFFSET;

    datamem_write_reg_Z(dmem, Z);

    progmem_write_addr(pmem, 0, instr);

    processor_step(&p, 1);

    result1 = datamem_read_io(dmem, PORTB);

    assert_int_equal(result1, 2);

    assert_int_equal(p.pc, 1);

}

