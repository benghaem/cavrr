CC=clang
CCFLAGS=-g -ansi -pedantic-errors

all: bin/memory.o bitutil instruction bin/processor.o bin/intelhex.o bin/processor_test

core: bin/memory.o bitutil instruction bin/processor.o bin/intelhex.o

# memory related

bin/memory.o: memory.c memory.h
	$(CC) $(CCFLAGS) -c memory.c -o bin/memory.o

##################


# bitutil related #

bitutil: bin/bitutil.o

bin/bitutil.o: util/bitutil.h util/bitutil.c
	$(CC) $(CCFLAGS) -c util/bitutil.c -o bin/bitutil.o

###################

# Instruction Related #

instruction: bin/instruction_decoder.o bin/instruction_str.o

bin/instruction_decoder.o: instruction.h instruction_decoder.c
	$(CC) $(CCFLAGS) -c instruction_decoder.c -o bin/instruction_decoder.o

bin/instruction_str.o: instruction.h instruction_str.c
	$(CC) $(CCFLAGS) -c instruction_str.c -o bin/instruction_str.o

#######################

# Processor Related #

bin/processor.o: processor.c processor.h
	$(CC) $(CCFLAGS) -c processor.c -o bin/processor.o


#######################

# Intel HEX

bin/intelhex.o: util/intelhex.c util/intelhex.h
	$(CC) $(CCFLAGS) -c util/intelhex.c -o bin/intelhex.o


######################

# Processor test program

bin/processor_test: test/processor_test.c core
	$(CC) $(CCFLAGS) test/processor_test.c bin/*.o -o bin/processor_test


clean:
	rm -rf bin/*

.PHONY: all clean bitutil instruction core
