#include "processor.h"

void PC_update(PC* p, uint16_t val){
    //mask out bits above 11
    //0000 0111 1111 1111
    *p = val & 0x07FF;
};
