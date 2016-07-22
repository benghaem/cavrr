#include "operation.h"
#include <stdio.h>

/*---------------------------------*/
/* op_get_reg_direct               */
/*     ---- | ---d | dddd | ----   */
/* --> dddd | ---- | ---- | ---d   */
/*---------------------------------*/
void op_get_reg_direct(struct operation* op, uint8_t* d){
    uint16_t bits = op->bits;
    *d = (( bits & 0xF000 ) >> 12 ) | (( bits & 0x1 ) << 4);
}

/*---------------------------------*/
/* op_get_reg_direct_2             */
/*     ---- | --rd | dddd | rrrr   */
/* --> dddd | rrrr | ---- | --rd   */
/*---------------------------------*/
void op_get_reg_direct_2(struct operation* op, uint8_t* r, uint8_t* d){
    uint16_t bits = op->bits;
    *r = (( bits & 0x0F00 ) >> 8 ) | ((( bits >> 1 ) & 0x1 ) << 4);
    *d = (( bits & 0xF000 ) >> 12 ) | (( bits & 0x1 ) << 4);
}


/*---------------------------------*/
/* op_get_reg_imm                  */
/*     ---- | KKKK | dddd | KKKK   */
/* --> dddd | KKKK | ---- | KKKK   */
/* 16<= d <= 31                    */
/*---------------------------------*/
void op_get_reg_imm(struct operation* op, uint8_t* d, uint8_t* K){
    uint16_t bits = op->bits;
    *K = (( bits & 0x0F00 ) >> 8 ) | (( bits  & 0x000F ) << 4);
    *d = (( bits & 0xF000 ) >> 12 ) | (0x10);
    /* note that the above line offsets d by 16 */
}


/*---------------------------------*/
/* op_get_io_direct                */
/*     ---- | -AAd | dddd | AAAA   */
/* --> dddd | AAAA | ---- | --Ad   */
/*---------------------------------*/
void op_get_io_direct(struct operation* op, uint8_t* A, uint8_t* d){
    uint16_t bits = op->bits;
    *A = (( bits & 0x0F00 ) >> 8 ) | ((( bits >> 1 ) & 0x3 ) << 4);
    *d = (( bits & 0xF000 ) >> 12 ) | (( bits & 0x1 ) << 4);
}


/*---------------------------------*/
/* op_get_rel_addr               */
/*     ---- | KKKK | KKKK | KKKK   */
/* --> KKKK | KKKK | ---- | KKKK   */
/*---------------------------------*/
void op_get_rel_addr(struct operation* op, int16_t* K){
    uint16_t bits = op->bits;
    uint16_t kloc;

    kloc = (( bits & 0xFF00 ) >> 8 ) | (( bits & 0x000F ) << 8);

    /* Convert from 12bit 2's complement to signed int */

    if (kloc >> 11 & 0x1){
        *K = -1 * (((~kloc) & 0x7ff) + 1);
    } else {
        *K = kloc;
    }
}

void disassemble_to_str(struct operation* op, uint16_t pc, char* str, size_t max_len){
    int16_t K;
    uint8_t imm;
    uint8_t d;
    uint8_t r;
    uint8_t A;

    switch (op->inst){
        case COM:
        case DEC:
        case INC:
        case NEG:
        case PUSH:
            op_get_reg_direct(op, &r);
            snprintf(str, max_len, "%s r%d", instruction_str(op->inst), r);
            break;
        case ADD:
        case AND:
        case CP:
        case CPC:
        case EOR:
        case SUB:
            op_get_reg_direct_2(op, &r, &d);
            snprintf(str, max_len, "%s r%d, r%d", instruction_str(op->inst), d, r);
            break;
        case IN:
            op_get_io_direct(op, &A, &d);
            snprintf(str, max_len, "%s r%d, 0x%X", instruction_str(op->inst), d, A);
            break;
        case OUT:
            op_get_io_direct(op, &A, &r);
            snprintf(str, max_len, "%s 0x%X, r%d", instruction_str(op->inst), A, r);
            break;
        case ANDI:
        case CPI:
        case LDI:
            op_get_reg_imm(op, &d, &imm);
            snprintf(str, max_len, "%s r%d, 0x%X", instruction_str(op->inst), d, imm);
            break;
        case RJMP:
        case RCALL:
            op_get_rel_addr(op, &K);
            snprintf(str, max_len, "%s .%+d ... %X", instruction_str(op->inst), K * 2, ((int)pc + K)*2 + 2);
            break;
        case NOP:
            snprintf(str, max_len, "%s", instruction_str(op->inst));
            break;
        default:
            snprintf(str, max_len, "no dasm");
            break;
    }
}

