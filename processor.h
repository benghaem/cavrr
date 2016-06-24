#include <stdint.h>
#include "memory.h"
#include "instruction.h"

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
    OPERATION oper;
    DATAMEM dmem;
    PROGMEM pmem;
}PROCESSOR;

void PROCESSOR_init(PROCESSOR* p);

void PROCESSOR_loop(PROCESSOR* p);

void PROCESSOR_fetch(PROCESSOR* p);

void PROCESSOR_exec(PROCESSOR* p);
