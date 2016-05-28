#include <stdint.h>

#ifndef BITUTIL_H
#define BITUTIL_H

int bit_test(unsigned long target, unsigned long test, unsigned long mask);

unsigned long bit_create(unsigned int* bits, unsigned int size);

//assumes a c style string of length 3
uint8_t ascii_byte_to_int(char* str);

#endif /* bitutil.h */
