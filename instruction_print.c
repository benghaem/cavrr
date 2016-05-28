#include "instruction.h"
#include <stdio.h>
void INSTRUCTION_print(INSTRUCTION instr){
    switch(instr){
        case ADC:
            printf("ADC");
            break;
        case ADD:
            printf("ADD");
            break;
        case ADIW:
            printf("ADIW");
            break;
        case AND:
            printf("AND");
            break;
        case ANDI:
            printf("ANDI");
            break;
        case ASR:
            printf("ASR");
            break;
        case BCLR:
            printf("BCLR");
            break;
        case BLD:
            printf("BLD");
            break;
        case BRBC:
            printf("BRBC");
            break;
        case BRBS:
            printf("BRBS");
            break;
        case BREAK:
            printf("BREAK");
            break;
        case BSET:
            printf("BSET");
            break;
        case BST:
            printf("BST");
            break;
        case CALL:
            printf("CALL");
            break;
        case CBI:
            printf("CBI");
            break;
        case COM:
            printf("COM");
            break;
        case CP:
            printf("CP");
            break;
        case CPC:
            printf("CPC");
            break;
        case CPI:
            printf("CPI");
            break;
        case CPSE:
            printf("CPSE");
            break;
        case DEC:
            printf("DEC");
            break;
        case DES:
            printf("DES");
            break;
        case EICALL:
            printf("EICALL");
            break;
        case EIJMP:
            printf("EIJMP");
            break;
        case ELPM1:
            printf("ELPM1");
            break;
        case ELPM2:
            printf("ELPM2");
            break;
        case ELPM3:
            printf("ELPM3");
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
        case ICALL:
            printf("ICALL");
            break;
        case IJMP:
            printf("IJMP");
            break;
        case IN:
            printf("IN");
            break;
        case INC:
            printf("INC");
            break;
        case JMP:
            printf("JMP");
            break;
        case LAC:
            printf("LAC");
            break;
        case LAS:
            printf("LAS");
            break;
        case LAT:
            printf("LAT");
            break;
        case LD1:
            printf("LD1");
            break;
        case LD2:
            printf("LD2");
            break;
        case LD3:
            printf("LD3");
            break;
        case LDD_Y2:
            printf("LDD_Y2");
            break;
        case LDD_Y3:
            printf("LDD_Y3");
            break;
        case LDD_Z2:
            printf("LDD_Z2");
            break;
        case LDD_Z3:
            printf("LDD_Z3");
            break;
        case LDI:
            printf("LDI");
            break;
        case LDS:
            printf("LDS");
            break;
        case LPM1:
            printf("LPM1");
            break;
        case LPM2:
            printf("LPM2");
            break;
        case LPM3:
            printf("LPM3");
            break;
        case LSR:
            printf("LSR");
            break;
        case MOV:
            printf("MOV");
            break;
        case MOVW:
            printf("MOVW");
            break;
        case MUL:
            printf("MUL");
            break;
        case MULSU:
            printf("MULSU");
            break;
        case NEG:
            printf("NEG");
            break;
        case NOP:
            printf("NOP");
            break;
        case OR:
            printf("OR");
            break;
        case ORI:
            printf("ORI");
            break;
        case OUT:
            printf("OUT");
            break;
        case POP:
            printf("POP");
            break;
        case PUSH:
            printf("PUSH");
            break;
        case RCALL:
            printf("RCALL");
            break;
        case RET:
            printf("RET");
            break;
        case RETI:
            printf("RETI");
            break;
        case RJMP:
            printf("RJMP");
            break;
        case ROR:
            printf("ROR");
            break;
        case SBC:
            printf("SBC");
            break;
        case SBCI:
            printf("SBCI");
            break;
        case SBI:
            printf("SBI");
            break;
        case SBIC:
            printf("SBIC");
            break;
        case SBIS:
            printf("SBIS");
            break;
        case SBIW:
            printf("SBIW");
            break;
        case SBRC:
            printf("SBRC");
            break;
        case SBRS:
            printf("SBRS");
            break;
        case SLEEP:
            printf("SLEEP");
            break;
        case SPM:
            printf("SPM");
            break;
        case SPM2_4:
            printf("SPM2_4");
            break;
        case ST1:
            printf("ST1");
            break;
        case ST2:
            printf("ST2");
            break;
        case ST3:
            printf("ST3");
            break;
        case STS:
            printf("STS");
            break;
        case ST_Y2:
            printf("ST_Y2");
            break;
        case ST_Y3:
            printf("ST_Y3");
            break;
        case ST_Z2:
            printf("ST_Z2");
            break;
        case ST_Z3:
            printf("ST_Z3");
            break;
        case SUB:
            printf("SUB");
            break;
        case SUBI:
            printf("SUBI");
            break;
        case SWAP:
            printf("SWAP");
            break;
        case UNKNOWN:
            printf("UNKNOWN");
            break;
        case WDR:
            printf("WDR");
            break;
        case XCH:
            printf("XCH");
            break;
    }
}
