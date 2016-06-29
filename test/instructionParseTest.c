#include "../instruction.h"
#include <stdio.h>
#include "../util/intelhex.h"
#include <unistd.h>

int main(int argc, char** argv){

    IHEX ih;
    uint16_t op;
    INSTRUCTION instr;

    if (argc < 1){
        printf("not enough arguments\n");
    }

    if(IHEX_open(&ih,argv[1])){
        printf("Opened: %s\n", argv[1]);
        while(!IHEX_at_end(&ih)){
            op = (IHEX_get_byte(&ih) << 8) + IHEX_get_byte(&ih);
            printf("%X --> ",op);
            instr = INSTRUCTION_decode_bytes(op);
            INSTRUCTION_print(instr);
            printf("\n");
        }
    }

    return 0;
}
