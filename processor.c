#include "processor.h"
#include "operation.h"
#include "util/bitutil.h"
#include "IOREG.h"
#include <stdio.h>

/* Update the program counter to a new value */
void PC_update(PC* pc, uint16_t val){
    /* mask out bits above 11 */
    /* 0000 0111 1111 1111 */
    *pc = val & 0x07FF;
    return;
}

/* Increment the program counter by one */
void PC_increment(PC* pc){
    *pc = *pc + 1;
    if (*pc > 0x07FF){
        *pc = 0;
    }
    return;
}

/* Initialize the processor */
void processor_init(struct processor* p, int debug){
    /* set to first state */
    p->state = FETCH;
    /* initialize the memory regions */
    datamem_init(&(p->dmem));
    progmem_init(&(p->pmem));

    p->debug = debug;
    return;
}

/* Run the processor state machine loop */
void processor_loop(struct processor* p){
    while (p->state != HALT){
        switch (p->state){
            case FETCH:
                processor_fetch(p);
                break;
            case EXEC:
                processor_exec(p);
                break;
            default:
                break;
        }
    }
}


/* Fetch the next instruction from the data memory */
/* and transform into an operation */
void processor_fetch(struct processor* p){
    uint16_t bits;
    uint16_t ex_bits;
    enum instruction next;

    bits = progmem_read_addr(&p->pmem, p->pc);

    next = instruction_decode_bytes(bits);


    /* if the instruction is 32b */
    if (instruction_is_32b(next)){
        PC_increment(&p->pc);
        ex_bits = progmem_read_addr(&p->pmem, p->pc);
    }

    p->oper.inst = next;
    p->oper.bits = bits;
    p->oper.ex_bits = ex_bits;

    p->state = EXEC;
    PC_increment(&p->pc);
    return;
}

void processor_exec(struct processor* p){

    if (p->debug){
        printf("Executing: %s\n",instruction_str(p->oper.inst));
    }

    switch (p->oper.inst){
        case ADD:
            PxADD(p);
            break;
        case NOP:
            PxNOP(p);
            break;
        case BREAK:
            PxBREAK(p);
            p->state = HALT;
            return;
        default:
            printf("EXEC: Not implemented\n");
            break;
    }

    p->state = FETCH;
    return;
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
void PxADD(struct processor* p){
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
    Rr = datamem_read_reg(&p->dmem, r);
    Rd = datamem_read_reg(&p->dmem, d);

    /* ADD */
    R = Rd + Rr;

    /* Set new value of Rd */
    datamem_write_reg(&p->dmem, d, R);

    /* Set SREG flags */

    H = (bit(Rd,3) & bit(Rr,3)) | (bit(Rr,3) & ~bit(R,3)) | (~bit(R,3) & bit(Rd,3));
    V = (bit(Rd,7) & bit(Rr,7) & ~bit(R,7)) | (~bit(Rd,7) & ~bit(Rr,7) & bit(R,7));
    N = bit(R,7);
    S = N ^ V;
    Z = (R = 0) ? 1 : 0;
    C = (bit(Rd,7) & bit(Rr,7)) | (bit(Rr,7) & ~bit(R,7)) | (~bit(R,7) & bit(Rd,7));

    datamem_write_io_bit(&p->dmem, SREG, SREG_H, H);
    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);
    datamem_write_io_bit(&p->dmem, SREG, SREG_C, C);

    return;
}

/*---------------------------------*/
/* BREAK 1001 | 0101 | 1001 | 1000 */
/* --> 1001 | 1000 | 1001 | 0101   */
/*---------------------------------*/
void PxBREAK(struct processor* p){
    /* BREAK */
    return;
}

/*---------------------------------*/
/* NOP 0000 | 0000 | 0000 | 0000   */
/* --> 0000 | 0000 | 0000 | 0000   */
/*---------------------------------*/
void PxNOP(struct processor* p){
    /* NOP */
    return;
}
