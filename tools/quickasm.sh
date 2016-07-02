#!/bin/bash

read ASM

echo $ASM | avr-as -m attiny45 -o tmp.obj
avr-objdump -d tmp.obj
rm tmp.obj

