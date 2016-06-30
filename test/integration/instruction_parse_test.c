#include "../instruction.h"
#include <stdio.h>
#include "../util/intelhex.h"
#include <unistd.h>

int main(int argc, char** argv){

    ihex ih;
    uint16_t op;
    enum instruction instr;

    if (argc < 1){
        printf("not enough arguments\n");
    }

    if(ihex_open(&ih,argv[1])){
        printf("Opened: %s\n", argv[1]);
        while(!ihex_at_end(&ih)){
            op = (ihex_get_byte(&ih) << 8) + ihex_get_byte(&ih);
            printf("%X --> ",op);
            instr = instruction_decode_bytes(op);
            printf("%s",instruction_str(instr));
            printf("\n");
        }
    }

    return 0;
}
