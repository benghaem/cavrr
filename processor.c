#include "processor.h"
#include "operation.h"
#include "util/bitutil.h"
#include "IOREG.h"

/* Update the program counter to a new value */
void PC_update(PC* pc, uint16_t val){
    /* mask out bits above 11 */
    /* 0000 0111 1111 1111 */
    *pc = val & 0x07FF;
    return;
}

/* Increment the program counter by one */
void PC_increment(PC* pc){
    *pc++;
    if (*pc > 0x07FF){
        *pc = 0;
    }
    return;
}

/* Initialize the processor */
void PROCESSOR_init(PROCESSOR* p){
    /* set to first state */
    p->state = FETCH;
    /* initialize the memory regions */
    DATAMEM_init(&(p->dmem));
    PROGMEM_init(&(p->pmem));
    return;
}

/* Run the processor state machine loop */
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


/* Fetch the next instruction from the data memory */
/* and transform into an operation */
void PROCESSOR_fetch(PROCESSOR* p){
    uint16_t bits;
    uint16_t ex_bits;

    bits = PROGMEM_read_addr(&p->pmem, p->pc);

    INSTRUCTION next = INSTRUCTION_decode_bytes(bits);


    /* if the instruction is 32b */
    if (INSTRUCTION_is_32b(next)){
        PC_increment(&p->pc);
        ex_bits = PROGMEM_read_addr(&p->pmem, p->pc);
    }

    p->oper.inst = next;
    p->oper.bits = bits;
    p->oper.ex_bits = ex_bits;

    p->state = EXEC;
    PC_increment(&p->pc);
}

void PROCESSOR_exec(PROCESSOR* p){
    switch (p->oper.inst){
        case ADD:
            PxADD(p);
            break;
        default:
            PxNOP(p);
            INSTRUCTION_str(p->oper.inst);
            break;
    }
}


/*---------------------------------*/
/* Px Functions (op execution)     */
/*---------------------------------*/


/*---------------------------------*/
/* ADD 0000 | 11rd | dddd | rrrr   */
/* --> dddd | rrrr | 0000 | 11rd   */
/* d - destination                 */
/* r - source                      */
/*---------------------------------*/
void PxADD(PROCESSOR* p){
    uint8_t r = 0;
    uint8_t d = 0;
    uint8_t R;
    uint8_t Rr;
    uint8_t Rd;
    int H, S, V, N, Z, C;

    /* Isolate r and d */
    r = (( p->oper.bits & 0x0F00 ) >> 8 ) | (( p->oper.bits >> 1 ) & 0x1 );
    d = (( p->oper.bits & 0xF000 ) >> 12 ) | ( p->oper.bits & 0x1 );

    /* Get values of r and d */
    Rr = DATAMEM_read_reg(&p->dmem, r);
    Rd = DATAMEM_read_reg(&p->dmem, d);

    /* ADD */
    R = Rd + Rr;

    /* Set new value of Rd */
    DATAMEM_write_reg(&p->dmem, d, R);

    /* Set SREG flags */

    H = bit(Rd,3) & bit(Rr,3) | bit(Rr,3) & ~bit(R,3) | ~bit(R,3) & bit(Rd,3);
    V = bit(Rd,7) & bit(Rr,7) & ~bit(R,7) | ~bit(Rd,7) & ~bit(Rr,7) & bit(R,7);
    N = bit(R,7);
    S = N ^ V;
    Z = (R = 0) ? 1 : 0;
    C = bit(Rd,7) & bit(Rr,7) | bit(Rr,7) & ~bit(R,7) | ~bit(R,7) & bit(Rd,7);

    DATAMEM_write_io_bit(&p->dmem, SREG, SREG_H, H);
    DATAMEM_write_io_bit(&p->dmem, SREG, SREG_V, V);
    DATAMEM_write_io_bit(&p->dmem, SREG, SREG_N, N);
    DATAMEM_write_io_bit(&p->dmem, SREG, SREG_S, S);
    DATAMEM_write_io_bit(&p->dmem, SREG, SREG_Z, Z);
    DATAMEM_write_io_bit(&p->dmem, SREG, SREG_C, C);

    return;
}
