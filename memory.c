#include "memory.h"
#include "IOREG.h"
#include <stdio.h>
#include <string.h>


/*  ---------------------------------  */
/*  AVR DATA MEMORY (REG, IO, SRAM)    */
/*  ---------------------------------  */

void* datamem_init(struct datamem* d){
    void* ret = memset(&(d->mem[0]),0,sizeof(d->mem));
    /* Set OCR1C to all 1's initial. See page 92 of datasheet */
    datamem_write_io(d, OCR1C, 0xFF);
    return ret;
}

/* read from address in data memory */
uint8_t datamem_read_addr(struct datamem* d, int offset, int addr){
    int target = offset + addr;
    if (target >= 0 && target < DATAMEM_SIZE){
        return d->mem[offset+addr];
    }
    return 0;
}

/*  write to any address in data memory */
/*  we can provide a offset to select */
/*  a specific subset of memory */
/*  */
/*  returns: */
/*   -1 on error */
/*   the address minus the offset on success */
int datamem_write_addr(struct datamem* d, int offset, int addr, uint8_t data){
    int target = offset + addr;
    /*  safety check on ranges */
    if (target >= 0 && target < DATAMEM_SIZE){
        d->mem[target] = data;
        return target - offset;
    } else {
        return -1;
    }
}

/*  ---------------------------------  */
/*  AVR GENERAL REGISTERS              */
/*  ---------------------------------  */

uint8_t datamem_read_reg(struct datamem* d, int addr){
    if (addr >= 0 && addr < RFILE_SIZE){
        return datamem_read_addr(d, RFILE_OFFSET, addr);
    }
    return 0;
}

int datamem_write_reg(struct datamem* d, int addr, uint8_t data){
    if (addr >= 0 && addr < RFILE_SIZE){
        return datamem_write_addr(d, RFILE_OFFSET, addr, data);
    }
    return -1;
}

/*  NOTE: Little Endian */
uint16_t datamem_read_reg16(struct datamem* d, int addrLow, int addrHigh){
    uint16_t L = datamem_read_addr(d, RFILE_OFFSET, addrLow);
    uint16_t H = datamem_read_addr(d, RFILE_OFFSET, addrHigh);
    L = L << 8;
    return L + H;
}

/*  NOTE Little Endian */
int datamem_write_reg16(struct datamem* d, int addrLow, int addrHigh, uint16_t data){
    /* mask to remove upper 8 bits from L (Little Endian) */
    uint8_t L = (data & 0xFF00) >> 8;
    /* mask to remove lower 8 bits then to move within 8bit space */
    uint8_t H = data & 0xFF;
    datamem_write_reg(d, addrHigh, H);
    return datamem_write_reg(d, addrLow, L);
}

uint16_t datamem_read_reg_X(struct datamem* d){
    return datamem_read_reg16(d, REG_XL, REG_XH);
}

uint16_t datamem_read_reg_Y(struct datamem* d){
    return datamem_read_reg16(d, REG_YL, REG_YH);
}

uint16_t datamem_read_reg_Z(struct datamem* d){
    return datamem_read_reg16(d, REG_ZL, REG_ZH);
}

int datamem_write_reg_X(struct datamem* d, uint16_t x){
    return datamem_write_reg16(d, REG_XL, REG_XH, x);
}

int datamem_write_reg_Y(struct datamem* d, uint16_t y){
    return datamem_write_reg16(d, REG_YL, REG_YH, y);
}

int datamem_write_reg_Z(struct datamem* d, uint16_t Z){
    return datamem_write_reg16(d, REG_ZL, REG_ZH, Z);
}

/*  ---------------------------------  */
/*  AVR IO REGISTERS                   */
/*  ---------------------------------  */

uint8_t datamem_read_io(struct datamem* d, int addr){
    if (addr >= 0 && addr < IOFILE_SIZE){
        return datamem_read_addr(d, IOFILE_OFFSET, addr);
    }
    return 0;
}

int datamem_read_io_bit(struct datamem* d, int addr, int bit){
    uint8_t data = datamem_read_io(d, addr);
    if (bit >=0 && bit < 8){
        return (data >> bit) & 0x1;
    }
    return -1;
}

int datamem_write_io(struct datamem* d, int addr, uint8_t data){
    if (addr >= 0 && addr < IOFILE_SIZE){
        return datamem_write_addr(d,IOFILE_OFFSET, addr, data);
    }
    return -1;
}

int datamem_write_io_bit(struct datamem* d, int addr, int bit, int data){
    uint8_t current_reg = datamem_read_io(d, addr);
    uint8_t isolated_bit;
    uint8_t mask;
    uint8_t updated_reg;

    if (bit >=0 && bit < 8){
        if (data == 0 || data == 1){
            mask = 0x1 << bit;
            isolated_bit = data << bit;
            /* the idea here is to recognize the bit that needs to be changed */
            /* and whether or not we should swap it from the current value */
            updated_reg = current_reg ^ ((current_reg ^ isolated_bit) & mask);
            return datamem_write_io(d, addr, updated_reg);
        }
    }
    return -1;
}

/*  ---------------------------------  */
/*  AVR SRAM                           */
/*  ---------------------------------  */

uint8_t datamem_read_sram(struct datamem* d, int addr){
    if (addr >= 0 && addr < SRAM_SIZE){
        return datamem_read_addr(d, addr, SRAM_OFFSET);
    }
    return 0;
}

int datamem_write_sram(struct datamem* d, int addr, uint8_t data){
    if (addr >= 0 && addr < SRAM_SIZE){
        return datamem_write_addr(d, addr, SRAM_OFFSET, data);
    }
    return -1;
}

/*  ---------------------------------  */
/*  datamem DEBUG EXTRAS               */
/*  ---------------------------------  */

void datamem_print_region(struct datamem* d, int startAddr, int stopAddr){
    uint8_t value;
    for (; startAddr < stopAddr; startAddr++){
        value = d->mem[startAddr];
        printf("%i : %X\n", startAddr, value);
    }
}


/*  ---------------------------------  */
/*  AVR PROGRAM MEMORY                 */
/*  ---------------------------------  */

void* progmem_init(struct progmem* p){
    return memset(&(p->mem[0]),0,sizeof(p->mem));
}

uint16_t progmem_read_addr(struct progmem* p, int addr){
    if (addr >= 0 && addr < PROGMEM_SIZE){
         return p->mem[addr];
    }
    return 0;
}

int progmem_write_addr(struct progmem* p, int addr, uint16_t data){
    if (addr >= 0 && addr < PROGMEM_SIZE){
        p->mem[addr] = data;
        return addr;
    }
    return -1;
}
