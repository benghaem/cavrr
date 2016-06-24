#include <stdint.h>
#include "memory.h"
#include "instruction.h"
#include "operation.h"

typedef uint16_t PC;

void PC_update(PC* p, uint16_t val);

void PC_increment(PC* p);

typedef enum{
    INIT,
    FETCH,
    EXEC,
    WAITING,
    HALT,
} PSTATE;

typedef struct{
    PC pc;
    PSTATE state;
    OP oper;
    DATAMEM dmem;
    PROGMEM pmem;
}PROCESSOR;

void PROCESSOR_init(PROCESSOR* p);

void PROCESSOR_loop(PROCESSOR* p);

void PROCESSOR_fetch(PROCESSOR* p);

void PROCESSOR_exec(PROCESSOR* p);


// --------------------------------- //
// Px Functions (op execution)       //
// --------------------------------- //

void PxADD(PROCESSOR* p);

void PxNOP(PROCESSOR* p);
