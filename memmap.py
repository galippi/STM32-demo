import re
from ast import literal_eval
import sys

ranges = [[0x08000000, 0x0000c000, "ROM"],
          [0x20002000, 0x00003000, "RAM"]]
used = dict()

def rangeCheck(addr):
    for i in range(0, len(ranges)):
        if (addr >= ranges[i][0]) and (addr < (ranges[i][0] + ranges[i][1])):
            return i
    return -1

# main
filename = sys.argv[1]
#print("filename=" + filename)
mapFile = open(filename, "r")
lines = mapFile.readlines()
mapFile.close()
#print("len: " + str(len(lines)))
#print("2: " + lines[2])

delimiterStr = "Linker script and memory map"
for i in range(0, len(lines)):
    if (lines[i].startswith(delimiterStr)):
        break
#print("i=" + str(i))

for j in range(i + 2, len(lines)):
    if (lines[j][0] == '.'): # section - .rodata         0x08000000      0x1f0
        #print("j=" +str(j) + ":" + lines[j])
        x = re.split("\s+", lines[j])
        #print(x)
        if (len(x) < 3):
            # raise Exception("Invalid section format in line " + str(j + 1) + ": " + lines[j])
            continue
        addr = literal_eval(x[1])
        sectionLen = literal_eval(x[2])
        idx = rangeCheck(addr)
        if (idx >= 0) and (sectionLen > 0):
            addrEnd = addr + sectionLen
            idxStr = str(idx)
            if not(idxStr in used):
                used[idxStr] = [addr, addrEnd]
            else:
                r = used[idxStr]
                if (addr < r[0]):
                    r[0] = addr
                if (addrEnd > r[1]):
                    r[1] = addrEnd
            #print(x[0] + ":" + hex(addr) + "," + hex(sectionLen) + " - " + str(used[idxStr]))
            #raise Exception("vege")

#print("Result: " + str(used))
#keysUsed = used.keys()
#print("keysUsed: " + str(keysUsed))

print("Used memories:")
for j in range(0, len(ranges)):
    r = used[str(j)]
    perc = (r[1] - r[0]) * 100 / ranges[j][1]
    #print("Section " + hex(ranges[j][0]) + f": {perc:.2f}")
    print("  Section " + ranges[j][2] + f": {perc:.1f}%")

#end
