#include "../processor.h"
#include "../instruction.h"
#include <stdio.h>
#include "../util/intelhex.h"
#include <unistd.h>

int main(int argc, char** argv){

    PROCESSOR p;
    /* Init processor in debug mode */
    PROCESSOR_init(&p, 1);

    PROGMEM* pm = &p.pmem;
    int pm_addr = 0;

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
            PROGMEM_write_addr(pm,pm_addr,op);
            pm_addr++;
            instr = INSTRUCTION_decode_bytes(op);
            printf("%s",INSTRUCTION_str(instr));
            printf("\n");
        }
    }

    PROGMEM_write_addr(pm,pm_addr,0x9895);

    PROCESSOR_loop(&p);

    return 0;
}
