## cavrr: An ATtiny45 Emulator Written in C [![Build Status](https://travis-ci.org/benghaem/cavrr.svg?branch=master)](https://travis-ci.org/benghaem/cavrr)

This is my attempt to write a functional emulator of the ATtiny45 in C. My goal is to learn more about the AVR architecture and practice C in general. 

Currently the following high level concepts have been implemented:

* Instruction decoding (automatically generated)
* Memory interactions
* IO Register Naming
* Intel HEX file format decoding
* Basic processor execution loop

## Building

First create a bin directory:

`mkdir bin`

Then current emulator can be built using the included makefile

`make`

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
