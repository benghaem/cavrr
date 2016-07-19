all: instrEnum.c.gen instrStr.c.gen

instructions-list.txt.gen instrSelect.c.gen: instr-subset-opcodes.csv genInstrSelect.py
	python genInstrSelect.py instr-subset-opcodes.csv

instrEnum.c.gen: instructions-list.txt.gen genInstrEnum.py
	python genInstrEnum.py > instrEnum.c.gen

instrStr.c.gen: instructions-list.txt.gen genInstrStr.py
	python genInstrStr.py > instrStr.c.gen
