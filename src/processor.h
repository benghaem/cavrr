#include <stdint.h>
#include "memory.h"
#include "instruction.h"
#include "operation.h"

#ifndef PROCESSOR_H
#define PROCESSOR_H

enum pstate{
    INIT,
    FETCH,
    EXEC,
    WAITING,
    HALT
};

struct processor{
    uint16_t pc;
    enum pstate state;
    struct operation oper;
    struct datamem dmem;
    struct progmem pmem;
    int debug;
};

void processor_init(struct processor* p);

void processor_loop(struct processor* p);

void processor_step(struct processor* p, int n);

void processor_fetch(struct processor* p);

void processor_exec(struct processor* p);

void processor_pc_update(struct processor* p, uint16_t pc);

void processor_pc_increment(struct processor* p, int v);

uint16_t processor_sp_read(struct processor* p);

void processor_sp_update(struct processor* p, uint16_t sp);

void processor_sp_decrement(struct processor* p, int v);

void processor_sp_increment(struct processor* p, int v);


/* ---------------------------------  */
/* Px Functions (op execution)        */
/* ---------------------------------  */

void PxADD(struct processor* p);

void PxBREAK(struct processor* p);

void PxCOM(struct processor* p);

void PxCP(struct processor* p);

void PxEOR(struct processor* p);

void PxIN(struct processor* p);

void PxLDD_Z(struct processor* p);

void PxLDI(struct processor* p);

void PxMOV(struct processor* p);

void PxMOVW(struct processor* p);

void PxNOP(struct processor* p);

void PxOUT(struct processor* p);

void PxPUSH(struct processor* p);

void PxRCALL(struct processor* p);

void PxRJMP(struct processor* p);

void PxSTD_Z(struct processor* p);

#endif /* processor.h */
