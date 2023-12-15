

CORE_COUNT = 8

files = []
outfile = open("AllDominoCases.csv", 'w')

for i in range(CORE_COUNT):
    files.append(open(str(i) + ".csv", 'r'))

num = 0

while True:
    fullstring = ""
    for i in range(CORE_COUNT):
        line = files[i].readline()
        outfile.write(line)
        fullstring += line
        num+=1
        
    if num % 10000 == 0:
        print("{}%".format(num/16257023.99))
    if fullstring == "":
        break
