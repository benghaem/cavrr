#include "intelhex.h"
#include "bitutil.h"
#include <string.h>

int IHEX_open(IHEX *ih, char* filename){
    ih->data_sum = 0;
    ih->data_remaining = 0;
    ih->in_data = 0;
    ih->at_end = 0;
    ih->valid = 1;
    ih->fp = fopen(filename,"r");

    if (ih->fp == NULL){
        printf("Could not open file");
        ih->valid = 0;
        return 0;
    }
    return 1;
}

int IHEX_close(IHEX *ih){
    return fclose(ih->fp);
}

int IHEX_at_end(IHEX* ih){
    return ih->at_end;
}

void IHEX_consume_start(IHEX* ih){
    char single[2];
    char bytestr[3];
    char wordstr[5];
    int recordtype;

    //read in line starting char
    fgets(single,2,ih->fp);

    if (strcmp(single, ":") == 0){

        //read in data length
        fgets(bytestr,3,ih->fp);
        ih->data_remaining = ascii_byte_to_int(bytestr);

        //read in address offset (ignoring for now);
        fgets(wordstr,5,ih->fp);

        //read in record type
        fgets(bytestr,3,ih->fp);
        recordtype = ascii_byte_to_int(bytestr);
        if(recordtype == 0){
            //data case
            ih->in_data = 1;
            //if we are in this case we should just request the
            //next byte
        }
        if(recordtype == 1){
            //end of file case
            ih->in_data = 0;
            ih->at_end = 1;

        }
    }
}

//Function to validate checksum
//currently does not validate checksum
//just consumes the values
int IHEX_validate_checksum(IHEX* ih){
    char bytestr[3];

    //eat checksum
    fgets(bytestr,3,ih->fp);
    printf("Checksum: %s \n",bytestr);

    //eat newline character
    fgets(bytestr,3,ih->fp);
    ih->in_data = 0;

    return 1;
}

uint8_t IHEX_get_byte(IHEX* ih){
    char bytestr[3];
    uint8_t ret = 0;
    uint8_t recordtype;

    if(ih->at_end){
        return 0;
    }

    //if we are in the data we can just go ahead and
    //read the bytes
    if(ih->in_data){
        if(ih->data_remaining > 0){
            fgets(bytestr,3,ih->fp);
            printf("%s, ",bytestr);

            ret = ascii_byte_to_int(bytestr);
            ih->data_remaining--;

            if(ih->data_remaining == 0){
                //checksum stuff here
                IHEX_validate_checksum(ih);
                //then setup for the next read
                IHEX_consume_start(ih);
            }
        }
    }
    //otherwise we need to consume the start
    //and then get bytes
    else{
        IHEX_consume_start(ih);
        //we can then get the byte as we will be in data
        ret = IHEX_get_byte(ih);
    }
    return ret;
}

