#include "processor.h"
#include "util/intelhex.h"
#include <stdio.h>
#include <string.h>

const char version_string[] = "cavrr: An ATtiny45 Emulator\n"
                              "Version 0.0.0\n"
                              "Benjamin Ghaemmaghami (2016)\n"
                              "View the source at: https://github.com/benghaem/cavrr\n";


int load_program(char* fname, struct processor* p){
    ihex ih;
    int pm_addr = 0;
    uint16_t op;

    if (ihex_open(&ih, fname)){
        printf("Loading file: %s\n",fname);
        while(!ihex_at_end(&ih)){
            op = (ihex_get_byte(&ih) << 8) + ihex_get_byte(&ih);
            progmem_write_addr(&p->pmem,pm_addr,op);
            pm_addr++;
        }
        printf("%i addresses loaded\n", pm_addr - 1);
        return 1;
    }
    return 0;
}

void print_pc_region(struct processor* p, int rel_start, int rel_end){
    uint16_t local_pc;
    uint16_t progmem_value;
    enum instruction instr = UNKNOWN;

    if (p->pc + rel_start < 0){
        rel_start = rel_start - (p->pc + rel_start);
    }
    local_pc = p->pc + rel_start;

    for(; local_pc <= (p->pc + rel_end); local_pc++){
        progmem_value = progmem_read_addr(&p->pmem, local_pc);

        printf("%4i, %2i : %X (%s)", local_pc, rel_start, progmem_value, instruction_str(instruction_decode_bytes(progmem_value)));

        (local_pc == p->pc) ? printf(" <--\n") : printf("\n");

        rel_start++;
    }
}


int main(int argc, char **argv){
    struct processor p;
    int running = 1;
    char cmd[100];

    if (argc < 1){
        printf("not enough arguments\n");
        return 1;
    }

    printf("%s", version_string);

    /* init debug off */
    processor_init(&p, 0);

    load_program(argv[1],&p);

    while (running){
        printf("cavrr> ");
        scanf("%99s", cmd);

        if (!strcmp(cmd, "run")){
            processor_loop(&p);
        } else if (!strcmp(cmd, "step")){
            processor_step(&p);
        } else if (!strcmp(cmd, "dbe")){
            printf("debug enabled\n");
            p.debug = 1;
        } else if (!strcmp(cmd, "exit")){
            return 0;
        } else if (!strcmp(cmd, "ppc")){
            printf("processor pc: %i",p.pc);
        } else if (!strcmp(cmd, "local")){
            print_pc_region(&p, -2, 4);
        } else{
            printf("%s: unknown command\n", cmd);
        }
    }

}
