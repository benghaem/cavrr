#include "operation.h"
#include "IOREG.h"
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
/* op_get_reg16_imm                */
/*     ---- | ---- | KKdd | KKKK   */
/* --> KKdd | KKKK | ---- | ----   */
/*---------------------------------*/
void op_get_reg16_imm(struct operation* op, uint8_t* d, uint8_t* K){
    uint16_t bits = op->bits;
    *K = (( bits & 0x0F00 ) >> 8 ) | (( bits  & 0xC000 ) >> 10 );
    /* 11000 -> 24 and 11110 -> 30 */
    *d = (( bits & 0x3000 ) >> 11 ) | (0x18);
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
/* op_get_rel_addr                 */
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


/*---------------------------------*/
/* op_get_rel_addr_sreg            */
/*     ---- | --kk | kkkk | ksss   */
/* --> kkkk | ksss | ---- | --kk   */
/*---------------------------------*/
void op_get_rel_addr_sreg(struct operation* op, int8_t* k, int* s){
    uint16_t bits = op->bits;
    uint8_t kloc;

    kloc = (( bits & 0xF800 ) >> 11 ) | (( bits & 0x0003 ) << 5);
    *s = ((bits & 0x0700) >> 8);


    /* Convert from 7bit 2's complement to signed int */

    if (kloc >> 6 & 0x1){
        *k = -1 * (((~kloc) & 0x3F) + 1);
    } else {
        *k = kloc;
    }
}

/*
 * Returns a branch instruction string as listed by the assembly spec
 * The normal instruction_str in instruction.h does not do this
 * because the only "true" branch instructions are BRBC and BRBS
 * and the remainder simply use the value of the s variable in the opcode
 * to differentiate from one another.
 *
 * The one option "use_high_low" returns BRSH instead of BRCC and
 * BRLO instead of BRCS. This is useful if you expect the disassembly in
 * a specific form. The instructions are equivalent
 */
char* nice_branch_instr_str(struct operation *op, int use_high_low){
    int8_t k;
    int s;
    op_get_rel_addr_sreg(op, &k, &s);
    if ( op->inst == BRBC ){
        switch (s){
            case SREG_I:
                return "BRID";
            case SREG_T:
                return "BRTC";
            case SREG_H:
                return "BRHC";
            case SREG_S:
                return "BRGE";
            case SREG_V:
                return "BRVC";
            case SREG_N:
                return "BRPL";
            case SREG_Z:
                return "BRNE";
            case SREG_C:
                if (use_high_low){
                    return "BRSH";
                }
                return "BRCC";
            default:
                return "UNKNOWN";
        }
    } else if ( op->inst == BRBS ){
        switch (s){
            case SREG_I:
                return "BRIE";
            case SREG_T:
                return "BRTS";
            case SREG_H:
                return "BRHS";
            case SREG_S:
                return "BRLT";
            case SREG_V:
                return "BRVS";
            case SREG_N:
                return "BRMI";
            case SREG_Z:
                return "BREQ";
            case SREG_C:
                if (use_high_low){
                    return "BRLO";
                }
                return "BRCS";
            default:
                return "UNKNOWN";
        }
    } else {
        return "NOT BRANCH";
    }
}


/*
 * Disassembles an operation into AT&T format as used by avr-gcc
 * and the like.
 *
 * Writes to an already allocated string with snprintf
 * Currently takes no configuration options
 */
void disassemble_to_str(struct operation* op, uint16_t pc, char* str, size_t max_len){
    int br_use_hl = 0;

    int16_t K;
    int8_t k;
    uint8_t imm;
    uint8_t d;
    uint8_t r;
    uint8_t A;
    int s;

    switch (op->inst){
        case COM:
        case DEC:
        case INC:
        case NEG:
        case PUSH:
        case SBC:
            op_get_reg_direct(op, &r);
            snprintf(str, max_len, "%s r%d", instruction_str(op->inst), r);
            break;
        case ADD:
        case AND:
        case CP:
        case CPC:
        case EOR:
        case OR:
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
        case ORI:
        case SBCI:
            op_get_reg_imm(op, &d, &imm);
            snprintf(str, max_len, "%s r%d, 0x%X", instruction_str(op->inst), d, imm);
            break;
        case RJMP:
        case RCALL:
            op_get_rel_addr(op, &K);
            snprintf(str, max_len, "%s .%+d ... %X", instruction_str(op->inst), K * 2, ((int)pc + K + 1)*2);
            break;
        case BRBS:
        case BRBC:
            op_get_rel_addr_sreg(op, &k, &s);
            snprintf(str, max_len, "%s .%+d ... %X", nice_branch_instr_str(op, br_use_hl), k * 2, ((int)pc + k + 1)*2 );
            break;
        case ADIW:
        case SBIW:
            op_get_reg16_imm(op, &d, &imm);
            snprintf(str,max_len, "%s r%d, 0x%X", instruction_str(op->inst), d, imm);
            break;
        case NOP:
        case RET:
            snprintf(str, max_len, "%s", instruction_str(op->inst));
            break;
        default:
            snprintf(str, max_len, "no dasm");
            break;
    }
}
