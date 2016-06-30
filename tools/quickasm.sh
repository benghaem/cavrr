#!/bin/bash

read ASM

echo $ASM | avr-as -o tmp.obj
avr-objdump -d tmp.obj
rm tmp.obj

