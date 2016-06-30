#include <stdint.h>

#ifndef OPERATION_H
#define OPERATION_H

struct operation
{

    enum instruction inst;
    uint16_t bits;
    uint16_t ex_bits;

};

#endif /* operation.h */
