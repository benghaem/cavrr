#include "../../processor.h"
#include "../../instruction.h"
#include <stdio.h>
#include "../../util/intelhex.h"
#include <unistd.h>

int main(int argc, char** argv){

    struct processor p;
    struct progmem* pm = &p.pmem;
    int pm_addr = 0;

    ihex ih;
    uint16_t op;
    enum instruction instr;

    if (argc < 1){
        printf("not enough arguments\n");
    }

    /* Init processor in debug mode */
    processor_init(&p, 1);

    if(ihex_open(&ih,argv[1])){
        printf("Opened: %s\n", argv[1]);
        while(!ihex_at_end(&ih)){
            op = (ihex_get_byte(&ih) << 8) + ihex_get_byte(&ih);
            printf("%X --> ",op);
            progmem_write_addr(pm,pm_addr,op);
            pm_addr++;
            instr = instruction_decode_bytes(op);
            printf("%s",instruction_str(instr));
            printf("\n");
        }
    }

    progmem_write_addr(pm,pm_addr,0x9895);

    processor_loop(&p);

    return 0;
}
