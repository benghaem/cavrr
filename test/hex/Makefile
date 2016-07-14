CC=avr-gcc
OBJC=avr-objcopy
CCFLAGS=-D__AVR_ATtiny45__
OBJCFLAGS=-j .text -j .data -O ihex

HEX = bin/simpleoutput.hex bin/simplestatemachine.hex bin/simpleoutput_plusdelay.hex
OBJDUMPS = bin/simpleoutput.dump bin/simplestatemachine.dump bin/simpleoutput_plusdelay.dump


bin/%.o : src/%.c
	$(CC) $(CCFLAGS) $< -o $@

bin/%.hex : bin/%.o
	$(OBJC) $(OBJCFLAGS) $< $@

bin/%.dump : bin/%.o
	avr-objdump -S $< > $@

all: $(HEX) $(OBJDUMPS)

clean:
	rm bin/*

.PHONY: all clean
