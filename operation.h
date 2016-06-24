#include <stdint.h>

#ifndef OPERATION_H
#define OPERATION_H

typedef struct
{

    INSTRUCTION inst;
    uint16_t bits;
    uint16_t ex_bits;

} OP;

#endif /* operation.h */
