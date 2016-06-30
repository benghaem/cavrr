## cavrr: An ATTiny45 Emulator Written in C

This is my attempt to write a functional emulator of the ATTiny45 in C. My goal is to learn more about the AVR architecture and practice C in general. 

Currently the following high level concepts have been implemented:

* Instruction decoding (automatically generated)
* Memory interactions
* IO Register Naming
* Intel HEX file format decoding
* Basic processor execution loop

## Building

The test program can be built using the included makefile

`make`

## Instruction Listing

Here is a list of the currently implemented instructions.
An instruction followed by a (p) denotes a partial implemention.

* ADD
* BREAK (p)
* NOP


### Partial Implementation Details

* BREAK -> Does not check lock bits or JTAGEN / OCDEN fuses before entering
stopped state
