#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "test_memory.h"
#include "memory.h"

void test_memory_dmem_rw_addr16(void **state){
    struct datamem d;
    uint16_t output;

    //no offset 0 -> L 1 -> H
    datamem_write_addr16(&d, 0, 0, 1, 0xABCD);
    output = datamem_read_addr16(&d, 0, 0, 1);

    assert_int_equal(output, 0xABCD);
}
