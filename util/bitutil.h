#include <stdint.h>

#ifndef BITUTIL_H
#define BITUTIL_H

#define bit( I, b ) ( I >> b & 0x1 )

int bit_test(unsigned long target, unsigned long test, unsigned long mask);

unsigned long bit_create(unsigned int* bits, unsigned int size);

uint16_t bit_isolate16(unsigned int part_count, ...);

/* assumes a c style string of length 3 */
uint8_t ascii_byte_to_int(char* str);

#endif /* bitutil.h */

/*
 * idea for selection range api
 * sel_t k0 = sel_range(1,4);
 * sel_t k1 = sel_loc(2);
 * sel_t k2 = sel_range(2,4);
 * uint16_t k = bit_isolate16(3,&k0,&k1,&k2);
 */
