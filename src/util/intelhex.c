#include "intelhex.h"
#include "bitutil.h"
#include <string.h>

/*
 * Opens an intel hex formatted file
 */
int ihex_open(ihex *ih, char* filename){
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

/*
 * Closes the file
 */
int ihex_close(ihex *ih){
    return fclose(ih->fp);
}

/*
 * Checks if at the end of the file
 */
int ihex_at_end(ihex* ih){
    return ih->at_end;
}

/*
 * Consumes the front of each line read in
 * The programmer should not have to call this function directly
 */
void ihex_consume_start(ihex* ih){
    char single[2];
    char bytestr[3];
    char wordstr[5];
    int recordtype;

    /* readin line starting char */
    if (fgets(single,2,ih->fp) != NULL){

        if (strcmp(single, ":") == 0){

            /* read in data length */
            if (fgets(bytestr,3,ih->fp) != NULL){
                ih->data_remaining = ascii_byte_to_int(bytestr);

                /* read in address offset (ignoring for now) */
                if (fgets(wordstr,5,ih->fp) != NULL){

                    /* read in record type */
                    if(fgets(bytestr,3,ih->fp) != NULL){;
                        recordtype = ascii_byte_to_int(bytestr);
                        if(recordtype == 0){
                            /* data case */
                            ih->in_data = 1;
                            /*
                             * if we are in this case we should just request the
                             * next byte
                             */
                        }
                        if(recordtype == 1){
                            /* end of file case */
                            ih->in_data = 0;
                            ih->at_end = 1;

                        }
                    }
                }

            }
        }
    }
}

/*
 * Validates checksums
 * TODO: currently does not validate checksum
 * just consumes the values
 */
int ihex_validate_checksum(ihex* ih){
    char bytestr[3];

    /* eat checksum */
    if (fgets(bytestr,3,ih->fp) != NULL){;
        printf("Checksum: %s \n",bytestr);

        /* eat newline character */
        if (fgets(bytestr,3,ih->fp) != NULL){
            ih->in_data = 0;
        }
    }
    return 1;
}


/*
 * Gets the next byte of the file. This function will always return so
 * one should check ihex_at_end before running. This function will automatically call
 * ihex_consume_start when needed
 */
uint8_t ihex_get_byte(ihex* ih){
    char bytestr[3];
    uint8_t ret = 0;

    if(ih->at_end){
        return 0;
    }

    /*
     * if we are in the data we can just go ahead and
     * read the bytes
     */
    if(ih->in_data){
        if(ih->data_remaining > 0){
            /*
             * This is not an amazing way of handling this
             * Maybe change in the future
             */
            if (fgets(bytestr,3,ih->fp) == NULL){
                printf("Read error\n");
                return 0;
            };
            printf("%s, ",bytestr);

            ret = ascii_byte_to_int(bytestr);
            ih->data_remaining--;

            /*
             * make sure that was not the last piece of data
             * if it was then we need to restart
             */
            if(ih->data_remaining == 0){
                /* checksum stuff here */
                ihex_validate_checksum(ih);
                /* then setup for the next read */
                ihex_consume_start(ih);
            }
        }
    }
    /*
     * otherwise we need to consume the start
     * and then get bytes
     */
    else{
        ihex_consume_start(ih);
        /* we can then get the byte as we will be in data */
        ret = ihex_get_byte(ih);
    }
    return ret;
}
