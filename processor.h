#include <stdint.h>
#include "memory.h"
#include "instruction.h"
#include "operation.h"

typedef uint16_t PC;

void PC_update(PC* p, uint16_t val);

void PC_increment(PC* p);

enum pstate{
    INIT,
    FETCH,
    EXEC,
    WAITING,
    HALT
};

struct processor{
    PC pc;
    enum pstate state;
    struct operation oper;
    struct datamem dmem;
    struct progmem pmem;
    int debug;
};

void processor_init(struct processor* p, int debug);

void processor_loop(struct processor* p);

void processor_fetch(struct processor* p);

void processor_exec(struct processor* p);


/* ---------------------------------  */
/* Px Functions (op execution)        */
/* ---------------------------------  */

void PxADD(struct processor* p);

void PxBREAK(struct processor* p);

void PxMOV(struct processor* p);

void PxNOP(struct processor* p);
