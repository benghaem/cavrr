#include "memory.h"
#include "IOREG.h"
#include <stdio.h>
#include <string.h>
// --------------------------------- //
// AVR DATA MEMORY (REG, IO, SRAM)   //
// --------------------------------- //

void* DATAMEM_init(DATAMEM* d){
    void* ret = memset(&(d->mem[0]),0,sizeof(d->mem));
    //Set OCR1C to all 1's initial. See page 92 of datasheet
    DATAMEM_write_io(d, OCR1C, 0xFF);
    return ret;
}

//read from address in data memory
uint8_t DATAMEM_read_addr(DATAMEM* d, int offset, int addr){
    int target = offset + addr;
    if (target >= 0 && target < DATAMEM_SIZE){
        return d->mem[offset+addr];
    }
    return 0;
}

// write to any address in data memory
// we can provide a offset to select
// a specific subset of memory
//
// returns:
//  -1 on error
//  the address minus the offset on success
int DATAMEM_write_addr(DATAMEM* d, int offset, int addr, uint8_t data){
    int target = offset + addr;
    // safety check on ranges
    if (target >= 0 && target < DATAMEM_SIZE){
        d->mem[target] = data;
        return target - offset;
    } else {
        return -1;
    }
}

// --------------------------------- //
// AVR GENERAL REGISTERS             //
// --------------------------------- //

uint8_t DATAMEM_read_reg(DATAMEM* d, int addr){
    if (addr >= 0 && addr < RFILE_SIZE){
        return DATAMEM_read_addr(d, RFILE_OFFSET, addr);
    }
    return 0;
}

int DATAMEM_write_reg(DATAMEM* d, int addr, uint8_t data){
    if (addr >= 0 && addr < RFILE_SIZE){
        return DATAMEM_write_addr(d, RFILE_OFFSET, addr, data);
    }
    return -1;
}

// NOTE: Little Endian
uint16_t DATAMEM_read_reg16(DATAMEM* d, int addrLow, int addrHigh){
    uint16_t L = DATAMEM_read_addr(d, RFILE_OFFSET, addrLow);
    uint16_t H = DATAMEM_read_addr(d, RFILE_OFFSET, addrHigh);
    L = L << 8;
    return L + H;
}

// NOTE Little Endian
int DATAMEM_write_reg16(DATAMEM* d, int addrLow, int addrHigh, uint16_t data){
    //mask to remove upper 8 bits from L (Little Endian)
    uint8_t L = (data & 0xFF00) >> 8;
    //mask to remove lower 8 bits then to move within 8bit space
    uint8_t H = data & 0xFF;
    DATAMEM_write_reg(d, addrHigh, H);
    return DATAMEM_write_reg(d, addrLow, L);
}

uint16_t DATAMEM_read_reg_X(DATAMEM* d){
    return DATAMEM_read_reg16(d, REG_XL, REG_XH);
}

uint16_t DATAMEM_read_reg_Y(DATAMEM* d){
    return DATAMEM_read_reg16(d, REG_YL, REG_YH);
}

uint16_t DATAMEM_read_reg_Z(DATAMEM* d){
    return DATAMEM_read_reg16(d, REG_ZL, REG_ZH);
}

int DATAMEM_write_reg_X(DATAMEM* d, uint16_t x){
    return DATAMEM_write_reg16(d, REG_XL, REG_XH, x);
}

int DATAMEM_write_reg_Y(DATAMEM* d, uint16_t y){
    return DATAMEM_write_reg16(d, REG_YL, REG_YH, y);
}

int DATAMEM_write_reg_Z(DATAMEM* d, uint16_t Z){
    return DATAMEM_write_reg16(d, REG_ZL, REG_ZH, Z);
}

// --------------------------------- //
// AVR IO REGISTERS                  //
// --------------------------------- //

uint8_t DATAMEM_read_io(DATAMEM* d, int addr){
    if (addr >= 0 && addr < IOFILE_SIZE){
        return DATAMEM_read_addr(d, IOFILE_OFFSET, addr);
    }
    return 0;
}

int DATAMEM_read_io_bit(DATAMEM* d, int addr, int bit){
    uint8_t data = DATAMEM_read_io(d, addr);
    if (bit >=0 && bit < 8){
        return (data >> bit) & 0x1;
    }
    return -1;
}

int DATAMEM_write_io(DATAMEM* d, int addr, uint8_t data){
    if (addr >= 0 && addr < IOFILE_SIZE){
        return DATAMEM_write_addr(d,IOFILE_OFFSET, addr, data);
    }
    return -1;
}

int DATAMEM_write_io_bit(DATAMEM* d, int addr, int bit, int data){
    uint8_t current_reg = DATAMEM_read_io(d, addr);
    uint8_t isolated_bit;
    uint8_t mask;
    uint8_t updated_reg;

    if (bit >=0 && bit < 8){
        if (data == 0 || data == 1){
            mask = 0x1 << bit;
            isolated_bit = data << bit;
            //the idea here is to recognize the bit that needs to be changed
            //and whether or not we should swap it from the current value
            updated_reg = current_reg ^ ((current_reg ^ isolated_bit) & mask);
            return DATAMEM_write_io(d, addr, updated_reg);
        }
    }
    return -1;
}

// --------------------------------- //
// AVR SRAM                          //
// --------------------------------- //

uint8_t DATAMEM_read_sram(DATAMEM* d, int addr){
    if (addr >= 0 && addr < SRAM_SIZE){
        return DATAMEM_read_addr(d, addr, SRAM_OFFSET);
    }
    return 0;
}

int DATAMEM_write_sram(DATAMEM* d, int addr, uint8_t data){
    if (addr >= 0 && addr < SRAM_SIZE){
        return DATAMEM_write_addr(d, addr, SRAM_OFFSET, data);
    }
    return -1;
}

// --------------------------------- //
// DATAMEM DEBUG EXTRAS              //
// --------------------------------- //

void DATAMEM_print_region(DATAMEM* d, int startAddr, int stopAddr){
    uint8_t value;
    for (; startAddr < stopAddr; startAddr++){
        value = d->mem[startAddr];
        printf("%i : %X\n", startAddr, value);
    }
}


// --------------------------------- //
// AVR PROGRAM MEMORY                //
// --------------------------------- //

void* PROGMEM_init(PROGMEM* p){
    return memset(&(p->mem[0]),0,sizeof(p->mem));
}

uint16_t PROGMEM_read_addr(PROGMEM* p, int addr){
    if (addr >= 0 && addr < PROGMEM_SIZE){
         return p->mem[addr];
    }
    return 0;
}

int PROGMEM_write_addr(PROGMEM* p, int addr, uint16_t data){
    if (addr >= 0 && addr < PROGMEM_SIZE){
        p->mem[addr] = data;
        return addr;
    }
    return -1;
}
