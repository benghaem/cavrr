#include "cavrr.h"
#include "memory.h"
#include "util/intelhex.h"
#include "util/list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "cavrr_config.h"

struct config{
    unsigned int run_ignores_break;
    unsigned int reset_on_load;
    unsigned int enable_step_delay;
    unsigned int step_delay_ms;
};

struct state{
    char bkps[PROGMEM_SIZE];
    struct list* watch_addrs;
    struct datamem local_dmem;
    volatile sig_atomic_t emu_running;
    volatile sig_atomic_t notify_stop;
};

static struct config config = {
    0,   /* run_ignores_break */
    1,   /* reset_on_load */
    0,   /* enable_step_delay */
    200  /* step_delay_ms */
};

static struct state state = {{0},0,{{0}},0,0};

const char version_string[] = "cavrr: An ATtiny45 Emulator\n"
                              "Version %d.%d.%d\n"
                              "Benjamin Ghaemmaghami (2016)\n"
                              "View the source at: https://github.com/benghaem/cavrr\n";


/*
 * Initializes complex state variables to reasonable values
 */
void init_state(){
    datamem_init(&state.local_dmem);
    state.watch_addrs = list_create();
}

/*
 * Catches SIGINT so that it may be used to stop
 * long running emulator programs
 */
void catch_sigint(int sig){
    if (state.emu_running){
        state.emu_running = 0;
        state.notify_stop = 1;
        signal(sig, catch_sigint);
    } else {
        exit(1);
    }
}

/*
 * Loads an Intel Hex formatted file into the processor's program memory
 * starting from address 0
 */
int load_program(char* fname, struct processor* p){
    ihex ih;
    int pm_addr = 0;
    uint16_t op;

    if (config.reset_on_load){
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
    struct operation op;
    char dasm_str[100];
    /* don't forget to update size of str below as well */

    if (p->pc + rel_start < 0){
        rel_start = rel_start - (p->pc + rel_start);
    }
    local_pc = p->pc + rel_start;

    for(; local_pc <= (p->pc + rel_end); local_pc++){
        op.bits = progmem_read_addr(&p->pmem, local_pc);

        op.inst = instruction_decode_bytes(op.bits);

        if (instruction_is_32b(op.inst)){
            op.ex_bits = progmem_read_addr(&p->pmem, local_pc + 1);
        }

        disassemble_to_str(&op, local_pc, dasm_str, 100);

        (state.bkps[local_pc] == 1) ? printf("*") : printf(" ");

        printf("%04X, %2i : %04X (%s)\t%s", local_pc*2, rel_start, op.bits, instruction_str(op.inst), dasm_str);

        (local_pc == p->pc) ? printf(" <--\n") : printf("\n");

        rel_start++;
    }
}

/*
 * Sets the given configuration flag
 */
void set_config_flag(char* str, unsigned int val){
    if(!strcmp(str, "run_ignores_break")){
        config.run_ignores_break = val;
    } else if(!strcmp(str, "reset_on_load")){
        config.reset_on_load = val;
    }else if(!strcmp(str, "enable_step_delay")){
        config.enable_step_delay = val;
    }else if(!strcmp(str, "step_delay_ms")){
        config.step_delay_ms = val;
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

    /* In case the input is only a newline */
    (*argv)[0] = "NULL";

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
        state.bkps[bp] = !state.bkps[bp];
    }
}

/*
 * Steps the processor forward until a breakpoint is reached
 */
void step_till_breakpoint(struct processor *p){
    state.emu_running = 1;
    /* Always step forward or we will get stuck on breakpoints */
    do{
        processor_step(p,1);
        check_watched(p);
    }while(state.bkps[p->pc] != 1 && state.emu_running);
    state.emu_running = 0;
}

/*
 * Wrapper for processor step that respects breakpoints and triggers watches
 * also may be used to step at a specified delay
 */
void step(struct processor *p, int n){
    struct timespec t1;
    /* integer division -- no need to floor */
    t1.tv_sec = config.step_delay_ms / 1000;
    t1.tv_nsec = (config.step_delay_ms - (t1.tv_sec * 1000)) * 100000;

    state.emu_running = 1;
    do{
        processor_step(p,1);
        check_watched(p);
        n--;
        if (config.enable_step_delay){
            nanosleep(&t1,NULL);
        }
    } while(state.bkps[p->pc] != 1 && n > 0 && state.emu_running);
    state.emu_running = 0;
}

/*
 * Converts a string based address into an integer value
 * Returns address on success and -1 on error
 */
int parse_address(char* offset_str, char* addr_str){

    int addr;
    int offset;

    addr = (int) strtol(addr_str, NULL, 16);

    if (addr >= 0 && addr < DATAMEM_SIZE){
        if (!strcmp(offset_str, "reg")){
            offset = RFILE_OFFSET;
        } else if (!strcmp(offset_str, "io")){
            offset = IOFILE_OFFSET;
        } else if (!strcmp(offset_str, "sram")){
            offset = SRAM_OFFSET;
        } else if (!strcmp(offset_str, "raw")){
            offset = ZERO_OFFSET;
        } else {
            printf("Offset mode is invalid. Use reg, io, sram, or raw\n");
            return -1;
        }

        return addr+offset;

    } else{
        printf("Address is out of range\n");
        return -1;
    }


}

/*
 * Checks for changes to watched addresses
 */
void check_watched(struct processor *p){
    struct list* iter = state.watch_addrs;
    uint8_t current_val;
    uint8_t prev_val;
    int addr;

    while (iter->next != NULL){
        addr = iter->v;


        current_val = datamem_read_addr(&p->dmem, ZERO_OFFSET, addr);
        prev_val = datamem_read_addr(&state.local_dmem, ZERO_OFFSET, addr);

        if (current_val != prev_val){
            datamem_write_addr(&state.local_dmem, ZERO_OFFSET, addr, current_val);
            printf("0x%03X changed: %02X -> %02X\n", addr, prev_val, current_val );
        }

        iter = iter->next;
    }
}


/*
 * Parses the instructions passed into the watch and unwatch commands
 */
int parse_watch(struct processor *p, char* mode, char* offset_str, char* addr_str){
    int addr;
    int set = 0;

    if(!strcmp(mode, "watch")){
        set = 1;
    }

    addr = parse_address(offset_str, addr_str);

    if (addr != -1){
        return set_watch(p, addr, set);
    }
    return 0;
}


/*
 * Adds or removes watching state on an address
 */
int set_watch(struct processor *p, int addr, int set){
    int already_set;
    uint8_t current_val;

    if (addr >=0 && addr <= DATAMEM_SIZE){

        current_val = datamem_read_addr(&p->dmem, ZERO_OFFSET, addr);

        already_set = list_find(state.watch_addrs, addr);

        /*
         * These are the only two cases that matter
         * in the other two cases we do not have to do anything
         */
        if (set && !already_set){
            /* load current value into the local dmem so we can diff */
            list_append(state.watch_addrs, addr);
            datamem_write_addr(&state.local_dmem, ZERO_OFFSET, addr, current_val);
            printf("Set watch on 0x%03X. Current value is 0x%02X\n", addr, current_val);
        } else if (!set && already_set){
            list_remove(state.watch_addrs, addr);
        }

        return 1;
    }
    return 0;
}

/*
 * Prints out the value of an address in datamem
 */
void print(struct processor *p, char* offset_str, char* addr_str ){
    int addr;
    uint8_t val;

    addr = parse_address(offset_str, addr_str);

    if (addr != -1){
        val = datamem_read_addr(&p->dmem, ZERO_OFFSET, addr);
        printf("0x%03X --> 0x%02X\n", addr, val);
    }
}

/*
 * Runs a very simple execution speed benchmark
 * Because the processor does not currently simulate the cycle lengths
 * of instructions the output frequency is based solely on _instructions_
 * processed
 */
void ubench(struct processor *p, int instr_count){
    clock_t time;
    float time_sec;
    float i_per_sec;
    double mhz;
    printf("Starting %i instruction benchmark\n", instr_count);
    time = clock();
    processor_step(p, instr_count);
    time = clock() - time;
    time_sec = (float) time / CLOCKS_PER_SEC;
    i_per_sec = instr_count / time_sec;
    mhz = i_per_sec / 1000000;
    printf("Benchmark complete\n");
    printf("Processed %i instructions in %f seconds\n",instr_count, time_sec);
    printf("Instructions/sec: %.0f, MHz: %.4f\n", i_per_sec, mhz);
}

int main(int argc, char **argv){
    struct processor p;
    int running = 1;
    char cmd[100];
    char** cmd_argv;
    int cmd_argc;

    /* capture SIGINT to stop long running emulator programs */
    signal (SIGINT, catch_sigint);

    printf(version_string, cavrr_VERSION_MAJOR, cavrr_VERSION_MINOR, cavrr_VERSION_PATCH);

    init_state();
    processor_init(&p);

    if (argc > 1){
        /* NOTE: initializes the processor a second time if reset_on_load = 1 */
        load_program(argv[1],&p);
    }

    while (running){
        if (state.notify_stop){
            printf("stopped\n");
            state.notify_stop = 0;
        }
        printf("cavrr> ");

        if (fgets(cmd, 100, stdin) == NULL || !strcmp(cmd, "\n")){
            continue;
        };
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
                step(&p,atoi(cmd_argv[1]));
            } else {
                step(&p, 1);
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
                set_config_flag(cmd_argv[1], (unsigned int)atoi(cmd_argv[2]));
            } else {
                printf("No flag given\n");
            }

        } else if (!strcmp(cmd_argv[0], "watch") || !strcmp(cmd_argv[0], "unwatch")){
            if (cmd_argc == 3){
                parse_watch(&p, cmd_argv[0], cmd_argv[1], cmd_argv[2]);
            } else {
                printf("Incorrect number of arguments\n");
            }

        } else if (!strcmp(cmd_argv[0], "show")){
            if (cmd_argc == 3){
                print(&p, cmd_argv[1], cmd_argv[2]);
            } else {
                printf("Incorrect number of arguments\n");
            }

        } else if (!strcmp(cmd_argv[0], "clear")){
            /* Will only work on terminals that support ANSI */
            printf("\033[2J\033[H");

        } else if (!strcmp(cmd_argv[0], "ubench")){
            if (cmd_argc > 1){
                ubench(&p, atoi(cmd_argv[1]));
            } else {
                 printf("Missing instruction count\n");
            }

        } else{
            printf("%s unknown command\n", cmd);
        }

        free(cmd_argv);
    }

    return 0;
}
