all: instrEnum.c.gen instrStr.c.gen

instructions-list.txt.gen instrSelect.c.gen: instruction-primary-opcodes-no-strange-16b-no-MULS.csv genInstrSelect.py
	python genInstrSelect.py instruction-primary-opcodes-no-strange-16b-no-MULS.csv

instrEnum.c.gen: instructions-list.txt.gen genInstrEnum.py
	python genInstrEnum.py > instrEnum.c.gen

instrStr.c.gen: instructions-list.txt.gen genInstrStr.py
	python genInstrStr.py > instrStr.c.gen
