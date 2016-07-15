#include "processor.h"

#ifndef CAVRR_H
#define CAVRR_H

int load_program(char* fname, struct processor* p);

void print_pc_region(struct processor *p, int rel_start, int rel_end);

void set_config_flag(char* str, char val);

void get_cmds(char *str, char** argv[], int* argc);

void toggle_breakpoint(int bp);

void step_till_breakpoint(struct processor *p);

void check_watched(struct processor *p);

int set_watch(struct processor *p, int addr, int set);


#endif /* cavrr.h */
