#include "processor.h"


//Update the program counter to a new value
void PC_update(PC* pc, uint16_t val){
    //mask out bits above 11
    //0000 0111 1111 1111
    *pc = val & 0x07FF;
    return;
}

//Increment the program counter by one
void PC_increment(PC* pc){
    *pc++;
    if (*pc > 0x07FF){
        *pc = 0;
    }
    return;
}

//Initialize the processor
void PROCESSOR_init(PROCESSOR* p){
    //set to first state
    p->state = FETCH;
    //initialize the memory regions
    DATAMEM_init(&(p->dmem));
    PROGMEM_init(&(p->pmem));
    return;
}

//Run the processor state machine loop
void PROCESSOR_loop(PROCESSOR* p){
    while (p->state != HALT){
        switch (p->state){
            case FETCH:
                PROCESSOR_fetch(p);
                break;
            case EXEC:
                PROCESSOR_exec(p);
                break;
            default:
                break;
        }
    }
}


//Fetch the next instruction from the data memory
//and transform into an operation
void PROCESSOR_fetch(PROCESSOR* p){
    uint16_t bits;
    uint16_t ex_bits;

    bits = PROGMEM_read_addr(&p->pmem, p->pc);

    INSTRUCTION next = INSTRUCTION_decode_bytes(bits);

    if (INSTRUCTION_is_32b(next)){
        PC_increment(&p->pc);
        ex_bits = PROGMEM_read_addr(&p->pmem, p->pc);
    }

    p->oper.inst = next;
    p->oper.bits = bits;
    p->oper.ex_bits = ex_bits;

    p->state = EXEC;
}


