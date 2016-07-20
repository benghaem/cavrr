#include "memory.h"
#include <stdio.h>
#include "IOREG.h"

int main(){
    struct datamem d;
    datamem_init(&d);
    datamem_print_region(&d, RFILE_OFFSET, RFILE_SIZE);
    datamem_write_reg(&d, 21, 0xA);
    datamem_write_reg_X(&d, 0xAABB);
    datamem_write_reg_Y(&d, 0xCCDD);
    datamem_print_region(&d, RFILE_OFFSET, RFILE_SIZE);
    printf("Reg Y: %X\n",datamem_read_reg_Y(&d));
    datamem_write_io(&d, SREG, 0xAA);
    printf("SREG: %X\n",datamem_read_io(&d, SREG));
    datamem_write_io_bit(&d, SREG, SREG_T,1);
    printf("SREG: %X\n",datamem_read_io(&d, SREG));
    datamem_print_region(&d, IOFILE_OFFSET, IOFILE_SIZE + IOFILE_OFFSET);
    printf("SREG_T: %X\n",datamem_read_io_bit(&d, SREG, SREG_T));
    return 0;
}
