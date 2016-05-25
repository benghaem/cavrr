#include "bitutil.h"
#include <stdint.h>

//Test the bits of a target
//For example to compare the third bits 0001, 0000, 0001
int bit_test(unsigned long target, unsigned long test, unsigned long mask){
    unsigned long masked = target & mask;
    return (masked == test);
}

//Create a new integer from an array of bits
unsigned long bit_create(unsigned int* bits, unsigned int size){
    int i;
    unsigned long ret = 0;
    if (size < 32){
        for(i = 0; i < size; i++){
            if (bits[i] > 1){
                bits[i] = 1;
            }
            ret |= (bits[i] << i);
        }
    }
    return ret;
}
