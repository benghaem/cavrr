CC=avr-gcc
OBJC=avr-objcopy
CCFLAGS=-mmcu=avr25 -D__AVR_ATtiny45__
OBJCFLAGS=-j .text -j .data -O ihex

HEX = bin/simpleoutput.hex

bin/%.o : src/%.c
	$(CC) $(CCFLAGS) -c $< -o $@

bin/%.hex : bin/%.o
	$(OBJC) $(OBJCFLAGS) $< $@

all: $(HEX)
