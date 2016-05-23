//memory.h
#include <stdint.h>

#ifndef MEMORY_H
#define MEMORY_H

// 256 SRAM (ATtiny45)
// 32 + 64 + 256 = 352 [REG + IO + SRAM]
#define DATAMEM_SIZE 352

// 2048 Program Memory Size (ATtiny45)
#define PROGMEM_SIZE 2048

// Offsets for register access within data memory
#define RFILE_OFFSET 0
#define IOFILE_OFFSET 32
#define SRAM_OFFSET 96

#define RFILE_SIZE 32
#define IOFILE_SIZE 64
#define SRAM_SIZE 256

// Special registers X, Y, Z
#define REG_XL 0x1A
#define REG_XH 0x1B
#define REG_YL 0x1C
#define REG_YH 0x1D
#define REG_ZL 0x1E
#define REG_ZH 0x1F

// Single contiguous data memory for AVR
// 8 Bit wide
typedef struct
{
    uint8_t mem[DATAMEM_SIZE];
} DATAMEM;

void* DATAMEM_init(DATAMEM *d);

// Read data from address (addresses the entire data memory)
uint8_t DATAMEM_read_addr(DATAMEM* d, int offset, int addr);

// Write data to address (addresses the entire data memory)
int DATAMEM_write_addr(DATAMEM* d, int offset, int addr, uint8_t data);

uint8_t DATAMEM_read_reg(DATAMEM* d, int addr);

int DATAMEM_write_reg(DATAMEM* d, int addr, uint8_t data);

uint16_t DATAMEM_read_reg16(DATAMEM* d, int addrLow, int addrHigh);

int DATAMEM_write_reg16(DATAMEM* d, int addrLow, int addrHigh, uint16_t data);

uint16_t DATAMEM_read_reg_X(DATAMEM* d);

uint16_t DATAMEM_read_reg_Y(DATAMEM* d);

uint16_t DATAMEM_read_reg_Z(DATAMEM* d);

int DATAMEM_write_reg_X(DATAMEM* d, uint16_t x);

int DATAMEM_write_reg_Y(DATAMEM* d, uint16_t y);

int DATAMEM_write_reg_Z(DATAMEM* d, uint16_t z);

uint8_t DATAMEM_read_io(DATAMEM* d, int addr);

int DATAMEM_read_io_bit(DATAMEM* d, int addr, int bit);

int DATAMEM_write_io(DATAMEM* d, int addr, uint8_t data);

int DATAMEM_write_io_bit(DATAMEM* d, int addr, int bit, int data);

uint8_t DATAMEM_read_sram(DATAMEM* d, int addr);

int DATAMEM_write_sram(DATAMEM* d, int addr, uint8_t data);

//DEBUG EXTRAS
//prints from [start, stop)
//ex: [1,4) -> 1,2,3
void DATAMEM_print_region(DATAMEM* d, int startAddr, int stopAddr);

// Contiguous data memory for AVR
typedef struct
{
    uint16_t mem[PROGMEM_SIZE];
} PROGMEM;

void* PROGMEM_init(PROGMEM* p);

uint16_t PROGMEM_read_addr(PROGMEM* p, int addr);

int PROGMEM_write_addr(PROGMEM* p, int addr, uint16_t data);

#endif /* memory.h */
