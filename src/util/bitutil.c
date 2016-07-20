#include "bitutil.h"
#include <stdint.h>

/*
 * Creates a new integer from an array of bits
 * and a length
 */
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

/*
 * Coverts a c style string with length 3
 * containing the hex characters 0-F
 * to its 8bit uint representation
 *
 * Ex "FF\0" -> 255, "00\0" -> 0
 */
uint8_t ascii_byte_to_int(char* str){
    uint8_t output = 0;
    int i;
    char c;
    int val = 0;
    for(i = 0; i < 2; i++){
        c = str[i];
        /* ascii 0 <-> ascii 9 */
        if (c >= 48 && c <= 57){
            val = c - 48;
        }
        /* ascii A <-> ascii F */
        else if (c >= 65 && c <= 70){
            val = c - 65 + 10;
        }
        /* ascii a <-> ascii f */
        else if (c >= 97 && c <= 102){
            val = c - 97 + 10;
        }
        else{
            val = 0;
        }
        /* shift over four if i = 0 */
        output += val << ((1-i) * 4);
    }
    return output;
}
