#include "processor.h"
#include "operation.h"
#include "util/bitutil.h"
#include "IOREG.h"
#include <stdio.h>
#include "memory.h"

/*
 * Initializes the processor
 */
void processor_init(struct processor* p){
    struct operation nop = {NOP,0x0,0x0};
    /* Initialize program counter */
    p->pc = 0;
    /* Init operation with nop */
    p->oper = nop;
    /* set to first state */
    p->state = FETCH;
    /* initialize the memory regions */
    datamem_init(&(p->dmem));
    progmem_init(&(p->pmem));

    p->debug = 0;
    return;
}

/*
 * Runs the processor state machine loop
 */
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

/*
 * Steps the state machine forward n steps
 */
void processor_step(struct processor* p, int n){
    for (; n > 0; n--){
        processor_fetch(p);
        processor_exec(p);
    }
}

/*
 * Fetches the next instruction from the data memory
 * and transforms it into an operation
 */
void processor_fetch(struct processor* p){
    uint16_t bits;
    uint16_t ex_bits = 0x0000;
    enum instruction next;

    if (p->debug){
        printf("FETCH\n");
    }

    bits = progmem_read_addr(&p->pmem, p->pc);

    next = instruction_decode_bytes(bits);


    /* if the instruction is 32b */
    if (instruction_is_32b(next)){
        /* read the next address as well */
        ex_bits = progmem_read_addr(&p->pmem, p->pc + 1);
    }

    p->oper.inst = next;
    p->oper.bits = bits;
    p->oper.ex_bits = ex_bits;

    p->state = EXEC;
    return;
}

/*
 * Executes the current instruction
 */
void processor_exec(struct processor* p){

    if (p->debug){
        printf("EXEC: %s\n",instruction_str(p->oper.inst));
    }

    switch (p->oper.inst){
        case ADD:
            PxADD(p);
            break;
        case ADIW:
            PxADIW(p);
            break;
        case AND:
            PxAND(p);
            break;
        case ANDI:
            PxANDI(p);
            break;
        case BCLR:
            PxBCLR(p);
            break;
        case BRBC:
            PxBRBC(p);
            break;
        case BRBS:
            PxBRBS(p);
            break;
        case BREAK:
            PxBREAK(p);
            return;
        case BSET:
            PxBSET(p);
            return;
        case COM:
            PxCOM(p);
            break;
        case CP:
            PxCP(p);
            break;
        case CPC:
            PxCPC(p);
            break;
        case DEC:
            PxDEC(p);
            break;
        case EOR:
            PxEOR(p);
            break;
        case IN:
            PxIN(p);
            break;
        case INC:
            PxINC(p);
            break;
        case LD_X:
            PxLD_X(p);
            break;
        case LD_Xm:
            PxLD_Xm(p);
            break;
        case LD_Xp:
            PxLD_Xp(p);
            break;
        case LD_Ym:
            PxLD_Ym(p);
            break;
        case LD_Yp:
            PxLD_Yp(p);
            break;
        case LDD_Y:
            PxLDD_Y(p);
            break;
        case LD_Zm:
            PxLD_Zm(p);
            break;
        case LD_Zp:
            PxLD_Zp(p);
            break;
        case LDD_Z:
            PxLDD_Z(p);
            break;
        case LDI:
            PxLDI(p);
            break;
        case MOV:
            PxMOV(p);
            break;
        case MOVW:
            PxMOVW(p);
            break;
        case NOP:
            PxNOP(p);
            break;
        case OR:
            PxOR(p);
            break;
        case ORI:
            PxORI(p);
            break;
        case OUT:
            PxOUT(p);
            break;
        case POP:
            PxPOP(p);
            break;
        case PUSH:
            PxPUSH(p);
            break;
        case RCALL:
            PxRCALL(p);
            break;
        case RET:
            PxRET(p);
            break;
        case RJMP:
            PxRJMP(p);
            break;
        case SBC:
            PxSBC(p);
            break;
        case SBCI:
            PxSBCI(p);
            break;
        case SBIW:
            PxSBIW(p);
            break;
        case STD_Z:
            PxSTD_Z(p);
            break;
        default:
            printf("EXEC: Not implemented\n");
            PxBREAK(p);
            return;
    }

    p->state = FETCH;
    return;
}

/*
 * Updates the program counter to a new value
 */
void processor_pc_update(struct processor* p, uint16_t pc){
    /* mask out bits above 10 */
    /* 0000 0111 1111 1111 */
    p->pc = pc & 0x07FF;
    return;
}

/*
 * Increments the program counter by value
 */
void processor_pc_increment(struct processor* p, int v){
    p->pc += v;
    if (p->pc > 0x07FF){
        p->pc = p->pc - 0x07FF;
    }
    return;
}

/*
 * Reads the processor stack pointer
 */
uint16_t processor_sp_read(struct processor *p){
    return datamem_read_io_SP(&p->dmem);
}

/*
 * Updates the stack pointer
 */
void processor_sp_update(struct processor *p, uint16_t sp){
    /* mask out bits above 8 for the 45 */
    /* 0000 0001 1111 1111 */
    sp = sp & 0x01FF;
    datamem_write_io_SP(&p->dmem, sp);
    return;
}

void processor_sp_decrement(struct processor *p, int v){
    uint16_t sp = datamem_read_io_SP(&p->dmem);
    sp -= v;
    datamem_write_io_SP(&p->dmem, sp);
    return;
}

void processor_sp_increment(struct processor *p, int v){
    uint16_t sp = datamem_read_io_SP(&p->dmem);
    sp += v;
    datamem_write_io_SP(&p->dmem, sp);
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
    op_get_reg_direct_2(&p->oper, &r, &d);

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
    Z = (R == 0) ? 1 : 0;
    C = (bit(Rd,7) & bit(Rr,7)) | (bit(Rr,7) & ~bit(R,7)) | (~bit(R,7) & bit(Rd,7));

    datamem_write_io_bit(&p->dmem, SREG, SREG_H, H);
    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);
    datamem_write_io_bit(&p->dmem, SREG, SREG_C, C);

    processor_pc_increment(p, 1);

    return;
}


/*----------------------------------*/
/* ADIW 1001 | 0110 | KKdd | KKKK   */
/*  --> KKdd | KKKK | 1001 | 0110   */
/* d - target register              */
/* K - immediate                    */
/* d = 24,26,28,30                  */
/*----------------------------------*/
void PxADIW(struct processor* p){
    uint8_t d;
    uint16_t Rd;
    uint16_t R;
    uint8_t K;
    int S, V, N, Z, C;

    op_get_reg16_imm(&p->oper, &d, &K);

    Rd = datamem_read_reg16(&p->dmem, d, d+1);

    R = Rd + K;

    datamem_write_reg16(&p->dmem, d, d+1, R);

    V = ~bit(Rd, 15) & bit(R, 15);
    N = bit(R,15);
    S = N ^ V;
    Z = (R == 0) ? 1 : 0;
    C = ~bit(R,15) & bit(Rd,15);

    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);
    datamem_write_io_bit(&p->dmem, SREG, SREG_C, C);

    processor_pc_increment(p, 1);

    return;
}


/*---------------------------------*/
/* AND 0000 | 00rd | dddd | rrrr   */
/* --> dddd | rrrr | 0000 | 00rd   */
/* d - destination                 */
/* r - source                      */
/*---------------------------------*/
void PxAND(struct processor* p){
    uint8_t r;
    uint8_t d;
    uint8_t R;
    uint8_t Rr;
    uint8_t Rd;
    int S, V, N, Z;

    /* Isolate r and d */
    op_get_reg_direct_2(&p->oper, &r, &d);

    /* Get values of r and d */
    Rr = datamem_read_reg(&p->dmem, r);
    Rd = datamem_read_reg(&p->dmem, d);

    R = Rd & Rr;

    /* Set new value of Rd */
    datamem_write_reg(&p->dmem, d, R);

    /* Set SREG flags */

    V = 0;
    N = bit(R,7);
    S = N ^ V;
    Z = (R == 0) ? 1 : 0;

    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);

    processor_pc_increment(p, 1);

    return;
}


/*----------------------------------*/
/* ANDI 0100 | KKKK | dddd | KKKK   */
/*  --> dddd | KKKK | 0100 | KKKK   */
/* d - target register              */
/* K - immediate                    */
/* 16 <= d <= 31                    */
/*----------------------------------*/
void PxANDI(struct processor* p){
    uint8_t d;
    uint8_t Rd;
    uint8_t R;
    uint8_t K;
    int S, V, N, Z;

    op_get_reg_imm(&p->oper, &d, &K);

    Rd = datamem_read_reg(&p->dmem, d);

    R = Rd & K;

    datamem_write_reg(&p->dmem, d, R);

    V = 0;
    N = bit(R,7);
    S = N ^ V;
    Z = (R == 0) ? 1 : 0;

    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);

    processor_pc_increment(p, 1);

    return;
}


/*----------------------------------*/
/* BCLR 1001 | 0100 | 1sss | 1000   */
/*  --> 1sss | 1000 | 1001 | 0100   */
/* s - sreg                         */
/*----------------------------------*/
void PxBCLR(struct processor* p){
    int s;

    op_get_sreg(&p->oper, &s);

    datamem_write_io_bit(&p->dmem, SREG, s, 0);

    processor_pc_increment(p, 1);

    return;
}


/*---------------------------------*/
/* BRBC 1111 | 01kk | kkkk | ksss  */
/*  --> kkkk | ksss | 1111 | 01kk  */
/* k = rel addr                    */
/* s = sreg bit                    */
/*---------------------------------*/
void PxBRBC(struct processor* p){
    int8_t k;
    int s;
    int sreg_bit;

    op_get_rel_addr_sreg(&p->oper, &k, &s);

    sreg_bit = datamem_read_io_bit(&p->dmem, SREG, s);

    if (sreg_bit == 0){
        processor_pc_increment(p, k + 1);
    } else {
        processor_pc_increment(p,1);
    }

    return;
}


/*---------------------------------*/
/* BRBC 1111 | 00kk | kkkk | ksss  */
/*  --> kkkk | ksss | 1111 | 00kk  */
/* k = rel addr                    */
/* s = sreg bit                    */
/*---------------------------------*/
void PxBRBS(struct processor* p){
    int8_t k;
    int s;
    int sreg_bit;

    op_get_rel_addr_sreg(&p->oper, &k, &s);

    sreg_bit = datamem_read_io_bit(&p->dmem, SREG, s);

    if (sreg_bit == 1){
        processor_pc_increment(p, k + 1);
    } else {
        processor_pc_increment(p,1);
    }
    return;
}

/*---------------------------------*/
/* BREAK 1001 | 0101 | 1001 | 1000 */
/* --> 1001 | 1000 | 1001 | 0101   */
/*---------------------------------*/
void PxBREAK(struct processor* p){
    /* BREAK */

    p->state = HALT;

    processor_pc_increment(p, 1);

    return;
}


/*----------------------------------*/
/* BSET 1001 | 0100 | 0sss | 1000   */
/*  --> 0sss | 1000 | 1001 | 0100   */
/* s - sreg                         */
/*----------------------------------*/
void PxBSET(struct processor* p){
    int s;

    op_get_sreg(&p->oper, &s);

    datamem_write_io_bit(&p->dmem, SREG, s, 1);

    processor_pc_increment(p, 1);

    return;
}


/*---------------------------------*/
/* COM 1001 | 010d | dddd | 0000   */
/* --> dddd | 0000 | 1001 | 010d   */
/* d - target register             */
/*---------------------------------*/
void PxCOM(struct processor* p){
    uint8_t d;
    uint8_t Rd;
    uint8_t R;
    int S, V, N, Z, C;

    op_get_reg_direct(&p->oper, &d);

    Rd = datamem_read_reg(&p->dmem, d);

    R = ~Rd;

    datamem_write_reg(&p->dmem, d, R);

    V = 0;
    N = bit(R,7);
    S = N ^ V;
    Z = (R == 0) ? 1 : 0;
    C = 1;

    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);
    datamem_write_io_bit(&p->dmem, SREG, SREG_C, C);

    processor_pc_increment(p, 1);

    return;
}


/*---------------------------------*/
/*  CP 0001 | 01rd | dddd | rrrr   */
/* --> dddd | rrrr | 0001 | 01rd   */
/* d - destination                 */
/* r - source                      */
/*---------------------------------*/
void PxCP(struct processor* p){
    uint8_t r = 0;
    uint8_t d = 0;
    uint8_t R;
    uint8_t Rr;
    uint8_t Rd;
    int H, S, V, N, Z, C;

    /* Isolate r and d */
    op_get_reg_direct_2(&p->oper, &r, &d);

    /* Get values of r and d */
    Rr = datamem_read_reg(&p->dmem, r);
    Rd = datamem_read_reg(&p->dmem, d);

    /* CP */
    R = Rd - Rr;

    /* Set SREG flags */

    H = (~bit(Rd,3) & bit(Rr,3)) | (bit(Rr,3) & bit(R,3)) | (bit(R,3) & ~bit(Rd,3));
    V = (bit(Rd,7) & ~bit(Rr,7) & ~bit(R,7)) | (~bit(Rd,7) & bit(Rr,7) & bit(R,7));
    N = bit(R,7);
    S = N ^ V;
    Z = (R == 0) ? 1 : 0;
    C = (~bit(Rd,7) & bit(Rr,7)) | (bit(Rr,7) & bit(R,7)) | (bit(R,7) & ~bit(Rd,7));

    datamem_write_io_bit(&p->dmem, SREG, SREG_H, H);
    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);
    datamem_write_io_bit(&p->dmem, SREG, SREG_C, C);

    processor_pc_increment(p, 1);

    return;
}


/*---------------------------------*/
/* CPC 0000 | 01rd | dddd | rrrr   */
/* --> dddd | rrrr | 0000 | 01rd   */
/* d - destination                 */
/* r - source                      */
/*---------------------------------*/
void PxCPC(struct processor* p){
    uint8_t r = 0;
    uint8_t d = 0;
    uint8_t R;
    uint8_t Rr;
    uint8_t Rd;
    int Z_prev, C_prev;
    int H, S, V, N, Z, C;

    /* Isolate r and d */
    op_get_reg_direct_2(&p->oper, &r, &d);

    /* Get values of r and d */
    Rr = datamem_read_reg(&p->dmem, r);
    Rd = datamem_read_reg(&p->dmem, d);

    /* Get the values of Z and C */
    Z_prev = datamem_read_io_bit(&p->dmem, SREG, SREG_Z);
    C_prev = datamem_read_io_bit(&p->dmem, SREG, SREG_C);

    /* CPC */
    R = Rd - Rr - C_prev;

    /* Set SREG flags */

    H = (~bit(Rd,3) & bit(Rr,3)) | (bit(Rr,3) & bit(R,3)) | (bit(R,3) & ~bit(Rd,3));
    V = (bit(Rd,7) & ~bit(Rr,7) & ~bit(R,7)) | (~bit(Rd,7) & bit(Rr,7) & bit(R,7));
    N = bit(R,7);
    S = N ^ V;
    Z = ((R == 0) & Z_prev) ? 1 : 0;
    C = (~bit(Rd,7) & bit(Rr,7)) | (bit(Rr,7) & bit(R,7)) | (bit(R,7) & ~bit(Rd,7));

    datamem_write_io_bit(&p->dmem, SREG, SREG_H, H);
    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);
    datamem_write_io_bit(&p->dmem, SREG, SREG_C, C);

    processor_pc_increment(p, 1);

    return;
}


/*---------------------------------*/
/* DEC 1001 | 010d | dddd | 1011   */
/* --> dddd | 1011 | 1001 | 010d   */
/* d - target register             */
/*---------------------------------*/
void PxDEC(struct processor* p){
    uint8_t d;
    uint8_t Rd;
    uint8_t R;
    int S, V, N, Z;

    op_get_reg_direct(&p->oper, &d);

    Rd = datamem_read_reg(&p->dmem, d);

    R = Rd - 1;

    datamem_write_reg(&p->dmem, d, R);

    V = (R == 0x7F) ? 1 : 0;
    N = bit(R,7);
    S = N ^ V;
    Z = (R == 0) ? 1 : 0;

    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);

    processor_pc_increment(p, 1);

    return;
}


/*---------------------------------*/
/* EOR 0010 | 01rd | dddd | rrrr   */
/* --> dddd | rrrr | 0010 | 01rd   */
/* d - destination                 */
/* r - source                      */
/*---------------------------------*/
void PxEOR(struct processor* p){
    uint8_t r = 0;
    uint8_t d = 0;
    uint8_t R;
    uint8_t Rr;
    uint8_t Rd;
    int S, V, N, Z;

    /* Isolate r and d */
    op_get_reg_direct_2(&p->oper, &r, &d);

    /* Get values of r and d */
    Rr = datamem_read_reg(&p->dmem, r);
    Rd = datamem_read_reg(&p->dmem, d);

    /* EOR */
    R = Rd ^ Rr;

    /* Set new value of Rd */
    datamem_write_reg(&p->dmem, d, R);

    /* Set SREG flags */

    V = 0;
    N = bit(R,7);
    S = N ^ V;
    Z = (R == 0) ? 1 : 0;

    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);

    processor_pc_increment(p, 1);

    return;
}


/*---------------------------------*/
/*  IN 1011 | 0aad | dddd | aaaa   */
/* --> dddd | aaaa | 1011 | 0aad   */
/* d - destination                 */
/* a - source in io space          */
/*---------------------------------*/
void PxIN(struct processor* p){
    uint8_t d;
    uint8_t a;
    uint8_t IOa;

    /* Isolate a and d */
    op_get_io_direct(&p->oper, &a, &d);

    IOa = datamem_read_io(&p->dmem, a);

    datamem_write_reg(&p->dmem, d, IOa);

    processor_pc_increment(p, 1);

    return;
}


/*---------------------------------*/
/* INC 1001 | 010d | dddd | 0011   */
/* --> dddd | 0011 | 1001 | 010d   */
/* d - target register             */
/*---------------------------------*/
void PxINC(struct processor* p){
    uint8_t d;
    uint8_t Rd;
    uint8_t R;
    int S, V, N, Z;

    op_get_reg_direct(&p->oper, &d);

    Rd = datamem_read_reg(&p->dmem, d);

    R = Rd + 1;

    datamem_write_reg(&p->dmem, d, R);

    V = (R == 0x80) ? 1 : 0;
    N = bit(R,7);
    S = N ^ V;
    Z = (R == 0) ? 1 : 0;

    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);

    processor_pc_increment(p, 1);

    return;
}


/*----------------------------------*/
/* LD_X 1001 | 000d | dddd | 1100   */
/*  --> dddd | 1100 | 1001 | 000d   */
/* d - Destination register         */
/*----------------------------------*/
void PxLD_X(struct processor* p){
    uint8_t d;
    uint16_t x;
    uint8_t dataX;

    op_get_reg_direct(&p->oper, &d);

    x = datamem_read_reg_X(&p->dmem);

    dataX = datamem_read_addr(&p->dmem, ZERO_OFFSET, x);

    datamem_write_reg(&p->dmem, d, dataX);

    return;
}


/*-----------------------------------*/
/* LD_Xm 1001 | 000d | dddd | 1110   */
/*   --> dddd | 1110 | 1001 | 000d   */
/* d - Destination register          */
/*-----------------------------------*/
void PxLD_Xm(struct processor* p){
    uint8_t d;
    uint16_t x;
    uint8_t dataXm;

    op_get_reg_direct(&p->oper, &d);

    x = datamem_read_reg_X(&p->dmem);

    x = x - 1;

    datamem_write_reg_X(&p->dmem, x );

    dataXm = datamem_read_addr(&p->dmem, ZERO_OFFSET, x);

    datamem_write_reg(&p->dmem, d, dataXm);

    return;
}


/*-----------------------------------*/
/* LD_Xp 1001 | 000d | dddd | 1101   */
/*   --> dddd | 1101 | 1001 | 000d   */
/* d - Destination register          */
/*-----------------------------------*/
void PxLD_Xp(struct processor* p){
    uint8_t d;
    uint16_t x;
    uint8_t dataXp;

    op_get_reg_direct(&p->oper, &d);

    x = datamem_read_reg_X(&p->dmem);

    dataXp = datamem_read_addr(&p->dmem, ZERO_OFFSET, x);

    datamem_write_reg(&p->dmem, d, dataXp);

    x = x + 1;

    datamem_write_reg_X(&p->dmem, x );


    return;
}


/*-----------------------------------*/
/* LD_Ym 1001 | 000d | dddd | 1010   */
/*   --> dddd | 1010 | 1001 | 000d   */
/* d - Destination register          */
/*-----------------------------------*/
void PxLD_Ym(struct processor* p){
    uint8_t d;
    uint16_t y;
    uint8_t dataYm;

    op_get_reg_direct(&p->oper, &d);

    y = datamem_read_reg_Y(&p->dmem);

    y = y - 1;

    datamem_write_reg_Y(&p->dmem, y );

    dataYm = datamem_read_addr(&p->dmem, ZERO_OFFSET, y);

    datamem_write_reg(&p->dmem, d, dataYm);

    return;
}


/*-----------------------------------*/
/* LD_Yp 1001 | 000d | dddd | 1001   */
/*   --> dddd | 1001 | 1001 | 000d   */
/* d - Destination register          */
/*-----------------------------------*/
void PxLD_Yp(struct processor* p){
    uint8_t d;
    uint16_t y;
    uint8_t dataYp;

    op_get_reg_direct(&p->oper, &d);

    y = datamem_read_reg_Y(&p->dmem);

    dataYp = datamem_read_addr(&p->dmem, ZERO_OFFSET, y);

    datamem_write_reg(&p->dmem, d, dataYp);

    y = y + 1;

    datamem_write_reg_Y(&p->dmem, y );

    return;
}


/*-----------------------------------*/
/* LDD_Y 10q0 | qq1d | dddd | 1qqq   */
/*   --> dddd | 1qqq | 10q0 | qq1d   */
/* d - Destination register          */
/* q - displacement                  */
/* Also captures LD_Y q=0            */
/*-----------------------------------*/
void PxLDD_Y(struct processor* p){
    uint8_t d;
    uint8_t q;
    uint16_t y;
    uint8_t dataYQ;

    /* Isolate d and q*/
    op_get_reg_displacement(&p->oper, &d, &q);

    y = datamem_read_reg_Y(&p->dmem);
    dataYQ = datamem_read_addr(&p->dmem, ZERO_OFFSET, y + q);

    datamem_write_reg(&p->dmem, d, dataYQ);

    processor_pc_increment(p, 1);

    return;
}


/*-----------------------------------*/
/* LD_Zm 1001 | 000d | dddd | 0010   */
/*   --> dddd | 0010 | 1001 | 000d   */
/* d - Destination register          */
/*-----------------------------------*/
void PxLD_Zm(struct processor* p){
    uint8_t d;
    uint16_t z;
    uint8_t dataZm;

    op_get_reg_direct(&p->oper, &d);

    z = datamem_read_reg_Z(&p->dmem);

    z = z - 1;

    datamem_write_reg_Z(&p->dmem, z );

    dataZm = datamem_read_addr(&p->dmem, ZERO_OFFSET, z);

    datamem_write_reg(&p->dmem, d, dataZm);

    return;
}


/*-----------------------------------*/
/* LD_Zp 1001 | 000d | dddd | 0001   */
/*   --> dddd | 0001 | 1001 | 000d   */
/* d - Destination register          */
/*-----------------------------------*/
void PxLD_Zp(struct processor* p){
    uint8_t d;
    uint16_t z;
    uint8_t dataZp;

    op_get_reg_direct(&p->oper, &d);

    z = datamem_read_reg_Z(&p->dmem);

    dataZp = datamem_read_addr(&p->dmem, ZERO_OFFSET, z);

    datamem_write_reg(&p->dmem, d, dataZp);

    z = z + 1;

    datamem_write_reg_Z(&p->dmem, z );

    return;
}


/*-----------------------------------*/
/* LDD_Z 10q0 | qq1d | dddd | 0qqq   */
/*   --> dddd | 0qqq | 10q0 | qq1d   */
/* d - Destination register          */
/* q - displacement                  */
/* Also captures LD_Z q=0            */
/*-----------------------------------*/
void PxLDD_Z(struct processor* p){
    uint8_t d;
    uint8_t q;
    uint16_t z;
    uint8_t dataZQ;

    /* Isolate d and q*/
    op_get_reg_displacement(&p->oper, &d, &q);

    z = datamem_read_reg_Z(&p->dmem);
    dataZQ = datamem_read_addr(&p->dmem, ZERO_OFFSET, z + q);

    datamem_write_reg(&p->dmem, d, dataZQ);

    processor_pc_increment(p, 1);

    return;
}

/*---------------------------------*/
/* LDI 1110 | kkkk | dddd | kkkk   */
/* --> dddd | kkkk | 1110 | kkkk   */
/* d - destination                 */
/* k - immediate                   */
/*---------------------------------*/
void PxLDI(struct processor* p){
    uint8_t d;
    uint8_t k;

    op_get_reg_imm(&p->oper, &d, &k);

    datamem_write_reg(&p->dmem, d, k);

    processor_pc_increment(p, 1);

    return;
}


/*---------------------------------*/
/* MOV 0010 | 11rd | dddd | rrrr   */
/* --> dddd | rrrr | 0010 | 11rd   */
/* d - destination                 */
/* r - source                      */
/*---------------------------------*/
void PxMOV(struct processor* p){
    uint8_t r;
    uint8_t d;
    uint8_t Rr;

    /* Isolate r and d */
    op_get_reg_direct_2(&p->oper, &r, &d);

    Rr = datamem_read_reg(&p->dmem, r);

    datamem_write_reg(&p->dmem, d, Rr);

    processor_pc_increment(p, 1);

    return;
}


/*----------------------------------*/
/* MOVW 0000 | 0001 | dddd | rrrr   */
/*  --> dddd | rrrr | 0000 | 0001   */
/* d - destination                  */
/* r - source                       */
/* Rd+1:Rd <- Rr+1:Rr               */
/*----------------------------------*/
void PxMOVW(struct processor* p){
    uint8_t r;
    uint8_t d;
    uint16_t Rr16;

    /* Isolate r and d */
    r = (( p->oper.bits & 0x0F00 ) >> 8 );
    d = (( p->oper.bits & 0xF000 ) >> 12 );

    Rr16 = datamem_read_reg16(&p->dmem, r, r+1);

    datamem_write_reg16(&p->dmem, d, d+1, Rr16);

    processor_pc_increment(p, 1);

    return;
}


/*---------------------------------*/
/* NOP 0000 | 0000 | 0000 | 0000   */
/* --> 0000 | 0000 | 0000 | 0000   */
/*---------------------------------*/
void PxNOP(struct processor* p){
    /* NOP */

    processor_pc_increment(p, 1);

    return;
}


/*---------------------------------*/
/*  OR 0010 | 10rd | dddd | rrrr   */
/* --> dddd | rrrr | 0010 | 10rd   */
/* d - destination                 */
/* r - source                      */
/*---------------------------------*/
void PxOR(struct processor* p){
    uint8_t r;
    uint8_t d;
    uint8_t R;
    uint8_t Rr;
    uint8_t Rd;
    int S, V, N, Z;

    /* Isolate r and d */
    op_get_reg_direct_2(&p->oper, &r, &d);

    /* Get values of r and d */
    Rr = datamem_read_reg(&p->dmem, r);
    Rd = datamem_read_reg(&p->dmem, d);

    R = Rd | Rr;

    /* Set new value of Rd */
    datamem_write_reg(&p->dmem, d, R);

    /* Set SREG flags */

    V = 0;
    N = bit(R,7);
    S = N ^ V;
    Z = (R == 0) ? 1 : 0;

    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);

    processor_pc_increment(p, 1);

    return;
}


/*----------------------------------*/
/*  ORI 0110 | KKKK | dddd | KKKK   */
/*  --> dddd | KKKK | 0110 | KKKK   */
/* d - target register              */
/* K - immediate                    */
/* 16 <= d <= 31                    */
/*----------------------------------*/
void PxORI(struct processor* p){
    uint8_t d;
    uint8_t Rd;
    uint8_t R;
    uint8_t K;
    int S, V, N, Z;

    op_get_reg_imm(&p->oper, &d, &K);

    Rd = datamem_read_reg(&p->dmem, d);

    R = Rd | K;

    datamem_write_reg(&p->dmem, d, R);

    V = 0;
    N = bit(R,7);
    S = N ^ V;
    Z = (R == 0) ? 1 : 0;

    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);

    processor_pc_increment(p, 1);

    return;
}

/*---------------------------------*/
/* OUT 1011 | 1aar | rrrr | aaaa   */
/* --> rrrr | aaaa | 1011 | 1aar   */
/* r - source                      */
/* a - dest in io space            */
/*---------------------------------*/
void PxOUT(struct processor* p){
    uint8_t r;
    uint8_t a;
    uint8_t Rr;

    /* Isolate a and r */
    /* maps to A and d of in */
    op_get_io_direct(&p->oper, &a, &r);

    Rr = datamem_read_reg(&p->dmem, r);

    datamem_write_io(&p->dmem, a, Rr);

    processor_pc_increment(p, 1);

    return;
}


/*----------------------------------*/
/*  POP 1001 | 000d | dddd | 1111   */
/*  --> dddd | 1111 | 1001 | 000d   */
/* d - dest                         */
/*----------------------------------*/
void PxPOP(struct processor* p){
    uint8_t d;
    uint8_t stack;
    uint16_t stack_addr;

    /* Isolate d */
    op_get_reg_direct(&p->oper, &d);

    stack_addr = processor_sp_read(p);

    stack = datamem_read_addr(&p->dmem, ZERO_OFFSET, stack_addr);

    datamem_write_reg(&p->dmem, stack_addr, stack);

    processor_pc_increment(p, 1);
    processor_sp_increment(p, 1);

    return;
}


/*----------------------------------*/
/* PUSH 1001 | 001r | rrrr | 1111   */
/*  --> rrrr | 1111 | 1001 | 001r   */
/* r - source                       */
/*----------------------------------*/
void PxPUSH(struct processor* p){
    uint8_t r;
    uint8_t Rr;
    uint16_t stack_addr;

    /* Isolate r */
    op_get_reg_direct(&p->oper, &r);

    Rr = datamem_read_reg(&p->dmem, r);

    stack_addr = processor_sp_read(p);

    datamem_write_addr(&p->dmem, ZERO_OFFSET, stack_addr, Rr);

    processor_pc_increment(p, 1);
    processor_sp_decrement(p, 1);

    return;
}


/*-----------------------------------*/
/* RCALL 1100 | kkkk | kkkk | kkkk   */
/*   --> kkkk | kkkk | 1100 | kkkk   */
/* k - rel address                   */
/*-----------------------------------*/
void PxRCALL(struct processor* p){
    int16_t k_signed;
    uint16_t stack_addr;

    op_get_rel_addr(&p->oper, &k_signed);

    /* STACK <- PC + 1 */

    stack_addr = processor_sp_read(p);

    datamem_write_addr(&p->dmem, ZERO_OFFSET, stack_addr, p->pc + 1);

    /* SP <- SP - 2 */

    processor_sp_decrement(p, 2);

    /* PC <- PC + K + 1 */

    processor_pc_increment(p, k_signed + 1);

    return;
}


/*---------------------------------*/
/* RET 1001 | 0101 | 0000 | 1000   */
/* --> 0000 | 1000 | 1001 | 0101   */
/*---------------------------------*/
void PxRET(struct processor* p){
    /* RET */
    uint16_t sp;
    uint8_t stack;

    processor_sp_increment(p, 2);

    sp = processor_sp_read(p);

    stack = datamem_read_addr(&p->dmem, ZERO_OFFSET, sp);

    processor_pc_update(p, stack);

    return;
}


/*----------------------------------*/
/* RJMP 1100 | kkkk | kkkk | kkkk   */
/*  --> kkkk | kkkk | 1100 | kkkk   */
/* k - rel address                  */
/*----------------------------------*/
void PxRJMP(struct processor* p){
    int16_t k_signed;

    op_get_rel_addr(&p->oper, &k_signed);

    processor_pc_increment(p, k_signed + 1);

    return;
}


/*---------------------------------*/
/* SBC 0000 | 10rd | dddd | rrrr   */
/* --> dddd | rrrr | 0000 | 10rd   */
/* d - destination                 */
/* r - source                      */
/*---------------------------------*/
void PxSBC(struct processor* p){
    uint8_t r = 0;
    uint8_t d = 0;
    uint8_t R;
    uint8_t Rr;
    uint8_t Rd;
    int C_flag, Z_flag;
    int H, S, V, N, Z, C;

    /* Isolate r and d */
    op_get_reg_direct_2(&p->oper, &r, &d);

    /* Get values of r and d */
    Rr = datamem_read_reg(&p->dmem, r);
    Rd = datamem_read_reg(&p->dmem, d);

    /* Get C and Z Flags */
    C_flag = datamem_read_io_bit(&p->dmem, SREG, SREG_C);
    Z_flag = datamem_read_io_bit(&p->dmem, SREG, SREG_Z);

    R = Rd - Rr - C_flag;

    /* Set new value of Rd */
    datamem_write_reg(&p->dmem, d, R);

    /* Set SREG flags */

    H = (~bit(Rd,3) & bit(Rr,3)) | (bit(Rr,3) & bit(R,3)) | (bit(R,3) & ~bit(Rd,3));
    V = (bit(Rd,7) & ~bit(Rr,7) & bit(R,7)) | (~bit(Rd,7) & bit(Rr,7) & bit(R,7));
    N = bit(R,7);
    S = N ^ V;
    Z = ((R == 0) | Z_flag) ? 1 : 0;
    C = (~bit(Rd,7) & bit(Rr,7)) | (bit(Rr,7) & bit(R,7)) | (bit(R,7) & ~bit(Rd,7));

    datamem_write_io_bit(&p->dmem, SREG, SREG_H, H);
    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);
    datamem_write_io_bit(&p->dmem, SREG, SREG_C, C);

    processor_pc_increment(p, 1);

    return;
}


/*----------------------------------*/
/* SBCI 0100 | KKKK | dddd | KKKK   */
/*  --> dddd | KKKK | 0100 | KKKK   */
/* d - target register              */
/* K - immediate                    */
/* 16 <= d <= 31                    */
/*----------------------------------*/
void PxSBCI(struct processor* p){
    uint8_t d;
    uint8_t Rd;
    uint8_t R;
    uint8_t K;
    int C_flag, Z_flag;
    int H, S, V, N, Z, C;

    op_get_reg_imm(&p->oper, &d, &K);

    Rd = datamem_read_reg(&p->dmem, d);

    C_flag = datamem_read_io_bit(&p->dmem, SREG, SREG_C);
    Z_flag = datamem_read_io_bit(&p->dmem, SREG, SREG_Z);

    R = Rd - K - C_flag;

    datamem_write_reg(&p->dmem, d, R);

    H = (~bit(Rd,3) & bit(K,3)) | (bit(K,3) & bit(R,3)) | (bit(R,3) & ~bit(Rd,3));
    V = (bit(Rd,7) & ~bit(K,7) & ~bit(R,7)) | (~bit(Rd,7) & bit(K,7) & bit(R,7));
    N = bit(R,7);
    S = N ^ V;
    Z = ((R == 0) | Z_flag) ? 1 : 0;
    C = (~bit(Rd,7) & bit(K,7)) | (bit(K,7) & bit(R,7)) | (bit(R,7) & ~bit(Rd,7));

    datamem_write_io_bit(&p->dmem, SREG, SREG_H, H);
    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);
    datamem_write_io_bit(&p->dmem, SREG, SREG_C, C);

    processor_pc_increment(p, 1);

    return;
}


/*----------------------------------*/
/* SBIW 1001 | 0111 | KKdd | KKKK   */
/*  --> KKdd | KKKK | 1001 | 0111   */
/* d - target register              */
/* K - immediate                    */
/* d = 24,26,28,30                  */
/*----------------------------------*/
void PxSBIW(struct processor* p){
    uint8_t d;
    uint16_t Rd;
    uint16_t R;
    uint8_t K;
    int S, V, N, Z, C;

    op_get_reg16_imm(&p->oper, &d, &K);

    Rd = datamem_read_reg16(&p->dmem, d, d+1);

    R = Rd - K;

    datamem_write_reg16(&p->dmem, d, d+1, R);

    V = bit(Rd, 15) & ~bit(R, 15);
    N = bit(R,15);
    S = N ^ V;
    Z = (R == 0) ? 1 : 0;
    C = bit(R,15) & ~bit(Rd,15);

    datamem_write_io_bit(&p->dmem, SREG, SREG_V, V);
    datamem_write_io_bit(&p->dmem, SREG, SREG_N, N);
    datamem_write_io_bit(&p->dmem, SREG, SREG_S, S);
    datamem_write_io_bit(&p->dmem, SREG, SREG_Z, Z);
    datamem_write_io_bit(&p->dmem, SREG, SREG_C, C);

    processor_pc_increment(p, 1);

    return;
}

/*-----------------------------------*/
/* STD_Z 10q0 | qq1r | rrrr | 0qqq   */
/*   --> rrrr | 0qqq | 10q0 | qq1r   */
/* r - Source register               */
/* q - displacement                  */
/* Also captures ST_Z q=0            */
/*-----------------------------------*/
void PxSTD_Z(struct processor* p){
    uint8_t r;
    uint8_t Rr;
    uint8_t q;
    uint16_t z;

    /* Isolate r and q*/
    r = (( p->oper.bits & 0xF000 ) >> 12 ) | (( p->oper.bits & 0x1 ) << 4 );
    q = (( p->oper.bits & 0x700 ) >> 8 ) | (( p->oper.bits & 0xc ) << 1) | (( p->oper.bits & 0x20 ));

    Rr = datamem_read_reg(&p->dmem, r);
    z = datamem_read_reg_Z(&p->dmem);

    /* printf("r, q, z: %X, %X, %X\n",r, q, z); */

    datamem_write_addr(&p->dmem, ZERO_OFFSET, z + q, Rr);

    processor_pc_increment(p, 1);

    return;
}
