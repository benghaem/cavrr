#include "instruction.h"
#include "util/bitutil.h"
#include <stdio.h>

INSTRUCTION INSTRUCTION_decode_bytes(uint16_t bytes){
    INSTRUCTION instr = UNKNOWN;
    //little endian inputs
    uint8_t low = (bytes & 0xFF00) >> 8;
    uint8_t high = bytes & 0xFF;

    // high part 0 -> 0xF0
    // high part 1 -> 0x0F
    // low  part 2 -> 0xF0
    // low  part 3 -> 0x0F

    // Generated on May 27, 2016 @ 16:21 by genInstrSelect.py
    if(bit_test(high, 0x0, 0xd0)){
    /*000 on bits: (0, 1, 3) from op part: 0
    --> ['ADD', 'AND', 'CLR', 'CPC', 'EOR', 'FMUL', 'FMULS', 'FMULSU', 'LSL', 'MOV', 'MOVW', 'MULSU', 'NOP', 'OR', 'SBC', 'TST']*/
        if(bit_test(high, 0x0, 0xf0)){
        /*0000 on bits: (0, 1, 2, 3) from op part: 0
        --> ['ADD', 'CPC', 'FMUL', 'FMULS', 'FMULSU', 'LSL', 'MOVW', 'MULSU', 'NOP', 'SBC']*/
            if(bit_test(high, 0x0, 0xc)){
            /*00 on bits: (0, 1) from op part: 1
            --> ['FMUL', 'FMULS', 'FMULSU', 'MOVW', 'MULSU', 'NOP']*/
                if(bit_test(high, 0x0, 0xf)){
                /*0000 on bits: (0, 1, 2, 3) from op part: 1
                --> ['NOP']*/
                instr = NOP;
                }
                if(bit_test(high, 0x1, 0xf)){
                /*0001 on bits: (0, 1, 2, 3) from op part: 1
                --> ['MOVW']*/
                instr = MOVW;
                }
                if(bit_test(high, 0x3, 0xf)){
                /*0011 on bits: (0, 1, 2, 3) from op part: 1
                --> ['FMUL', 'FMULS', 'FMULSU', 'MULSU']*/
                    if(bit_test(low, 0x0, 0x80)){
                    /*0 on bits: (0,) from op part: 2
                    --> ['FMUL', 'MULSU']*/
                        if(bit_test(low, 0x0, 0x8)){
                        /*0 on bits: (0,) from op part: 3
                        --> ['MULSU']*/
                        instr = MULSU;
                        }
                        if(bit_test(low, 0x8, 0x8)){
                        /*1 on bits: (0,) from op part: 3
                        --> ['FMUL']*/
                        instr = FMUL;
                        }
                    }
                    if(bit_test(low, 0x80, 0x80)){
                    /*1 on bits: (0,) from op part: 2
                    --> ['FMULS', 'FMULSU']*/
                        if(bit_test(low, 0x0, 0x8)){
                        /*0 on bits: (0,) from op part: 3
                        --> ['FMULS']*/
                        instr = FMULS;
                        }
                        if(bit_test(low, 0x8, 0x8)){
                        /*1 on bits: (0,) from op part: 3
                        --> ['FMULSU']*/
                        instr = FMULSU;
                        }
                    }
                }
            }
            if(bit_test(high, 0x4, 0xc)){
            /*01 on bits: (0, 1) from op part: 1
            --> ['CPC']*/
            instr = CPC;
            }
            if(bit_test(high, 0x8, 0xc)){
            /*10 on bits: (0, 1) from op part: 1
            --> ['SBC']*/
            instr = SBC;
            }
            if(bit_test(high, 0xc, 0xc)){
            /*11 on bits: (0, 1) from op part: 1
            --> ['ADD', 'LSL']*/
            // NOTE: Instruction codes equal
            instr = ADD;
            }
        }
        if(bit_test(high, 0x20, 0xf0)){
        /*0010 on bits: (0, 1, 2, 3) from op part: 0
        --> ['AND', 'CLR', 'EOR', 'MOV', 'OR', 'TST']*/
            if(bit_test(high, 0x0, 0xc)){
            /*00 on bits: (0, 1) from op part: 1
            --> ['AND', 'TST']*/
            // NOTE: Instruction codes equal
            instr = AND;
            }
            if(bit_test(high, 0x4, 0xc)){
            /*01 on bits: (0, 1) from op part: 1
            --> ['CLR', 'EOR']*/
            // NOTE: Instruction codes equal
            instr = EOR;
            }
            if(bit_test(high, 0x8, 0xc)){
            /*10 on bits: (0, 1) from op part: 1
            --> ['OR']*/
            instr = OR;
            }
            if(bit_test(high, 0xc, 0xc)){
            /*11 on bits: (0, 1) from op part: 1
            --> ['MOV']*/
            instr = MOV;
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
            }
            if(bit_test(high, 0x4, 0xc)){
            /*01 on bits: (0, 1) from op part: 1
            --> ['CP']*/
            instr = CP;
            }
            if(bit_test(high, 0x8, 0xc)){
            /*10 on bits: (0, 1) from op part: 1
            --> ['SUB']*/
            instr = SUB;
            }
            if(bit_test(high, 0xc, 0xc)){
            /*11 on bits: (0, 1) from op part: 1
            --> ['ADC', 'ROL']*/
            // NOTE: Instruction codes equal
            instr = ROL;
            }
        }
        if(bit_test(high, 0x30, 0xf0)){
        /*0011 on bits: (0, 1, 2, 3) from op part: 0
        --> ['CPI']*/
        instr = CPI;
        }
    }
    if(bit_test(high, 0x40, 0xd0)){
    /*010 on bits: (0, 1, 3) from op part: 0
    --> ['ORI', 'SBCI', 'SBR']*/
        if(bit_test(high, 0x40, 0xf0)){
        /*0100 on bits: (0, 1, 2, 3) from op part: 0
        --> ['SBCI']*/
        instr = SBCI;
        }
        if(bit_test(high, 0x60, 0xf0)){
        /*0110 on bits: (0, 1, 2, 3) from op part: 0
        --> ['ORI', 'SBR']*/
        // NOTE: Instruction codes equal
        instr = ORI;
        }
    }
    if(bit_test(high, 0x50, 0xd0)){
    /*011 on bits: (0, 1, 3) from op part: 0
    --> ['ANDI', 'CBR', 'SUBI']*/
        if(bit_test(high, 0x50, 0xf0)){
        /*0101 on bits: (0, 1, 2, 3) from op part: 0
        --> ['SUBI']*/
        instr = SUBI;
        }
        if(bit_test(high, 0x70, 0xf0)){
        /*0111 on bits: (0, 1, 2, 3) from op part: 0
        --> ['ANDI', 'CBR']*/
        // NOTE: Instruction codes equal
        instr = ANDI;
        }
    }
    if(bit_test(high, 0x80, 0xd0)){
    /*100 on bits: (0, 1, 3) from op part: 0
    --> ['LDD_Y1', 'LDD_4', 'LDD_Z1', 'LDD_Z4', 'ST_Y1', 'ST_Y4', 'ST_Z1', 'ST_Z4']*/
        if(bit_test(high, 0x0, 0x2)){
        /*0 on bits: (2,) from op part: 1
        --> ['LDD_Y1', 'LDD_4', 'LDD_Z1', 'LDD_Z4']*/
            if(bit_test(low, 0x0, 0x8)){
            /*0 on bits: (0,) from op part: 3
            --> ['LDD_Z1', 'LDD_Z4']*/
            instr = LDD_Z4;
            }
            if(bit_test(low, 0x8, 0x8)){
            /*1 on bits: (0,) from op part: 3
            --> ['LDD_Y1', 'LDD_4']*/
            instr = LDD_4;
            }
        }
        if(bit_test(high, 0x2, 0x2)){
        /*1 on bits: (2,) from op part: 1
        --> ['ST_Y1', 'ST_Y4', 'ST_Z1', 'ST_Z4']*/
            if(bit_test(low, 0x0, 0x8)){
            /*0 on bits: (0,) from op part: 3
            --> ['ST_Z1', 'ST_Z4']*/
            instr = ST_Z4;
            }
            if(bit_test(low, 0x8, 0x8)){
            /*1 on bits: (0,) from op part: 3
            --> ['ST_Y1', 'ST_Y4']*/
            instr = ST_Y4;
            }
        }
    }
    if(bit_test(high, 0x90, 0xd0)){
    /*101 on bits: (0, 1, 3) from op part: 0
    --> ['ADIW', 'ASR', 'BCLR', 'BREAK', 'BSET', 'CALL', 'CBI', 'CLC', 'CLH', 'CLI', 'CLN', 'CLS', 'CLT', 'CLV', 'CLZ', 'COM', 'DEC', 'DES', 'EICALL', 'EIJMP', 'ELPM1', 'ELPM2', 'ELPM3', 'ICALL', 'IJMP', 'IN', 'INC', 'JMP', 'LAC', 'LAS', 'LAT', 'LD1', 'LD2', 'LD3', 'LDD_Y2', 'LDD_Y3', 'LDD_Z2', 'LDD_Z3', 'LDS', 'LPM1', 'LPM2', 'LPM3', 'LSR', 'MUL', 'NEG', 'OUT', 'POP', 'PUSH', 'RET', 'RETI', 'ROR', 'SBI', 'SBIC', 'SBIS', 'SBIW', 'SEC', 'SEH', 'SEI', 'SEN', 'SES', 'SET', 'SEV', 'SEZ', 'SLEEP', 'SPM', 'SPM2', 'SPM2_4', 'ST1', 'ST2', 'ST3', 'ST_Y2', 'ST_Y3', 'ST_Z2', 'ST_Z3', 'STS', 'SWAP', 'WDR', 'XCH']*/
        if(bit_test(high, 0x90, 0xf0)){
        /*1001 on bits: (0, 1, 2, 3) from op part: 0
        --> ['ADIW', 'ASR', 'BCLR', 'BREAK', 'BSET', 'CALL', 'CBI', 'CLC', 'CLH', 'CLI', 'CLN', 'CLS', 'CLT', 'CLV', 'CLZ', 'COM', 'DEC', 'DES', 'EICALL', 'EIJMP', 'ELPM1', 'ELPM2', 'ELPM3', 'ICALL', 'IJMP', 'INC', 'JMP', 'LAC', 'LAS', 'LAT', 'LD1', 'LD2', 'LD3', 'LDD_Y2', 'LDD_Y3', 'LDD_Z2', 'LDD_Z3', 'LDS', 'LPM1', 'LPM2', 'LPM3', 'LSR', 'MUL', 'NEG', 'POP', 'PUSH', 'RET', 'RETI', 'ROR', 'SBI', 'SBIC', 'SBIS', 'SBIW', 'SEC', 'SEH', 'SEI', 'SEN', 'SES', 'SET', 'SEV', 'SEZ', 'SLEEP', 'SPM', 'SPM2', 'SPM2_4', 'ST1', 'ST2', 'ST3', 'ST_Y2', 'ST_Y3', 'ST_Z2', 'ST_Z3', 'STS', 'SWAP', 'WDR', 'XCH']*/
            if(bit_test(high, 0x0, 0xc)){
            /*00 on bits: (0, 1) from op part: 1
            --> ['ELPM2', 'ELPM3', 'LAC', 'LAS', 'LAT', 'LD1', 'LD2', 'LD3', 'LDD_Y2', 'LDD_Y3', 'LDD_Z2', 'LDD_Z3', 'LDS', 'LPM2', 'LPM3', 'POP', 'PUSH', 'ST1', 'ST2', 'ST3', 'ST_Y2', 'ST_Y3', 'ST_Z2', 'ST_Z3', 'STS', 'XCH']*/
                if(bit_test(low, 0x0, 0xf)){
                /*0000 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LDS', 'STS']*/
                    if(bit_test(high, 0x0, 0xe)){
                    /*000 on bits: (0, 1, 2) from op part: 1
                    --> ['LDS']*/
                    instr = LDS;
                    }
                    if(bit_test(high, 0x2, 0xe)){
                    /*001 on bits: (0, 1, 2) from op part: 1
                    --> ['STS']*/
                    instr = STS;
                    }
                }
                if(bit_test(low, 0x1, 0xf)){
                /*0001 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LDD_Z2', 'ST_Z2']*/
                    if(bit_test(high, 0x0, 0xe)){
                    /*000 on bits: (0, 1, 2) from op part: 1
                    --> ['LDD_Z2']*/
                    instr = LDD_Z2;
                    }
                    if(bit_test(high, 0x2, 0xe)){
                    /*001 on bits: (0, 1, 2) from op part: 1
                    --> ['ST_Z2']*/
                    instr = ST_Z2;
                    }
                }
                if(bit_test(low, 0x2, 0xf)){
                /*0010 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LDD_Z3', 'ST_Z3']*/
                    if(bit_test(high, 0x0, 0xe)){
                    /*000 on bits: (0, 1, 2) from op part: 1
                    --> ['LDD_Z3']*/
                    instr = LDD_Z3;
                    }
                    if(bit_test(high, 0x2, 0xe)){
                    /*001 on bits: (0, 1, 2) from op part: 1
                    --> ['ST_Z3']*/
                    instr = ST_Z3;
                    }
                }
                if(bit_test(low, 0x4, 0xf)){
                /*0100 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LPM2', 'XCH']*/
                    if(bit_test(high, 0x0, 0xe)){
                    /*000 on bits: (0, 1, 2) from op part: 1
                    --> ['LPM2']*/
                    instr = LPM2;
                    }
                    if(bit_test(high, 0x2, 0xe)){
                    /*001 on bits: (0, 1, 2) from op part: 1
                    --> ['XCH']*/
                    instr = XCH;
                    }
                }
                if(bit_test(low, 0x5, 0xf)){
                /*0101 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LAS', 'LPM3']*/
                    if(bit_test(high, 0x0, 0xe)){
                    /*000 on bits: (0, 1, 2) from op part: 1
                    --> ['LPM3']*/
                    instr = LPM3;
                    }
                    if(bit_test(high, 0x2, 0xe)){
                    /*001 on bits: (0, 1, 2) from op part: 1
                    --> ['LAS']*/
                    instr = LAS;
                    }
                }
                if(bit_test(low, 0x6, 0xf)){
                /*0110 on bits: (0, 1, 2, 3) from op part: 3
                --> ['ELPM2', 'LAC']*/
                    if(bit_test(high, 0x0, 0xe)){
                    /*000 on bits: (0, 1, 2) from op part: 1
                    --> ['ELPM2']*/
                    instr = ELPM2;
                    }
                    if(bit_test(high, 0x2, 0xe)){
                    /*001 on bits: (0, 1, 2) from op part: 1
                    --> ['LAC']*/
                    instr = LAC;
                    }
                }
                if(bit_test(low, 0x7, 0xf)){
                /*0111 on bits: (0, 1, 2, 3) from op part: 3
                --> ['ELPM3', 'LAT']*/
                    if(bit_test(high, 0x0, 0xe)){
                    /*000 on bits: (0, 1, 2) from op part: 1
                    --> ['ELPM3']*/
                    instr = ELPM3;
                    }
                    if(bit_test(high, 0x2, 0xe)){
                    /*001 on bits: (0, 1, 2) from op part: 1
                    --> ['LAT']*/
                    instr = LAT;
                    }
                }
                if(bit_test(low, 0x9, 0xf)){
                /*1001 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LDD_Y2', 'ST_Y2']*/
                    if(bit_test(high, 0x0, 0xe)){
                    /*000 on bits: (0, 1, 2) from op part: 1
                    --> ['LDD_Y2']*/
                    instr = LDD_Y2;
                    }
                    if(bit_test(high, 0x2, 0xe)){
                    /*001 on bits: (0, 1, 2) from op part: 1
                    --> ['ST_Y2']*/
                    instr = ST_Y2;
                    }
                }
                if(bit_test(low, 0xa, 0xf)){
                /*1010 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LDD_Y3', 'ST_Y3']*/
                    if(bit_test(high, 0x0, 0xe)){
                    /*000 on bits: (0, 1, 2) from op part: 1
                    --> ['LDD_Y3']*/
                    instr = LDD_Y3;
                    }
                    if(bit_test(high, 0x2, 0xe)){
                    /*001 on bits: (0, 1, 2) from op part: 1
                    --> ['ST_Y3']*/
                    instr = ST_Y3;
                    }
                }
                if(bit_test(low, 0xc, 0xf)){
                /*1100 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LD1', 'ST1']*/
                    if(bit_test(high, 0x0, 0xe)){
                    /*000 on bits: (0, 1, 2) from op part: 1
                    --> ['LD1']*/
                    instr = LD1;
                    }
                    if(bit_test(high, 0x2, 0xe)){
                    /*001 on bits: (0, 1, 2) from op part: 1
                    --> ['ST1']*/
                    instr = ST1;
                    }
                }
                if(bit_test(low, 0xd, 0xf)){
                /*1101 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LD2', 'ST2']*/
                    if(bit_test(high, 0x0, 0xe)){
                    /*000 on bits: (0, 1, 2) from op part: 1
                    --> ['LD2']*/
                    instr = LD2;
                    }
                    if(bit_test(high, 0x2, 0xe)){
                    /*001 on bits: (0, 1, 2) from op part: 1
                    --> ['ST2']*/
                    instr = ST2;
                    }
                }
                if(bit_test(low, 0xe, 0xf)){
                /*1110 on bits: (0, 1, 2, 3) from op part: 3
                --> ['LD3', 'ST3']*/
                    if(bit_test(high, 0x0, 0xe)){
                    /*000 on bits: (0, 1, 2) from op part: 1
                    --> ['LD3']*/
                    instr = LD3;
                    }
                    if(bit_test(high, 0x2, 0xe)){
                    /*001 on bits: (0, 1, 2) from op part: 1
                    --> ['ST3']*/
                    instr = ST3;
                    }
                }
                if(bit_test(low, 0xf, 0xf)){
                /*1111 on bits: (0, 1, 2, 3) from op part: 3
                --> ['POP', 'PUSH']*/
                    if(bit_test(high, 0x0, 0xe)){
                    /*000 on bits: (0, 1, 2) from op part: 1
                    --> ['POP']*/
                    instr = POP;
                    }
                    if(bit_test(high, 0x2, 0xe)){
                    /*001 on bits: (0, 1, 2) from op part: 1
                    --> ['PUSH']*/
                    instr = PUSH;
                    }
                }
            }
            if(bit_test(high, 0x4, 0xc)){
            /*01 on bits: (0, 1) from op part: 1
            --> ['ADIW', 'ASR', 'BCLR', 'BREAK', 'BSET', 'CALL', 'CLC', 'CLH', 'CLI', 'CLN', 'CLS', 'CLT', 'CLV', 'CLZ', 'COM', 'DEC', 'DES', 'EICALL', 'EIJMP', 'ELPM1', 'ICALL', 'IJMP', 'INC', 'JMP', 'LPM1', 'LSR', 'NEG', 'RET', 'RETI', 'ROR', 'SBIW', 'SEC', 'SEH', 'SEI', 'SEN', 'SES', 'SET', 'SEV', 'SEZ', 'SLEEP', 'SPM', 'SPM2', 'SPM2_4', 'SWAP', 'WDR']*/
                if(bit_test(high, 0x4, 0xe)){
                /*010 on bits: (0, 1, 2) from op part: 1
                --> ['ASR', 'BCLR', 'BREAK', 'BSET', 'CALL', 'CLC', 'CLH', 'CLI', 'CLN', 'CLS', 'CLT', 'CLV', 'CLZ', 'COM', 'DEC', 'DES', 'EICALL', 'EIJMP', 'ELPM1', 'ICALL', 'IJMP', 'INC', 'JMP', 'LPM1', 'LSR', 'NEG', 'RET', 'RETI', 'ROR', 'SEC', 'SEH', 'SEI', 'SEN', 'SES', 'SET', 'SEV', 'SEZ', 'SLEEP', 'SPM', 'SPM2', 'SPM2_4', 'SWAP', 'WDR']*/
                    if(bit_test(low, 0x0, 0xe)){
                    /*000 on bits: (0, 1, 2) from op part: 3
                    --> ['COM', 'NEG']*/
                        if(bit_test(low, 0x0, 0xf)){
                        /*0000 on bits: (0, 1, 2, 3) from op part: 3
                        --> ['COM']*/
                        instr = COM;
                        }
                        if(bit_test(low, 0x1, 0xf)){
                        /*0001 on bits: (0, 1, 2, 3) from op part: 3
                        --> ['NEG']*/
                        instr = NEG;
                        }
                    }
                    if(bit_test(low, 0x2, 0xe)){
                    /*001 on bits: (0, 1, 2) from op part: 3
                    --> ['INC', 'SWAP']*/
                        if(bit_test(low, 0x2, 0xf)){
                        /*0010 on bits: (0, 1, 2, 3) from op part: 3
                        --> ['SWAP']*/
                        instr = SWAP;
                        }
                        if(bit_test(low, 0x3, 0xf)){
                        /*0011 on bits: (0, 1, 2, 3) from op part: 3
                        --> ['INC']*/
                        instr = INC;
                        }
                    }
                    if(bit_test(low, 0x4, 0xe)){
                    /*010 on bits: (0, 1, 2) from op part: 3
                    --> ['ASR']*/
                    instr = ASR;
                    }
                    if(bit_test(low, 0x6, 0xe)){
                    /*011 on bits: (0, 1, 2) from op part: 3
                    --> ['LSR', 'ROR']*/
                        if(bit_test(low, 0x6, 0xf)){
                        /*0110 on bits: (0, 1, 2, 3) from op part: 3
                        --> ['LSR']*/
                        instr = LSR;
                        }
                        if(bit_test(low, 0x7, 0xf)){
                        /*0111 on bits: (0, 1, 2, 3) from op part: 3
                        --> ['ROR']*/
                        instr = ROR;
                        }
                    }
                    if(bit_test(low, 0x8, 0xe)){
                    /*100 on bits: (0, 1, 2) from op part: 3
                    --> ['BCLR', 'BREAK', 'BSET', 'CLC', 'CLH', 'CLI', 'CLN', 'CLS', 'CLT', 'CLV', 'CLZ', 'EICALL', 'EIJMP', 'ELPM1', 'ICALL', 'IJMP', 'LPM1', 'RET', 'RETI', 'SEC', 'SEH', 'SEI', 'SEN', 'SES', 'SET', 'SEV', 'SEZ', 'SLEEP', 'SPM', 'SPM2', 'SPM2_4', 'WDR']*/
                        if(bit_test(high, 0x4, 0xf)){
                        /*0100 on bits: (0, 1, 2, 3) from op part: 1
                        --> ['BCLR', 'BSET', 'CLC', 'CLH', 'CLI', 'CLN', 'CLS', 'CLT', 'CLV', 'CLZ', 'EIJMP', 'IJMP', 'SEC', 'SEH', 'SEI', 'SEN', 'SES', 'SET', 'SEV', 'SEZ']*/
                            if(bit_test(low, 0x8, 0xf)){
                            /*1000 on bits: (0, 1, 2, 3) from op part: 3
                            --> ['BCLR', 'BSET', 'CLC', 'CLH', 'CLI', 'CLN', 'CLS', 'CLT', 'CLV', 'CLZ', 'SEC', 'SEH', 'SEI', 'SEN', 'SES', 'SET', 'SEV', 'SEZ']*/
                                if(bit_test(low, 0x0, 0x80)){
                                /*0 on bits: (0,) from op part: 2
                                --> ['BSET', 'SEC', 'SEH', 'SEI', 'SEN', 'SES', 'SET', 'SEV', 'SEZ']*/
                                instr = BSET;
                                }
                                if(bit_test(low, 0x80, 0x80)){
                                /*1 on bits: (0,) from op part: 2
                                --> ['BCLR', 'CLC', 'CLH', 'CLI', 'CLN', 'CLS', 'CLT', 'CLV', 'CLZ']*/
                                instr = BCLR;
                                }
                            }
                            if(bit_test(low, 0x9, 0xf)){
                            /*1001 on bits: (0, 1, 2, 3) from op part: 3
                            --> ['EIJMP', 'IJMP']*/
                                if(bit_test(low, 0x0, 0xf0)){
                                /*0000 on bits: (0, 1, 2, 3) from op part: 2
                                --> ['IJMP']*/
                                instr = IJMP;
                                }
                                if(bit_test(low, 0x10, 0xf0)){
                                /*0001 on bits: (0, 1, 2, 3) from op part: 2
                                --> ['EIJMP']*/
                                instr = EIJMP;
                                }
                            }
                        }
                        if(bit_test(high, 0x5, 0xf)){
                        /*0101 on bits: (0, 1, 2, 3) from op part: 1
                        --> ['BREAK', 'EICALL', 'ELPM1', 'ICALL', 'LPM1', 'RET', 'RETI', 'SLEEP', 'SPM', 'SPM2', 'SPM2_4', 'WDR']*/
                            if(bit_test(low, 0x0, 0xf0)){
                            /*0000 on bits: (0, 1, 2, 3) from op part: 2
                            --> ['ICALL', 'RET']*/
                                if(bit_test(low, 0x8, 0xf)){
                                /*1000 on bits: (0, 1, 2, 3) from op part: 3
                                --> ['RET']*/
                                instr = RET;
                                }
                                if(bit_test(low, 0x9, 0xf)){
                                /*1001 on bits: (0, 1, 2, 3) from op part: 3
                                --> ['ICALL']*/
                                instr = ICALL;
                                }
                            }
                            if(bit_test(low, 0x10, 0xf0)){
                            /*0001 on bits: (0, 1, 2, 3) from op part: 2
                            --> ['EICALL', 'RETI']*/
                                if(bit_test(low, 0x8, 0xf)){
                                /*1000 on bits: (0, 1, 2, 3) from op part: 3
                                --> ['RETI']*/
                                instr = RETI;
                                }
                                if(bit_test(low, 0x9, 0xf)){
                                /*1001 on bits: (0, 1, 2, 3) from op part: 3
                                --> ['EICALL']*/
                                instr = EICALL;
                                }
                            }
                            if(bit_test(low, 0x80, 0xf0)){
                            /*1000 on bits: (0, 1, 2, 3) from op part: 2
                            --> ['SLEEP']*/
                            instr = SLEEP;
                            }
                            if(bit_test(low, 0x90, 0xf0)){
                            /*1001 on bits: (0, 1, 2, 3) from op part: 2
                            --> ['BREAK']*/
                            instr = BREAK;
                            }
                            if(bit_test(low, 0xa0, 0xf0)){
                            /*1010 on bits: (0, 1, 2, 3) from op part: 2
                            --> ['WDR']*/
                            instr = WDR;
                            }
                            if(bit_test(low, 0xc0, 0xf0)){
                            /*1100 on bits: (0, 1, 2, 3) from op part: 2
                            --> ['LPM1']*/
                            instr = LPM1;
                            }
                            if(bit_test(low, 0xd0, 0xf0)){
                            /*1101 on bits: (0, 1, 2, 3) from op part: 2
                            --> ['ELPM1']*/
                            instr = ELPM1;
                            }
                            if(bit_test(low, 0xe0, 0xf0)){
                            /*1110 on bits: (0, 1, 2, 3) from op part: 2
                            --> ['SPM', 'SPM2']*/
                            // NOTE: Instruction codes equal
                            instr = SPM;
                            }
                            if(bit_test(low, 0xf0, 0xf0)){
                            /*1111 on bits: (0, 1, 2, 3) from op part: 2
                            --> ['SPM2_4']*/
                            instr = SPM2_4;
                            }
                        }
                    }
                    if(bit_test(low, 0xa, 0xe)){
                    /*101 on bits: (0, 1, 2) from op part: 3
                    --> ['DEC', 'DES']*/
                        if(bit_test(low, 0xa, 0xf)){
                        /*1010 on bits: (0, 1, 2, 3) from op part: 3
                        --> ['DEC']*/
                        instr = DEC;
                        }
                        if(bit_test(low, 0xb, 0xf)){
                        /*1011 on bits: (0, 1, 2, 3) from op part: 3
                        --> ['DES']*/
                        instr = DES;
                        }
                    }
                    if(bit_test(low, 0xc, 0xe)){
                    /*110 on bits: (0, 1, 2) from op part: 3
                    --> ['JMP']*/
                    instr = JMP;
                    }
                    if(bit_test(low, 0xe, 0xe)){
                    /*111 on bits: (0, 1, 2) from op part: 3
                    --> ['CALL']*/
                    instr = CALL;
                    }
                }
                if(bit_test(high, 0x6, 0xe)){
                /*011 on bits: (0, 1, 2) from op part: 1
                --> ['ADIW', 'SBIW']*/
                    if(bit_test(high, 0x6, 0xf)){
                    /*0110 on bits: (0, 1, 2, 3) from op part: 1
                    --> ['ADIW']*/
                    instr = ADIW;
                    }
                    if(bit_test(high, 0x7, 0xf)){
                    /*0111 on bits: (0, 1, 2, 3) from op part: 1
                    --> ['SBIW']*/
                    instr = SBIW;
                    }
                }
            }
            if(bit_test(high, 0x8, 0xc)){
            /*10 on bits: (0, 1) from op part: 1
            --> ['CBI', 'SBI', 'SBIC', 'SBIS']*/
                if(bit_test(high, 0x8, 0xf)){
                /*1000 on bits: (0, 1, 2, 3) from op part: 1
                --> ['CBI']*/
                instr = CBI;
                }
                if(bit_test(high, 0x9, 0xf)){
                /*1001 on bits: (0, 1, 2, 3) from op part: 1
                --> ['SBIC']*/
                instr = SBIC;
                }
                if(bit_test(high, 0xa, 0xf)){
                /*1010 on bits: (0, 1, 2, 3) from op part: 1
                --> ['SBI']*/
                instr = SBI;
                }
                if(bit_test(high, 0xb, 0xf)){
                /*1011 on bits: (0, 1, 2, 3) from op part: 1
                --> ['SBIS']*/
                instr = SBIS;
                }
            }
            if(bit_test(high, 0xc, 0xc)){
            /*11 on bits: (0, 1) from op part: 1
            --> ['MUL']*/
            instr = MUL;
            }
        }
        if(bit_test(high, 0xb0, 0xf0)){
        /*1011 on bits: (0, 1, 2, 3) from op part: 0
        --> ['IN', 'OUT']*/
            if(bit_test(high, 0x0, 0x8)){
            /*0 on bits: (0,) from op part: 1
            --> ['IN']*/
            instr = IN;
            }
            if(bit_test(high, 0x8, 0x8)){
            /*1 on bits: (0,) from op part: 1
            --> ['OUT']*/
            instr = OUT;
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
        }
        if(bit_test(high, 0xe0, 0xf0)){
        /*1110 on bits: (0, 1, 2, 3) from op part: 0
        --> ['LDI', 'SER']*/
        instr = LDI;
        }
    }
    if(bit_test(high, 0xd0, 0xd0)){
    /*111 on bits: (0, 1, 3) from op part: 0
    --> ['BLD', 'BRBC', 'BRBS', 'BRCC', 'BRCS', 'BREQ', 'BRGE', 'BRHC', 'BRHS', 'BRID', 'BRIE', 'BRLO', 'BRLT', 'BRMI', 'BRNE', 'BRPL', 'BRSH', 'BRTC', 'BRTS', 'BRVC', 'BRVS', 'BST', 'RCALL', 'SBRC', 'SBRS']*/
        if(bit_test(high, 0xd0, 0xf0)){
        /*1101 on bits: (0, 1, 2, 3) from op part: 0
        --> ['RCALL']*/
        instr = RCALL;
        }
        if(bit_test(high, 0xf0, 0xf0)){
        /*1111 on bits: (0, 1, 2, 3) from op part: 0
        --> ['BLD', 'BRBC', 'BRBS', 'BRCC', 'BRCS', 'BREQ', 'BRGE', 'BRHC', 'BRHS', 'BRID', 'BRIE', 'BRLO', 'BRLT', 'BRMI', 'BRNE', 'BRPL', 'BRSH', 'BRTC', 'BRTS', 'BRVC', 'BRVS', 'BST', 'SBRC', 'SBRS']*/
            if(bit_test(high, 0x0, 0xc)){
            /*00 on bits: (0, 1) from op part: 1
            --> ['BRBS', 'BRCS', 'BREQ', 'BRHS', 'BRIE', 'BRLO', 'BRLT', 'BRMI', 'BRTS', 'BRVS']*/
            instr = BRBS;
            }
            if(bit_test(high, 0x4, 0xc)){
            /*01 on bits: (0, 1) from op part: 1
            --> ['BRBC', 'BRCC', 'BRGE', 'BRHC', 'BRID', 'BRNE', 'BRPL', 'BRSH', 'BRTC', 'BRVC']*/
            instr = BRBC;
            }
            if(bit_test(high, 0x8, 0xc)){
            /*10 on bits: (0, 1) from op part: 1
            --> ['BLD', 'BST']*/
                if(bit_test(high, 0x8, 0xe)){
                /*100 on bits: (0, 1, 2) from op part: 1
                --> ['BLD']*/
                instr = BLD;
                }
                if(bit_test(high, 0xa, 0xe)){
                /*101 on bits: (0, 1, 2) from op part: 1
                --> ['BST']*/
                instr = BST;
                }
            }
            if(bit_test(high, 0xc, 0xc)){
            /*11 on bits: (0, 1) from op part: 1
            --> ['SBRC', 'SBRS']*/
                if(bit_test(high, 0xc, 0xe)){
                /*110 on bits: (0, 1, 2) from op part: 1
                --> ['SBRC']*/
                instr = SBRC;
                }
                if(bit_test(high, 0xe, 0xe)){
                /*111 on bits: (0, 1, 2) from op part: 1
                --> ['SBRS']*/
                instr = SBRS;
                }
            }
        }
    }
    return instr;
}

int INSTRUCTION_is_32b(INSTRUCTION instr){
    int is_32b = 0;
    switch (instr){
        case CALL:
        case JMP:
        case STS:
        case LDS:
            is_32b = 1;
            break;
        default:
            break;
    }
    return is_32b;
}
