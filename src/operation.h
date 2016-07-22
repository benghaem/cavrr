#include <stdint.h>
#include <string.h>
#include "instruction.h"

#ifndef OPERATION_H
#define OPERATION_H

struct operation
{

    enum instruction inst;
    uint16_t bits;
    uint16_t ex_bits;

};

/* op var isolation */

void op_get_reg_direct(struct operation *op, uint8_t* d);

void op_get_reg_direct_2(struct operation* op, uint8_t* r, uint8_t* d);

void op_get_reg_imm(struct operation* op, uint8_t* d, uint8_t* K);

void op_get_io_direct(struct operation* op, uint8_t* A, uint8_t* d);

void op_get_rel_addr(struct operation* op, int16_t* K);


/* disassembler */

void disassemble_to_str(struct operation* op, uint16_t pc, char* str, size_t max_len);

#endif /* operation.h */
