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

Run the binary at `bin/cavrr`. 

`cavrr [filename]`

Files may be loaded by either passing a filename as an argument or using the load command.
Program execution may be controlled through the built-in debugger.

The commands have not been formalized, but at the time of writing the following commands are usable:

* run [filename] - run the program in program memory or load and run
* load &lt;filename&gt; - load a program from file
* step [n] - step forward 1 or n instructions
* bt [addr] - set breakpoint at current pc address or another address in program memory
* local - display instructions in program memory near the program counter
* set &lt;flag&gt; &lt;value&gt; - set the value of a config flag
* dbe  - enable processor debug output
* ppc  - print program counter
* clear - clear the output (Terminals with ANSI support only)
* exit - exit the program

### Available Config Flags

* run_ignores_break - instruct the run command to ignore breakpoints (minor speed boost) [ default = 0 ]
* no_reset_on_load - when loading a program from file do not reset the processor state [ default = 0 ]

## Test Suite

The automated test suite can be built by running:

`make test`

(Requires [cmocka](https://github.com/clibs/cmocka))

## Instruction Listing

Here is a list of the currently implemented instructions.

(p) -&gt; Partial implementation

(!) -&gt; No automated tests

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

* BREAK -&gt; Does not check lock bits or JTAGEN / OCDEN fuses before entering
stopped state
