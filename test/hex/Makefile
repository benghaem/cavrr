CC=avr-gcc
OBJC=avr-objcopy
CCFLAGS=-D__AVR_ATtiny45__
OBJCFLAGS=-j .text -j .data -O ihex
OBJCEEPROMFLAGS=-j .eeprom --change-section-lma .eeprom=0 -O ihex

HEX = bin/simpleoutput.hex bin/simplestatemachine.hex bin/simpleoutput_plusdelay.hex
EEPROMHEX = bin/eeprom_simpleoutput.hex bin/eeprom_simpleoutput_plusdelay.hex
OBJDUMPS = bin/simpleoutput.dump bin/simplestatemachine.dump bin/simpleoutput_plusdelay.dump


bin/%.o : src/%.c
	$(CC) $(CCFLAGS) $< -o $@

bin/%.hex : bin/%.o
	$(OBJC) $(OBJCFLAGS) $< $@

bin/eeprom_%.hex : bin/%.o
	$(OBJC) $(OBJCEEPROMFLAGS) $< $@

bin/%.dump : bin/%.o
	avr-objdump -S $< > $@

all: $(HEX) $(OBJDUMPS) $(EEPROMHEX)

clean:
	rm bin/*

.PHONY: all clean
