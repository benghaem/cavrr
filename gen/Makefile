all: instrEnum.c.gen instrPrint.c.gen

instructions-list.txt.gen instrSelect.c.gen: instruction-primary-opcodes-no-strange-16b-no-MULS.csv
	python genInstrSelect.py instruction-primary-opcodes-no-strange-16b-no-MULS.csv

instrEnum.c.gen: instructions-list.txt.gen
	python genInstrEnum.py > instrEnum.c.gen

instrPrint.c.gen: instructions-list.txt.gen
	python genInstrPrint.py > instrPrint.c.gen
