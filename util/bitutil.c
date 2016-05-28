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

//assumes a c style string of length 3
uint8_t ascii_byte_to_int(char* str){
    uint8_t output = 0;
    int i;
    char c;
    int val = 0;
    for(i = 0; i < 2; i++){
        c = str[i];
        //ascii 0 <-> ascii 9
        if (c >= 48 && c <= 57){
            val = c - 48;
        }
        //ascii A <-> ascii F
        else if (c >= 65 && c <= 70){
            val = c - 65 + 10;
        }
        //ascii a <-> ascii f
        else if (c >= 97 && c <= 102){
            val = c - 97 + 10;
        }
        else{
            val = 0;
        }
        //shift over four if i = 0
        output += val << ((1-i) * 4);
    }
    return output;
}
