import datetime
import itertools
import time


class outputBuffer():
    b = [""] * 10

    def printToBuff(self, i, s):
        self.b[i] = self.b[i] + str(s) + "\n"

    def writeToFile(self, i, filename):
        with open(filename, 'w') as f:
            f.write(self.b[i])
            f.close()

def getBinaryString(val, bits):
    b = format(val, 'b')
    bfull = '0' * (bits - len(b)) + b
    return bfull


def allSame(l):
    if len(l) > 0:
        last = l[0]
        for item in l:
            if item != last:
                return False
            last = item
    return True

def allSameOp(instrList, opMap):
    if len(instrList) > 0:
        last = opMap[instrList[0]]
        for item in instrList:
            if opMap[item] != last:
                return False
            last = opMap[item]
    return True

def capturesOpPart(op_part, cmp_op_part):
    captures = True
    for biti in range(4):
        if op_part[biti] != 'x':
            captures = captures & (op_part[biti] == cmp_op_part[biti])
    return captures


# combination, location 0,1,2,3, value of combination
def printBitTestArgs(comb, loc, val, offset, ob):
    valueString = ""
    maskString = ""
    fakeIndex = 0
    for index in range(4):
        if index in comb:
            valueString = valueString + str(val[fakeIndex])
            maskString = maskString + "1"
            fakeIndex = fakeIndex + 1
        else:
            valueString = valueString + "0"
            maskString = maskString + "0"

    intValue = int(valueString[0]) * 8 + int(valueString[1]) * 4 + int(valueString[2]) * 2 + int(valueString[3])
    intMask = int(maskString[0]) * 8 + int(maskString[1]) * 4 + int(maskString[2]) * 2 + int(maskString[3])

    if loc == 0 or loc == 2:
        intValue = intValue << 4
        intMask = intMask << 4

    base = ""
    if loc == 0 or loc == 1:
        base = "high"
    if loc == 2 or loc == 3:
        base = "low"
    ob.printToBuff(1, offset + "if(bit_test(" +base + ", " + str(hex(intValue)) + ", " + str(hex(intMask)) + ")){")




# from a list of instructions that could not be subdivided by the normal method
# we now attempt to select the important instructions from that set

# 1. Select instructions with the most free bits
# 2. Go across each slot and ensure that these instructions
#    capture all the other instructions
# 3. If they each only capture a subset determine the bits we can use to differentiate them



def selectionMethodTwo(instrList, opMap, level, ob):

    offset = "    " * level

    #print(offset + "trying method two on " + str(instrList))

    countMap = {}

    max_free = 0

    for instr in instrList:
        op = opMap[instr]
        free_total = sum([op_part.count('x') for op_part in op])
        countMap[instr] = free_total

        if free_total > max_free:
            max_free = free_total

    max_free_subset = []

    for key, value in countMap.items():
        if value == max_free:
            max_free_subset.append(key)

    captureMap = {}

    for instr in max_free_subset:
        for cmpInstr in max_free_subset:
            test = True
            op = opMap[instr]
            cmpOp = opMap[cmpInstr]
            for i in range(4):
      #          print("comparing ops", op[i], cmpOp[i])
                if not capturesOpPart(op[i], cmpOp[i]):
     #               print("does not capture")
                    test = False
                    break
            if test:
                captureMap[instr] = captureMap.get(instr, []) + [cmpInstr]

    #for key, value in captureMap.items():
    #   print(key, value)

    #print(offset + "method two produced this subset: " + str(max_free_subset))
    return max_free_subset

# select the location with the lowest number of free bits  next and then check
# for a set of static bits of size 4 - free bits
# check all combinations of these bits and then subdivide the sets by comb and
# rerun this same process

# Ex: free = x static = S
# SSSS SSxx xxxx SSSx
# free bits = 0 2 4 1
# free bits in first slot = 0
# select first slot
# find (4 - free bits) that are shared by all members
# check all combinations of these bits and subdivide
# ex 0000 -> []
#    0001 -> []
# if all items in subdivision are not the same
# rerun the process


def determineSelectionOrder(instrList, opMap, level, ob):

    printOffset = "    " * level

    #print(printOffset + " attempting to determine selection order on " + str(instrList))

    if not allSame(instrList):

        max_free = [0, 0, 0, 0]

        for instr in instrList:
            op = opMap[instr]
            free_counts = [op_part.count('x') for op_part in op]

            for i, val in enumerate(free_counts):
                if val > max_free[i]:
                    max_free[i] = val

        # list the indexes of the smallest value to the largest value
        index_ordering = [el[1] for el in sorted([(val, i) for i, val in enumerate(max_free)])]
        # print (printOffset + "max_free: " + str(max_free))
        for index_option in index_ordering:
            # print(printOffset + " using index: " + str(index_option))
            min_index = index_option
            static_count = 4 - max_free[min_index]

            working_comb = []

            combs = list(itertools.combinations(range(4), static_count))
            for comb in combs:
                validComb = True
                for instr in instrList:
                    selected_op_part = opMap[instr][min_index]
                    for comb_index in comb:
                        if selected_op_part[comb_index] == 'x':
                            validComb = False
                            break
                    if not validComb:
                        break
                if validComb:
                    working_comb = comb
                    break

            # print("Subdividing by op_part : bits")
            # print(str(min_index) + ": " + str(working_comb))

            successfullSearches = []
            newListsMap = {}

            # for x from 0 to 2^number of static bits
            for x in range(pow(2, len(working_comb))):
                # we should generate a binary number
                # and try to search our list by it
                search_results = []
                bits = getBinaryString(x, len(working_comb))
                for instr in instrList:
                    instructionMatches = True
                    working_op_part = opMap[instr][min_index]
                    for i, index_val in enumerate(working_comb):
                        if working_op_part[index_val] != bits[i]:
                            instructionMatches = False
                            break
                    if instructionMatches:
                        search_results.append(instr)

                if len(search_results) > 0:
                    successfullSearches.append(bits)
                newListsMap[bits] = search_results

            if len(successfullSearches) > 1:
                # print(printOffset + "newly subdivided lists")
                for k, v in sorted(newListsMap.items()):
                    if len(v) > 0:
                        printBitTestArgs(comb, min_index, k, printOffset, ob)
                        ob.printToBuff(1, printOffset +"/*"+ str(k)
                              + " on bits: " + str(comb)
                              + " from op part: "
                              + str(min_index) + "\n" + printOffset + "--> " + str(v) + "*/")
                        ret = determineSelectionOrder(v, opMap, level+1, ob)
                        if ret == 2 or ret == 3:
                            instrForPrint = "UNKNOWN"
                            if ret == 2:
                                instrForPrint = str(v[0])
                            if ret == 3:
                                if len(v) > 0:
                                    subset = selectionMethodTwo(v,opMap, level+1, ob)
                                    if len(subset) == 1:
                                        instrForPrint = subset[0]
                                    else:
                                        if allSameOp(subset,opMap):
                                            ob.printToBuff(1, printOffset + "// NOTE: Instruction codes equal")
                                            print("Multiple op-code equal options detected")
                                            for i2, v2 in enumerate(subset):
                                                print(i2, v2)
                                            print("input number to select")
                                            indexChoice = int(input())
                                            instrForPrint = v[indexChoice]
                                            print("selected: " + v[indexChoice])

                                        determineSelectionOrder(subset,opMap, level+1, ob)
                                    #print("Multiple options detected:")
                                    #for i2, v2  in enumerate(v):
                                    #    print(i2,v2)
                                    #print("Input number to select")
                                    #indexChoice = int(input())
                                    #instrForPrint = v[indexChoice]
                            ob.printToBuff(1, printOffset + "instr = " + instrForPrint + ";")
                            ob.printToBuff(2, instrForPrint)
                        ob.printToBuff(1, printOffset+"}")
                # done printing
                return 4
            elif len(successfullSearches) == 1:
                successSearch = successfullSearches[0]
                if len(newListsMap[successSearch]) == len(instrList):
                    continue
            break
        # no additional subsets
        return 3
    # all same
    return 2

instrList = []
opMap = {}

with open("instruction-primary-opcodes.csv", 'r') as f:

    for l in f:
        data = l.split(",")
        data = [x.strip() for x in data]
        instrList.append(data[0])
        opMap[data[0]] = data[1:]

    f.close()

ob = outputBuffer()

t = datetime.datetime.now()
datestr = time.strftime("%b %d, %Y @ %H:%M", t.timetuple())

ob.printToBuff(1, "// Generated on " + datestr + " by genInstrSelect.py")
determineSelectionOrder(instrList, opMap, 0,ob)

ob.writeToFile(1, "genoutput.c")
ob.writeToFile(2, "instructions-list.txt")
