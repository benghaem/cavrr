# Changelog

## 7/19/16
* Change build system to CMake

## 7/15/16
* Some minor changes to fix compiler warnings

## 7/14/16
* Add address printing with `print (reg | io | sram | raw) <addr>`
* Add helpful function to parse addresses (replaces logic for watch and print)
* Add tests for LDI, and STD_Z
* Add ubench command for simple benchmarking
* Switch from bit_test function to a macro of the same name. Improves performance ~50%
* Add -O2 to default compiler flags
* bugfix: fix offset problem with d=0 -> r16 in LDI

## 7/13/16

* Add address watching with `watch (reg | io | sram | raw) <addr>`
* Add barebones linked list to `util/list.c`
* bugfix: fix the makefile that builds the hex test programs to output correct objects