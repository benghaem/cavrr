#include "memory.h"

//read from address in data memory
//TODO: Should we safety check here?
uint8_t DATAMEM_read_addr(DATAMEM* d, int offset, int addr){
    return d->mem[offset+addr];
}

// write to any address in data memory
// we can provide a offset to select
// a specific subset of memory
//
// returns:
//  0 on error
//  the address minus the offset on success
int DATAMEM_write_addr(DATAMEM* d, int offset, int addr, uint8_t data){
    int target = offset + addr;
    // safety check on ranges
    if (target < DATAMEM_SIZE && target > 0){
        d->mem[target] = data;
        return target - offset;
    } else {
        return 0;
    }
}

uint8_t DATAMEM_read_reg(DATAMEM* d, int addr){
    return DATAMEM_read_addr(d, RFILE_OFFSET, addr);
}

int DATAMEM_write_reg(DATAMEM* d, int addr, uint8_t data){
    if (addr >= 0 && addr < RFILE_SIZE){
        return DATAMEM_write_addr(d, RFILE_OFFSET, addr, data);
    }
    return 0;
}

uint16_t DATAMEM_read_reg16(DATAMEM* d, int addrLow, int addrHigh){
    uint16_t L = DATAMEM_read_addr(d, RFILE_OFFSET, addrLow);
    uint16_t H = DATAMEM_read_addr(d, RFILE_OFFSET, addrHigh);
    H = H << 8;
    return H + L;
}

int DATAMEM_write_reg16(DATAMEM* d, int addrLow, int addrHigh, uint16_t data){
    //mask to remove upper 8 bits from L
    uint8_t L = data & 0xFF;
    //mask to remove lower 8 bits then to move within 8bit space
    uint8_t H = (data & 0xFF00) >> 8;
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



