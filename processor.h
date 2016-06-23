#include <stdint.h>
#include "memory.h"
#include "instruction.h"

typedef uint16_t PC;

void PC_update(PC*, uint16_t val);

typedef enum{
    waiting,

} PSTATE;

typedef struct{
    PC pc;
    PSTATE state;
    INSTRUCTION instr;
    DATAMEM dmem;
    PROGMEM pmem;
}PROCESSOR;
