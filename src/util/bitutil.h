#include <stdint.h>

#ifndef BITUTIL_H
#define BITUTIL_H

/* Isolates a single bit from a value */
#define bit( I, b ) ( I >> b & 0x1 )

/*
 * Compares the bits of the target value to that of the test
 * value as described by the mask
 * For example to compare the big-e 0th bits
 * the arguments will be: 0001, 0000, 0001
 */
#define bit_test(Target,Test,Mask) (((Target) & (Mask)) == (Test))

unsigned long bit_create(unsigned int* bits, unsigned int size);

/* assumes a c style string of length 3 */
uint8_t ascii_byte_to_int(char* str);

#endif /* bitutil.h */
