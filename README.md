## cavrr: An ATtiny45 Emulator Written in C [![Build Status](https://travis-ci.org/benghaem/cavrr.svg?branch=master)](https://travis-ci.org/benghaem/cavrr)

This is an emulator of the ATtiny45 in C. My goal is to learn more
about the AVR architecture and practice C in general.

Currently the following high level concepts have been implemented:

* A simple debugger
* Instruction decoding (automatically generated)
* Memory interactions
* IO Register Naming
* Intel HEX file format decoding
* Basic processor execution loop

## Building

Building the project depends on [cmake](https://github.com/Kitware/CMake)
and the AVR toolchain if you would like build test programs.

You can enable or disable test program compilation by setting the option when
you run CMake. By default the option is off.

First create a build directory:

`mkdir build && cd build`

And build the project

`cmake .. && make` or `cmake -DCOMPILE_IHEX=ON .. && make`

You can also run the tests with

`ctest -V` or `make test`

## Usage

Run the binary at `build/bin/cavrr`.

`cavrr [filename]`

Files may be loaded by either passing a filename as an argument or using the load command.
Program execution may be controlled through the built-in debugger.

The commands have not been formalized, but at the time of writing the following commands are usable:

* run [filename] - run the program in program memory or load and run
* load &lt;filename&gt; - load a program from file
* step [n] - step forward 1 or n instructions
* bt [addr] - set breakpoint at current pc address or another address in program memory
* watch (reg | io | sram | raw) &lt;addr&gt; - watch a memory location for changes. reg, io, sram, and 
raw each provide address offsets for easy access to those spaces. Works with run the and step commands (Make sure to set a breakpoint if the target program contains any infinite loops)
* show (reg | io | sram | raw) &lt;addr&gt; - print out the value of a memory location
* local - display instructions in program memory near the program counter
* set &lt;flag&gt; &lt;value&gt; - set the value of a config flag
* dbe  - enable processor debug output
* ppc  - print program counter
* clear - clear the output (Terminals with ANSI support only)
* ubench &lt;instr_count&gt; - run a simple benchmark over instr_count instructions
* exit - exit the program

### Available Config Flags

* run_ignores_break - instruct the run command to ignore breakpoints (minor speed boost) [ default = 0 ]
* reset_on_load - when loading a program from file reset the processor state [ default = 1 ]
* enable_step_delay - delay each processor step by some amount of time [ default = 0 ]
* step_delay_ms - step delay in ms [ default = 200 ]


## Testing

The automated test suite can be built by running:

`make test` from within the build directory.

If you enabled the test programs the output files in Intel HEX format are available
at `build/test/hex/` you can also place new files to build
in `test/hex/src/`

(Requires [cmocka](https://github.com/clibs/cmocka))

## Instruction Listing

Here is a list of the currently implemented instructions.

(p) -&gt; Partial implementation

(!) -&gt; No automated tests

* ADD
* BRBS \[BREQ, BRCS, BRLO, BRMI, BRLT, BRHS, BRTS, BRVS, BRIE\] (!)
* BRBC \[BRNE, BRCC, BRSH, BRPL, BRGE, BRHC, BRTC, BRVC, BRID\] (!)
* BREAK (p)
* COM (!)
* CP (!)
* CPC (!)
* EOR (!)
* IN (!)
* LD Z / LDD Z + q (!)
* LDI
* MOV
* MOVW (!)
* NOP
* OUT (!)
* PUSH
* RCALL (!)
* RJMP
* ST Z
* STD Z + q (!)

### Partial Implementation Details

* BREAK -&gt; Does not check lock bits or JTAGEN / OCDEN fuses before entering
stopped state
