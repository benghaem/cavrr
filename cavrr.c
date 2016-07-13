#include "processor.h"
#include "memory.h"
#include "util/intelhex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct config{
    char bkps[PROGMEM_SIZE];
    char run_ignores_break;
    char no_reset_on_load;
};

static struct config config = {0};

const char version_string[] = "cavrr: An ATtiny45 Emulator\n"
                              "Version 0.0.0\n"
                              "Benjamin Ghaemmaghami (2016)\n"
                              "View the source at: https://github.com/benghaem/cavrr\n";

/*
 * Loads an Intel Hex formatted file into the processor's program memory
 * starting from address 0
 */
int load_program(char* fname, struct processor* p){
    ihex ih;
    int pm_addr = 0;
    uint16_t op;

    if (!config.no_reset_on_load){
        processor_init(p);
    }

    if (ihex_open(&ih, fname)){
        printf("Loading file: %s\n",fname);
        while(!ihex_at_end(&ih)){
            op = (ihex_get_byte(&ih) << 8) + ihex_get_byte(&ih);
            progmem_write_addr(&p->pmem,pm_addr,op);
            pm_addr++;
        }
        printf("%i addresses loaded\n", pm_addr);
        return pm_addr;
    }
    return 0;
}

/*
 * Prints out a region of program memory relative to the processor's
 * program counter.
 * rel_start is assumed to be smaller than rel_end as the function can
 * be called with a negative value for rel_start and a positive value for
 * rel_end so that we can display something like:
 * pc - 2
 * pc - 1
 * pc
 * pc + 1
 */
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

        (config.bkps[local_pc] == 1) ? printf("*") : printf(" ");

        printf("%4i, %2i : %04X (%s)", local_pc, rel_start, progmem_value, instruction_str(instruction_decode_bytes(progmem_value)));

        (local_pc == p->pc) ? printf(" <--\n") : printf("\n");

        rel_start++;
    }
}

/*
 * Sets the given configuration flag
 */
void set_config_flag(char* str, char val){
    if(!strcmp(str, "run_ignores_break")){
        config.run_ignores_break = val;
    } else if(!strcmp(str, "no_reset_on_load")){
        config.no_reset_on_load = val;
    } else {
        printf("unknown config value: %s\n",str);
    }
}

/*
 * Transforms a string into an array of strings and
 * reports the number of elements in this array.
 */
void get_cmds(char *str, char** argv[], int* argc){
    /* We need enough space for a possible tokenization of all characters
     * so we allocate space for up to strlen(str) char*'s */
    char *pch;
    int n = 0;

    *argv = malloc(sizeof(char*) * strlen(str));

    pch = strtok(str, " \n");
    while(pch != NULL){
        (*argv)[n] = pch;
        pch = strtok(NULL, " \n");
        n++;
    }
    *argc = n;
}

/*
 * Toggles a breakpoint in the config struct
 * breakpoints are simply 0/1 values for every progmem address
 * that are checked by the step_till_breakpoint function
 */
void toggle_breakpoint(int bp){
    if (bp >=0 && bp < PROGMEM_SIZE){
        config.bkps[bp] = !config.bkps[bp];
    }
}

/*
 * Steps the processor forward until a breakpoint is reached
 */
void step_till_breakpoint(struct processor *p){
    /* Always step forward or we will get stuck on breakpoints */
    processor_step(p,1);
    while(config.bkps[p->pc] != 1){
        processor_step(p, 1);
    }
}

int main(int argc, char **argv){
    struct processor p;
    int running = 1;
    char cmd[100];
    char** cmd_argv;
    int cmd_argc;

    printf("%s", version_string);

    processor_init(&p);

    if (argc > 1){
        /* NOTE: initializes the processor a second time if no_reset_on_load = 0 */
        load_program(argv[1],&p);
    }

    while (running){
        printf("cavrr> ");

        fgets(cmd, 100, stdin);
        get_cmds(cmd, &cmd_argv, &cmd_argc);

        if (!strcmp(cmd_argv[0], "run")){
            if (cmd_argc > 1){
                load_program(cmd_argv[1], &p);
            }
            if (config.run_ignores_break){
                processor_loop(&p);
            } else {
                step_till_breakpoint(&p);
            }

        } else if (!strcmp(cmd_argv[0], "step")){
            if (cmd_argc > 1){
                processor_step(&p,atoi(cmd_argv[1]));
            } else {
                processor_step(&p, 1);
            }

        } else if (!strcmp(cmd_argv[0], "load")){
            if (cmd_argc > 1){
                load_program(cmd_argv[1],&p);
            } else {
                printf("No filename given\n");
            }

        } else if (!strcmp(cmd_argv[0], "dbe")){
            printf("processor debug enabled\n");
            p.debug = 1;

        } else if (!strcmp(cmd_argv[0], "exit")){
            running = 0;

        } else if (!strcmp(cmd, "ppc")){
            printf("processor pc: %i",p.pc);

        } else if (!strcmp(cmd, "local")){
            print_pc_region(&p, -2, 4);

        } else if (!strcmp(cmd_argv[0], "bt")){
            if (cmd_argc > 1){
                toggle_breakpoint(atoi(cmd_argv[1]));
            } else{
            toggle_breakpoint(p.pc);
            }
        } else if (!strcmp(cmd_argv[0], "set")){
            if (cmd_argc > 2){
                set_config_flag(cmd_argv[1], atoi(cmd_argv[2]));
            } else {
                printf("No flag given\n");
            }

        } else if (!strcmp(cmd_argv[0], "clear")){
            /* Will only work on terminals that support ANSI */
            printf("\033[2J\033[H");

        } else{
            printf("%s unknown command\n", cmd);
        }

        free(cmd_argv);
    }

}
