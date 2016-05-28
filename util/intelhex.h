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
} IHEX;

int IHEX_open(IHEX *ih, char* filename);

int IHEX_close(IHEX *ih);

int IHEX_validate_checksum(IHEX* ih);

void IHEX_consume_start(IHEX* ih);

uint8_t IHEX_get_byte(IHEX* ih);

int IHEX_at_end(IHEX* ih);

#endif

