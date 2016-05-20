#include "registers.h"

unsigned char RFILE_read_reg(RFILE* rfile, int r){
    //bounds check as our regfile only has 32 slots
    //0-31
    if (r >= 0 && r < 32){
        return rfile->reg[r];
    }
    else{
        return 0;
    }
}

void RFILE_write_reg(RFILE* rfile, int r, unsigned char val){
    //bounds check again as our regfile only has 32 slots
    //0-31
    if (r >= 0 && r < 32){
        rfile->reg[r] = val;
    }
}
