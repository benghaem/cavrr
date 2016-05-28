CC=clang
CCFLAGS=-g

all: bitutil instruction


# bitutil related #

bitutil: bin/bitutil.o

bin/bitutil.o: util/bitutil.h util/bitutil.c
	$(CC) $(CCFLAGS) -c util/bitutil.c -o bin/bitutil.o

###################

# Instruction Related #

instruction: bin/instruction_decoder.o bin/instruction_print.o

bin/instruction_decoder.o: instruction.h instruction_decoder.c
	$(CC) $(CCFLAGS) -c instruction_decoder.c -o bin/instruction_decoder.o

bin/instruction_print.o: instruction.h instruction_print.c
	$(CC) $(CCFLAGS) -c instruction_print.c -o bin/instruction_print.o

#######################


clean:
	rm -rf bin/*

.PHONY: all clean bitutil instruction
