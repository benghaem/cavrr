#include <stdio.h>
#include <stdint.h>
#ifndef INTELHEX_H
#define INTELHEX_H

typedef struct{
    FILE* fp;
    int data_sum;
    int data_remaining;
    int in_data;
    int at_end;
    int valid;
} ihex;

int ihex_open(ihex *ih, char* filename);

int ihex_close(ihex *ih);

int ihex_validate_checksum(ihex* ih);

void ihex_consume_start(ihex* ih);

uint8_t ihex_get_byte(ihex* ih);

int ihex_at_end(ihex* ih);

#endif

