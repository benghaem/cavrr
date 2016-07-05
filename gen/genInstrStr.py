print("#include \"instruction.h\"")
print("""char* instruction_str(enum instruction instr){
    switch(instr){"""
    )
with open("instructions-list.txt.gen", 'r') as f:
    instrList = []
    for instr in f:
        if instr not in instrList:
            instrList.append(instr)

    for instr in sorted(instrList):
        print("        case " + instr.strip() + ":")
        print("            return \"" + instr.strip() + "\";")

print("""    }
}"""
)
