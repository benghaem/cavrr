#include "memory.h"
#include <stdint.h>

typedef enum{
    //Add without carry
    ADD,
    AND,
    CPC,
    EOR,
    FMUL,
    FMULS,
    FMULSU,
    MULSU,
    MOV,
    MOVW,
    NOP,
    OR,
    SBC,
    UNKNOWN,

}INSTRUCTION;

INSTRUCTION bytes_to_instruction(uint16_t bytes);

void INSTRUCTION_print(INSTRUCTION instr);

typedef struct
{

    INSTRUCTION inst;
    uint16_t bits;
    uint16_t ex_bits;

} OPERATION;
