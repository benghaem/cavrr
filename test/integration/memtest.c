#include "memory.h"
#include <stdio.h>
#include "IOREG.h"

int main(){
    DATAMEM d;
    DATAMEM_init(&d);
    DATAMEM_print_region(&d, RFILE_OFFSET, RFILE_SIZE);
    DATAMEM_write_reg(&d, 21, 0xA);
    DATAMEM_write_reg_X(&d, 0xAABB);
    DATAMEM_write_reg_Y(&d, 0xCCDD);
    DATAMEM_print_region(&d, RFILE_OFFSET, RFILE_SIZE);
    printf("Reg Y: %X\n",DATAMEM_read_reg_Y(&d));
    DATAMEM_write_io(&d, SREG, 0xAA);
    printf("SREG: %X\n",DATAMEM_read_io(&d, SREG));
    DATAMEM_write_io_bit(&d, SREG, SREG_T,1);
    printf("SREG: %X\n",DATAMEM_read_io(&d, SREG));
    DATAMEM_print_region(&d, IOFILE_OFFSET, IOFILE_SIZE + IOFILE_OFFSET);
    printf("SREG_T: %X\n",DATAMEM_read_io_bit(&d, SREG, SREG_T));
    return 0;
}
