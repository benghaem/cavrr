with open("instructions-list.txt.gen", 'r') as f:
    instrList = []
    for instr in f:
        if instr not in instrList:
            instrList.append(instr)

    print("typedef enum{")
    for instr in sorted(instrList):
        print("    "+instr.strip()+",")
    print("} INSTRUCTION;")
