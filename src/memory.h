/* memory.h */
#include <stdint.h>

#ifndef MEMORY_H
#define MEMORY_H

/* 256 SRAM (ATtiny45) */
/* 32 + 64 + 256 = 352 [REG + IO + SRAM] */
#define DATAMEM_SIZE 352

/* 2048 Program Memory Size (ATtiny45) */
#define PROGMEM_SIZE 2048

/* Offsets for register access within data memory */
#define ZERO_OFFSET 0
#define RFILE_OFFSET 0
#define IOFILE_OFFSET 32
#define SRAM_OFFSET 96

#define RFILE_SIZE 32
#define IOFILE_SIZE 64
#define SRAM_SIZE 256

/* Special registers X, Y, Z */
#define REG_XL 0x1A
#define REG_XH 0x1B
#define REG_YL 0x1C
#define REG_YH 0x1D
#define REG_ZL 0x1E
#define REG_ZH 0x1F

/* Single contiguous data memory for AVR */
/* 8 Bit wide */
struct datamem{
    uint8_t mem[DATAMEM_SIZE];
} datamem;

void* datamem_init(struct datamem* d);

/* Read data from address (addresses the entire data memory) */
uint8_t datamem_read_addr(struct datamem* d, int offset, int addr);

uint16_t datamem_read_addr16(struct datamem* d, int offset, int addr_low, int addr_high);

/* Write data to address (addresses the entire data memory) */
int datamem_write_addr(struct datamem* d, int offset, int addr, uint8_t data);

int datamem_write_addr16(struct datamem* d, int offset, int addr_low, int addr_high, uint16_t data);

uint8_t datamem_read_reg(struct datamem* d, int addr);

int datamem_write_reg(struct datamem* d, int addr, uint8_t data);

uint16_t datamem_read_reg16(struct datamem* d, int addr_low, int addr_high);

int datamem_write_reg16(struct datamem* d, int addr_low, int addr_high, uint16_t data);

uint16_t datamem_read_reg_X(struct datamem* d);

uint16_t datamem_read_reg_Y(struct datamem* d);

uint16_t datamem_read_reg_Z(struct datamem* d);

int datamem_write_reg_X(struct datamem* d, uint16_t x);

int datamem_write_reg_Y(struct datamem* d, uint16_t y);

int datamem_write_reg_Z(struct datamem* d, uint16_t z);

uint8_t datamem_read_io(struct datamem* d, int addr);

int datamem_read_io_bit(struct datamem* d, int addr, int bit);

uint16_t datamem_read_io_SP(struct datamem* d);

int datamem_write_io(struct datamem* d, int addr, uint8_t data);

int datamem_write_io_bit(struct datamem* d, int addr, int bit, int data);

int datamem_write_io_SP(struct datamem* d, uint16_t sp);

uint8_t datamem_read_sram(struct datamem* d, int addr);

int datamem_write_sram(struct datamem* d, int addr, uint8_t data);

/* DEBUG EXTRAS
 * prints from [start, stop)
 *  ex: [1,4) -> 1,2,3 */
void datamem_print_region(struct datamem* d, int startAddr, int stopAddr);

/* Contiguous data memory for AVR */
struct progmem{
    uint16_t mem[PROGMEM_SIZE];
} progmem;

void* progmem_init(struct progmem* p);

uint16_t progmem_read_addr(struct progmem* p, int addr);

int progmem_write_addr(struct progmem* p, int addr, uint16_t data);

#endif /* memory.h */
