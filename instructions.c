#include "instructions.h"
#include "bitutil.h"
#include <stdio.h>

INSTRUCTION bytes_to_instruction(uint16_t bytes){
    INSTRUCTION instr = UNKNOWN;
    //little endian inputs
    uint8_t low = (bytes & 0xFF00) >> 8;
    uint8_t high = bytes & 0xFF;

    // high part 0 -> 0xF0
    // high part 1 -> 0x0F
    // low  part 2 -> 0xF0
    // low  part 3 -> 0x0F

    if(bit_test(high, 0x0, 0xD0)){
        if(bit_test(high, 0x0, 0xF0)){
            if(bit_test(high, 0x0, 0x0C)){
                if(bit_test(high, 0x0, 0x0F)){
                    instr = NOP;
                }
                else if(bit_test(high,0x1,0x0F)){
                    instr = MOVW;
                }
                else if(bit_test(high,0x3,0x0F)){
                    if(bit_test(low,0x0,0x10)){
                        if(bit_test(low,0x0,0x01)){
                            instr = MULSU;
                        }
                        else if(bit_test(low,0x1,0x01)){
                            instr = FMUL;
                        }
                    }
                    else if(bit_test(low,0x1,0x10)){
                        if(bit_test(low,0x0,0x01)){
                            instr = FMULS;
                        }
                        else if(bit_test(low,0x1,0x01)){
                            instr = FMULSU;
                        }
                    }
                }
            }
            else if(bit_test(high, 0x1, 0x0C)){
                instr = CPC;
            }
            else if(bit_test(high, 0x2, 0x0C)){
                instr = SBC;
            }
            else if(bit_test(high, 0x3, 0x0C)){
                //also captures LSL
                //LSL is a case of ADD
                instr = ADD;
            }
        } else if(bit_test(high, 0x2, 0xF0)){
            if (bit_test(high, 0x0, 0x0F)){
                // also captures TST
                instr = AND;
            }
            else if(bit_test(high,0x1,0x0F)){
                // also captures CLR
                instr = EOR;
            }
            else if(bit_test(high,0x2,0x0F)){
                instr = OR;
            }
            else if(bit_test(high,0x3,0x0F)){
                instr = MOV;
            }
        }
    }

    return instr;
}

void INSTRUCTION_print(INSTRUCTION instr){
    switch(instr){
        case ADD:
            printf("ADD");
            break;
        case AND:
            printf("AND");
            break;
        case CPC:
            printf("CPC");
            break;
        case EOR:
            printf("EOR");
            break;
        case FMUL:
            printf("FMUL");
            break;
        case FMULS:
            printf("FMULS");
            break;
        case FMULSU:
            printf("FMULSU");
            break;
        case MULSU:
            printf("MULSU");
            break;
        case MOV:
            printf("MOV");
            break;
        case MOVW:
            printf("MOVW");
            break;
        case NOP:
            printf("NOP");
            break;
        case OR:
            printf("OR");
            break;
        case SBC:
            printf("SBC");
            break;
        case UNKNOWN:
            printf("UNKNOWN");
            break;
    }
}
