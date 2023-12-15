

def search():
    infile = open("AllDominoCases.csv")
    maxVal = -1
    maxLine = []

    for line in infile:
        splitline = line.split(",")
        val = int(splitline[0])
        if val == maxVal:
            maxLine.append(line)
        if val > maxVal:
            maxLine = []
            maxLine.append(line)
            maxVal = val

    print(maxVal)
    for line in maxLine:
        print(line)

def generate_e_slices():
    AEFS = ["U D' ", "U' D ", "U2 D2 "]
    THREECYCLES = ["R2 U D' F2 U' D ", "B2 U' D L2 U D' ", "R2 U2 D2 L2 U2 D2 ", "R2 L2 U D' F2 B2 U' D "]
    possibilities = []
    for a in AEFS:
        for b in THREECYCLES:
            for c in AEFS:
                possibilities.append(a+b+c)
                
    print((possibilities))
    for i in possibilities:
        print("./nissy solve -t 8 " + "\"" + i + "U D' R2 U D' F2 U' D U D' L2 B2 L2 U2 L2 F2 U B2 U' R2 U2 B2 U B2 U B2 U' F2\"")
    

generate_e_slices()
