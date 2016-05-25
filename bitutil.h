#ifndef BITUTIL_H
#define BITUTIL_H

int bit_test(unsigned long target, unsigned long test, unsigned long mask);

unsigned long bit_create(unsigned int* bits, unsigned int size);

#endif /* bitutil.h */
