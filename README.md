## cavrr: An ATtiny45 Emulator Written in C [![Build Status](https://travis-ci.org/benghaem/cavrr.svg?branch=master)](https://travis-ci.org/benghaem/cavrr)

This is an emulator of the ATtiny45 in C. My goal is to learn more about the AVR architecture and practice C in general.

Currently the following high level concepts have been implemented:

* A simple debugger
* Instruction decoding (automatically generated)
* Memory interactions
* IO Register Naming
* Intel HEX file format decoding
* Basic processor execution loop

## Building

First create a bin directory:

`mkdir bin`

And build the project

`make`

## Usage

Run the binary at `bin/cavrr`. Currently the program requires one argument: a file to load.

For example: `bin/cavrr test/hex/bin/simpleoutput.hex`

Program execution may be controlled through the built-in debugger.

The commands have not been formalized, but at the time of writing the following commands are usable:

* exit - exit the program
* dbe  - enable processor debug output
* run  - run the program in program memory
* step - step forward one instruction
* local - display instructions in program memory near the program counter

## Test Suite

The automated test suite can be built by running:

`make test`

(Requires [cmocka](https://github.com/clibs/cmocka))

## Instruction Listing

Here is a list of the currently implemented instructions.

(p) -> Partial implementation

(!) -> No automated tests

* ADD
* BREAK (p)
* COM (!)
* IN (!)
* LD Z / LDD Z + q (!)
* LDI (!)
* MOV
* MOVW (!)
* NOP
* PUSH
* RJMP
* ST Z / STD Z + q (!)

### Partial Implementation Details

* BREAK -> Does not check lock bits or JTAGEN / OCDEN fuses before entering
stopped state
