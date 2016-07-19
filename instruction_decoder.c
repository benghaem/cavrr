#include "instruction.h"
#include "util/bitutil.h"
#include <stdio.h>

/*
 * Decodes 16bits in little endian format
 * to an instruction
 *
 * This function is auto-generated and attempts to
 * efficiently subset groups of instructions by bit
 */
enum instruction instruction_decode_bytes(uint16_t bytes){
    enum instruction instr = UNKNOWN;
    /* little endian inputs */
    uint8_t low = (bytes & 0xFF00) >> 8;
    uint8_t high = bytes & 0xFF;

    /*
    high part 0 -> 0xF0
    high part 1 -> 0x0F
    low  part 2 -> 0xF0
    low  part 3 -> 0x0F
    */
    /* Generated on Jul 18, 2016 @ 15:25 by genInstrSelect.py */
    if(bit_test(high, 0x0, 0xd0)){
    /*000 on bits: (0, 1, 3) from op part: 0
    --> ['ADD', 'AND', 'CLR', 'CPC', 'EOR', 'LSL', 'MOV', 'MOVW', 'NOP', 'OR', 'SBC', 'TST']*/
        if(bit_test(high, 0x0, 0xf0)){
        /*0000 on bits: (0, 1, 2, 3) from op part: 0
        --> ['ADD', 'CPC', 'LSL', 'MOVW', 'NOP', 'SBC']*/
            if(bit_test(high, 0x0, 0xc)){
            /*00 on bits: (0, 1) from op part: 1
            --> ['MOVW', 'NOP']*/
                if(bit_test(high, 0x0, 0xf)){
                /*0000 on bits: (0, 1, 2, 3) from op part: 1
                --> ['NOP']*/
                instr = NOP;
                return instr;
                }
                if(bit_test(high, 0x1, 0xf)){
                /*0001 on bits: (0, 1, 2, 3) from op part: 1
                --> ['MOVW']*/
                instr = MOVW;
                return instr;
                }
            }
            if(bit_test(high, 0x4, 0xc)){
            /*01 on bits: (0, 1) from op part: 1
            --> ['CPC']*/
            instr = CPC;
            return instr;
            }
            if(bit_test(high, 0x8, 0xc)){
            /*10 on bits: (0, 1) from op part: 1
            --> ['SBC']*/
            instr = SBC;
            return instr;
            }
            if(bit_test(high, 0xc, 0xc)){
            /*11 on bits: (0, 1) from op part: 1
            --> ['ADD', 'LSL']*/
            /* NOTE: Instruction codes equal */
            instr = ADD;
            return instr;
            }
        }
        if(bit_test(high, 0x20, 0xf0)){
        /*0010 on bits: (0, 1, 2, 3) from op part: 0
        --> ['AND', 'CLR', 'EOR', 'MOV', 'OR', 'TST']*/
            if(bit_test(high, 0x0, 0xc)){
            /*00 on bits: (0, 1) from op part: 1
            --> ['AND', 'TST']*/
            /* NOTE: Instruction codes equal */
            instr = AND;
            return instr;
            }
            if(bit_test(high, 0x4, 0xc)){
            /*01 on bits: (0, 1) from op part: 1
            --> ['CLR', 'EOR']*/
            /* NOTE: Instruction codes equal */
            instr = EOR;
            return instr;
            }
            if(bit_test(high, 0x8, 0xc)){
            /*10 on bits: (0, 1) from op part: 1
            --> ['OR']*/
            instr = OR;
            return instr;
            }
            if(bit_test(high, 0xc, 0xc)){
            /*11 on bits: (0, 1) from op part: 1
            --> ['MOV']*/
            instr = MOV;
            return instr;
            }
        }
    }
    if(bit_test(high, 0x10, 0xd0)){
    /*001 on bits: (0, 1, 3) from op part: 0
    --> ['ADC', 'CP', 'CPI', 'CPSE', 'ROL', 'SUB']*/
        if(bit_test(high, 0x10, 0xf0)){
        /*0001 on bits: (0, 1, 2, 3) from op part: 0
        --> ['ADC', 'CP', 'CPSE', 'ROL', 'SUB']*/
            if(bit_test(high, 0x0, 0xc)){
            /*00 on bits: (0, 1) from op part: 1
            --> ['CPSE']*/
            instr = CPSE;
            return instr;
            }
            if(bit_test(high, 0x4, 0xc)){
            /*01 on bits: (0, 1) from op part: 1
            --> ['CP']*/
            instr = CP;
            return instr;
            }
            if(bit_test(high, 0x8, 0xc)){
            /*10 on bits: (0, 1) from op part: 1
            --> ['SUB']*/
            instr = SUB;
            return instr;
            }
            if(bit_test(high, 0xc, 0xc)){
            /*11 on bits: (0, 1) from op part: 1
            --> ['ADC', 'ROL']*/
            /* NOTE: Instruction codes equal */
            instr = ROL;
            return instr;
            }
        }
        if(bit_test(high, 0x30, 0xf0)){
        /*0011 on bits: (0, 1, 2, 3) from op part: 0
        --> ['CPI']*/
        instr = CPI;
        return instr;
        }
    }
    if(bit_test(high, 0x40, 0xd0)){
    /*010 on bits: (0, 1, 3) from op part: 0
    --> ['ORI', 'SBCI', 'SBR']*/
        if(bit_test(high, 0x40, 0xf0)){
        /*0100 on bits: (0, 1, 2, 3) from op part: 0
        --> ['SBCI']*/
        instr = SBCI;
        return instr;
        }
        if(bit_test(high, 0x60, 0xf0)){
        /*0110 on bits: (0, 1, 2, 3) from op part: 0
        --> ['ORI', 'SBR']*/
        /* NOTE: Instruction codes equal */
        instr = ORI;
        return instr;
        }
    }
    if(bit_test(high, 0x50, 0xd0)){
    /*011 on bits: (0, 1, 3) from op part: 0
    --> ['ANDI', 'CBR', 'SUBI']*/
        if(bit_test(high, 0x50, 0xf0)){
        /*0101 on bits: (0, 1, 2, 3) from op part: 0
        --> ['SUBI']*/
        instr = SUBI;
        return instr;
        }
        if(bit_test(high, 0x70, 0xf0)){
        /*0111 on bits: (0, 1, 2, 3) from op part: 0
        --> ['ANDI', 'CBR']*/
        /* NOTE: Instruction codes equal */
        instr = ANDI;
        return instr;
        }
    }
    if(bit_test(high, 0x80, 0xd0)){
    /*100 on bits: (0, 1, 3) from op part: 0
    --> ['LD_Y', 'LDD_Y', 'LD_Z', 'LDD_Z', 'ST_Y', 'STD_Y', 'ST_Z', 'STD_Z']*/
        if(bit_test(high, 0x0, 0x2)){
        /*0 on bits: (2,) from op part: 1
        --> ['LD_Y', 'LDD_Y', 'LD_Z', 'LDD_Z']*/
            if(bit_test(low, 0x0, 0x8)){
            /*0 on bits: (0,) from op part: 3
            --> ['LD_Z', 'LDD_Z']*/
            instr = LDD_Z;
            return instr;
            }
            if(bit_test(low, 0x8, 0x8)){
            /*1 on bits: (0,) from op part: 3
            --> ['LD_Y', 'LDD_Y']*/
            instr = LDD_Y;
            return instr;
            }
        }
        if(bit_test(high, 0x2, 0x2)){
        /*1 on bits: (2,) from op part: 1
        --> ['ST_Y', 'STD_Y', 'ST_Z', 'STD_Z']*/
            if(bit_test(low, 0x0, 0x8)){
            /*0 on bits: (0,) from op part: 3
            --> ['ST_Z', 'STD_Z']*/
            instr = STD_Z;
            return instr;
            }
            if(bit_test(low, 0x8, 0x8)){
            /*1 on bits: (0,) from op part: 3
            --> ['ST_Y', 'STD_Y']*/
            instr = STD_Y;
            return instr;
            }
        }
    }
    if(bit_test(high, 0x90, 0xd0)){
    /*101 on bits: (0, 1, 3) from op part: 0
    --> ['ADIW', 'ASR', 'BCLR', 'BREAK', 'BSET', 'CBI', 'CLC', 'CLH', 'CLI', 'CLN', 'CLS', 'CLT', 'CLV', 'CLZ', 'COM', 'DEC', 'ICALL', 'IJMP', 'IN', 'INC', 'LD_X', 'LD_Xp', 'LD_Xm', 'LD_Yp', 'LD_Ym', 'LD_Zp', 'LD_Zm', 'LDS', 'LPM', 'LPM_Z', 'LPM_Zp', 'LSR', 'NEG', 'OUT', 'POP', 'PUSH', 'RET', 'RETI', 'ROR', 'SBI', 'SBIC', 'SBIS', 'SBIW', 'SEC', 'SEH', 'SEI', 'SEN', 'SES', 'SET', 'SEV', 'SEZ', 'SLEEP', 'SPM', 'ST_X', 'ST_Xp', 'ST_Xm', 'ST_Yp', 'ST_Ym', 'ST_Zp', 'ST_Zm', 'STS', 'SWAP', 'WDR']*/
        if(bit_test(high, 0x90, 0xf0)){
        /*1001 on bits: (0, 1, 2, 3) from op part: 0
        --> ['ADIW', 'ASR', 'BCLR', 'BREAK', 'BSET', 'CBI', 'CLC', 'CLH', 'CLI', 'CLN', 'CLS', 'CLT', 'CLV', 'CLZ', 'COM', 'DEC', 'ICALL', 'IJMP', 'INC', 'LD_X', 'LD_Xp', 'LD_Xm', 'LD_Yp', 'LD_Ym', 'LD_Zp', 'LD_Zm', 'LDS', 'LPM', 'LPM_Z', 'LPM_Zp', 'LSR', 'NEG', 'POP', 'PUSH', 'RET', 'RETI', 'ROR', 'SBI', 'SBIC', 'SBIS', 'SBIW', 'SEC', 'SEH', 'SEI', 'SEN', 'SES', 'SET', 'SEV', 'SEZ', 'SLEEP', 'SPM', 'ST_X', 'ST_Xp', 'ST_Xm', 'ST_Yp', 'ST_Ym', 'ST_Zp', 'ST_Zm', 'STS', 'SWAP', 'WDR']*/
            if(bit_test(high, 0x0, 0xe)){
            /*000 on bits: (0, 1, 2) from op part: 1
            --> ['LD_X', 'LD_Xp', 'LD_Xm', 'LD_Yp', 'LD_Ym', 'LD_Zp', 'LD_Zm', 'LDS', 'LPM_Z', 'LPM_Zp', 'POP']*/
                if(bit_test(low, 0x0, 0xf)){
                /*0000 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LDS']*/
                instr = LDS;
                return instr;
                }
                if(bit_test(low, 0x1, 0xf)){
                /*0001 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LD_Zp']*/
                instr = LD_Zp;
                return instr;
                }
                if(bit_test(low, 0x2, 0xf)){
                /*0010 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LD_Zm']*/
                instr = LD_Zm;
                return instr;
                }
                if(bit_test(low, 0x4, 0xf)){
                /*0100 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LPM_Z']*/
                instr = LPM_Z;
                return instr;
                }
                if(bit_test(low, 0x5, 0xf)){
                /*0101 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LPM_Zp']*/
                instr = LPM_Zp;
                return instr;
                }
                if(bit_test(low, 0x9, 0xf)){
                /*1001 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LD_Yp']*/
                instr = LD_Yp;
                return instr;
                }
                if(bit_test(low, 0xa, 0xf)){
                /*1010 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LD_Ym']*/
                instr = LD_Ym;
                return instr;
                }
                if(bit_test(low, 0xc, 0xf)){
                /*1100 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LD_X']*/
                instr = LD_X;
                return instr;
                }
                if(bit_test(low, 0xd, 0xf)){
                /*1101 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LD_Xp']*/
                instr = LD_Xp;
                return instr;
                }
                if(bit_test(low, 0xe, 0xf)){
                /*1110 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LD_Xm']*/
                instr = LD_Xm;
                return instr;
                }
                if(bit_test(low, 0xf, 0xf)){
                /*1111 on bits: (0, 1, 2, 3) from op part: 3
                --> ['POP']*/
                instr = POP;
                return instr;
                }
            }
            if(bit_test(high, 0x2, 0xe)){
            /*001 on bits: (0, 1, 2) from op part: 1
            --> ['PUSH', 'ST_X', 'ST_Xp', 'ST_Xm', 'ST_Yp', 'ST_Ym', 'ST_Zp', 'ST_Zm', 'STS']*/
                if(bit_test(low, 0x0, 0xf)){
                /*0000 on bits: (0, 1, 2, 3) from op part: 3
                --> ['STS']*/
                instr = STS;
                return instr;
                }
                if(bit_test(low, 0x1, 0xf)){
                /*0001 on bits: (0, 1, 2, 3) from op part: 3
                --> ['ST_Zp']*/
                instr = ST_Zp;
                return instr;
                }
                if(bit_test(low, 0x2, 0xf)){
                /*0010 on bits: (0, 1, 2, 3) from op part: 3
                --> ['ST_Zm']*/
                instr = ST_Zm;
                return instr;
                }
                if(bit_test(low, 0x9, 0xf)){
                /*1001 on bits: (0, 1, 2, 3) from op part: 3
                --> ['ST_Yp']*/
                instr = ST_Yp;
                return instr;
                }
                if(bit_test(low, 0xa, 0xf)){
                /*1010 on bits: (0, 1, 2, 3) from op part: 3
                --> ['ST_Ym']*/
                instr = ST_Ym;
                return instr;
                }
                if(bit_test(low, 0xc, 0xf)){
                /*1100 on bits: (0, 1, 2, 3) from op part: 3
                --> ['ST_X']*/
                instr = ST_X;
                return instr;
                }
                if(bit_test(low, 0xd, 0xf)){
                /*1101 on bits: (0, 1, 2, 3) from op part: 3
                --> ['ST_Xp']*/
                instr = ST_Xp;
                return instr;
                }
                if(bit_test(low, 0xe, 0xf)){
                /*1110 on bits: (0, 1, 2, 3) from op part: 3
                --> ['ST_Xm']*/
                instr = ST_Xm;
                return instr;
                }
                if(bit_test(low, 0xf, 0xf)){
                /*1111 on bits: (0, 1, 2, 3) from op part: 3
                --> ['PUSH']*/
                instr = PUSH;
                return instr;
                }
            }
            if(bit_test(high, 0x4, 0xe)){
            /*010 on bits: (0, 1, 2) from op part: 1
            --> ['ASR', 'BCLR', 'BREAK', 'BSET', 'CLC', 'CLH', 'CLI', 'CLN', 'CLS', 'CLT', 'CLV', 'CLZ', 'COM', 'DEC', 'ICALL', 'IJMP', 'INC', 'LPM', 'LSR', 'NEG', 'RET', 'RETI', 'ROR', 'SEC', 'SEH', 'SEI', 'SEN', 'SES', 'SET', 'SEV', 'SEZ', 'SLEEP', 'SPM', 'SWAP', 'WDR']*/
                if(bit_test(low, 0x0, 0xf)){
                /*0000 on bits: (0, 1, 2, 3) from op part: 3
                --> ['COM']*/
                instr = COM;
                return instr;
                }
                if(bit_test(low, 0x1, 0xf)){
                /*0001 on bits: (0, 1, 2, 3) from op part: 3
                --> ['NEG']*/
                instr = NEG;
                return instr;
                }
                if(bit_test(low, 0x2, 0xf)){
                /*0010 on bits: (0, 1, 2, 3) from op part: 3
                --> ['SWAP']*/
                instr = SWAP;
                return instr;
                }
                if(bit_test(low, 0x3, 0xf)){
                /*0011 on bits: (0, 1, 2, 3) from op part: 3
                --> ['INC']*/
                instr = INC;
                return instr;
                }
                if(bit_test(low, 0x5, 0xf)){
                /*0101 on bits: (0, 1, 2, 3) from op part: 3
                --> ['ASR']*/
                instr = ASR;
                return instr;
                }
                if(bit_test(low, 0x6, 0xf)){
                /*0110 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LSR']*/
                instr = LSR;
                return instr;
                }
                if(bit_test(low, 0x7, 0xf)){
                /*0111 on bits: (0, 1, 2, 3) from op part: 3
                --> ['ROR']*/
                instr = ROR;
                return instr;
                }
                if(bit_test(low, 0x8, 0xf)){
                /*1000 on bits: (0, 1, 2, 3) from op part: 3
                --> ['BCLR', 'BREAK', 'BSET', 'CLC', 'CLH', 'CLI', 'CLN', 'CLS', 'CLT', 'CLV', 'CLZ', 'LPM', 'RET', 'RETI', 'SEC', 'SEH', 'SEI', 'SEN', 'SES', 'SET', 'SEV', 'SEZ', 'SLEEP', 'SPM', 'WDR']*/
                    if(bit_test(high, 0x4, 0xf)){
                    /*0100 on bits: (0, 1, 2, 3) from op part: 1
                    --> ['BCLR', 'BSET', 'CLC', 'CLH', 'CLI', 'CLN', 'CLS', 'CLT', 'CLV', 'CLZ', 'SEC', 'SEH', 'SEI', 'SEN', 'SES', 'SET', 'SEV', 'SEZ']*/
                        if(bit_test(low, 0x0, 0x80)){
                        /*0 on bits: (0,) from op part: 2
                        --> ['BSET', 'SEC', 'SEH', 'SEI', 'SEN', 'SES', 'SET', 'SEV', 'SEZ']*/
                        instr = BSET;
                        return instr;
                        }
                        if(bit_test(low, 0x80, 0x80)){
                        /*1 on bits: (0,) from op part: 2
                        --> ['BCLR', 'CLC', 'CLH', 'CLI', 'CLN', 'CLS', 'CLT', 'CLV', 'CLZ']*/
                        instr = BCLR;
                        return instr;
                        }
                    }
                    if(bit_test(high, 0x5, 0xf)){
                    /*0101 on bits: (0, 1, 2, 3) from op part: 1
                    --> ['BREAK', 'LPM', 'RET', 'RETI', 'SLEEP', 'SPM', 'WDR']*/
                        if(bit_test(low, 0x0, 0xf0)){
                        /*0000 on bits: (0, 1, 2, 3) from op part: 2
                        --> ['RET']*/
                        instr = RET;
                        return instr;
                        }
                        if(bit_test(low, 0x10, 0xf0)){
                        /*0001 on bits: (0, 1, 2, 3) from op part: 2
                        --> ['RETI']*/
                        instr = RETI;
                        return instr;
                        }
                        if(bit_test(low, 0x80, 0xf0)){
                        /*1000 on bits: (0, 1, 2, 3) from op part: 2
                        --> ['SLEEP']*/
                        instr = SLEEP;
                        return instr;
                        }
                        if(bit_test(low, 0x90, 0xf0)){
                        /*1001 on bits: (0, 1, 2, 3) from op part: 2
                        --> ['BREAK']*/
                        instr = BREAK;
                        return instr;
                        }
                        if(bit_test(low, 0xa0, 0xf0)){
                        /*1010 on bits: (0, 1, 2, 3) from op part: 2
                        --> ['WDR']*/
                        instr = WDR;
                        return instr;
                        }
                        if(bit_test(low, 0xc0, 0xf0)){
                        /*1100 on bits: (0, 1, 2, 3) from op part: 2
                        --> ['LPM']*/
                        instr = LPM;
                        return instr;
                        }
                        if(bit_test(low, 0xe0, 0xf0)){
                        /*1110 on bits: (0, 1, 2, 3) from op part: 2
                        --> ['SPM']*/
                        instr = SPM;
                        return instr;
                        }
                    }
                }
                if(bit_test(low, 0x9, 0xf)){
                /*1001 on bits: (0, 1, 2, 3) from op part: 3
                --> ['ICALL', 'IJMP']*/
                    if(bit_test(high, 0x4, 0xf)){
                    /*0100 on bits: (0, 1, 2, 3) from op part: 1
                    --> ['IJMP']*/
                    instr = IJMP;
                    return instr;
                    }
                    if(bit_test(high, 0x5, 0xf)){
                    /*0101 on bits: (0, 1, 2, 3) from op part: 1
                    --> ['ICALL']*/
                    instr = ICALL;
                    return instr;
                    }
                }
                if(bit_test(low, 0xa, 0xf)){
                /*1010 on bits: (0, 1, 2, 3) from op part: 3
                --> ['DEC']*/
                instr = DEC;
                return instr;
                }
            }
            if(bit_test(high, 0x6, 0xe)){
            /*011 on bits: (0, 1, 2) from op part: 1
            --> ['ADIW', 'SBIW']*/
                if(bit_test(high, 0x6, 0xf)){
                /*0110 on bits: (0, 1, 2, 3) from op part: 1
                --> ['ADIW']*/
                instr = ADIW;
                return instr;
                }
                if(bit_test(high, 0x7, 0xf)){
                /*0111 on bits: (0, 1, 2, 3) from op part: 1
                --> ['SBIW']*/
                instr = SBIW;
                return instr;
                }
            }
            if(bit_test(high, 0x8, 0xe)){
            /*100 on bits: (0, 1, 2) from op part: 1
            --> ['CBI', 'SBIC']*/
                if(bit_test(high, 0x8, 0xf)){
                /*1000 on bits: (0, 1, 2, 3) from op part: 1
                --> ['CBI']*/
                instr = CBI;
                return instr;
                }
                if(bit_test(high, 0x9, 0xf)){
                /*1001 on bits: (0, 1, 2, 3) from op part: 1
                --> ['SBIC']*/
                instr = SBIC;
                return instr;
                }
            }
            if(bit_test(high, 0xa, 0xe)){
            /*101 on bits: (0, 1, 2) from op part: 1
            --> ['SBI', 'SBIS']*/
                if(bit_test(high, 0xa, 0xf)){
                /*1010 on bits: (0, 1, 2, 3) from op part: 1
                --> ['SBI']*/
                instr = SBI;
                return instr;
                }
                if(bit_test(high, 0xb, 0xf)){
                /*1011 on bits: (0, 1, 2, 3) from op part: 1
                --> ['SBIS']*/
                instr = SBIS;
                return instr;
                }
            }
        }
        if(bit_test(high, 0xb0, 0xf0)){
        /*1011 on bits: (0, 1, 2, 3) from op part: 0
        --> ['IN', 'OUT']*/
            if(bit_test(high, 0x0, 0x8)){
            /*0 on bits: (0,) from op part: 1
            --> ['IN']*/
            instr = IN;
            return instr;
            }
            if(bit_test(high, 0x8, 0x8)){
            /*1 on bits: (0,) from op part: 1
            --> ['OUT']*/
            instr = OUT;
            return instr;
            }
        }
    }
    if(bit_test(high, 0xc0, 0xd0)){
    /*110 on bits: (0, 1, 3) from op part: 0
    --> ['LDI', 'RJMP', 'SER']*/
        if(bit_test(high, 0xc0, 0xf0)){
        /*1100 on bits: (0, 1, 2, 3) from op part: 0
        --> ['RJMP']*/
        instr = RJMP;
        return instr;
        }
        if(bit_test(high, 0xe0, 0xf0)){
        /*1110 on bits: (0, 1, 2, 3) from op part: 0
        --> ['LDI', 'SER']*/
        instr = LDI;
        return instr;
        }
    }
    if(bit_test(high, 0xd0, 0xd0)){
    /*111 on bits: (0, 1, 3) from op part: 0
    --> ['BLD', 'BRBC', 'BRBS', 'BRCC', 'BRCS', 'BREQ', 'BRGE', 'BRHC', 'BRHS', 'BRID', 'BRIE', 'BRLO', 'BRLT', 'BRMI', 'BRNE', 'BRPL', 'BRSH', 'BRTC', 'BRTS', 'BRVC', 'BRVS', 'BST', 'RCALL', 'SBRC', 'SBRS']*/
        if(bit_test(high, 0xd0, 0xf0)){
        /*1101 on bits: (0, 1, 2, 3) from op part: 0
        --> ['RCALL']*/
        instr = RCALL;
        return instr;
        }
        if(bit_test(high, 0xf0, 0xf0)){
        /*1111 on bits: (0, 1, 2, 3) from op part: 0
        --> ['BLD', 'BRBC', 'BRBS', 'BRCC', 'BRCS', 'BREQ', 'BRGE', 'BRHC', 'BRHS', 'BRID', 'BRIE', 'BRLO', 'BRLT', 'BRMI', 'BRNE', 'BRPL', 'BRSH', 'BRTC', 'BRTS', 'BRVC', 'BRVS', 'BST', 'SBRC', 'SBRS']*/
            if(bit_test(high, 0x0, 0xc)){
            /*00 on bits: (0, 1) from op part: 1
            --> ['BRBS', 'BRCS', 'BREQ', 'BRHS', 'BRIE', 'BRLO', 'BRLT', 'BRMI', 'BRTS', 'BRVS']*/
            instr = BRBS;
            return instr;
            }
            if(bit_test(high, 0x4, 0xc)){
            /*01 on bits: (0, 1) from op part: 1
            --> ['BRBC', 'BRCC', 'BRGE', 'BRHC', 'BRID', 'BRNE', 'BRPL', 'BRSH', 'BRTC', 'BRVC']*/
            instr = BRBC;
            return instr;
            }
            if(bit_test(high, 0x8, 0xc)){
            /*10 on bits: (0, 1) from op part: 1
            --> ['BLD', 'BST']*/
                if(bit_test(high, 0x8, 0xe)){
                /*100 on bits: (0, 1, 2) from op part: 1
                --> ['BLD']*/
                instr = BLD;
                return instr;
                }
                if(bit_test(high, 0xa, 0xe)){
                /*101 on bits: (0, 1, 2) from op part: 1
                --> ['BST']*/
                instr = BST;
                return instr;
                }
            }
            if(bit_test(high, 0xc, 0xc)){
            /*11 on bits: (0, 1) from op part: 1
            --> ['SBRC', 'SBRS']*/
                if(bit_test(high, 0xc, 0xe)){
                /*110 on bits: (0, 1, 2) from op part: 1
                --> ['SBRC']*/
                instr = SBRC;
                return instr;
                }
                if(bit_test(high, 0xe, 0xe)){
                /*111 on bits: (0, 1, 2) from op part: 1
                --> ['SBRS']*/
                instr = SBRS;
                return instr;
                }
            }
        }
    }
    return instr;
}

/*
 * Returns 1 if an instruction is 32b. Used when the processor fetches
 * instructions from program memory.
 */
int instruction_is_32b(enum instruction instr){
    int is_32b = 0;
    switch (instr){
        case STS:
        case LDS:
            is_32b = 1;
            break;
        default:
            break;
    }
    return is_32b;
}
