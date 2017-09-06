import os

files = []

for file in os.listdir():
    if file.endswith(".h"):
        print(file)
        files.append(file)

for file in os.listdir():
    if file.endswith(".cpp"):
        print(file)
        files.append(file)


with open('allcodes.txt', 'w') as outfile:
    for fname in files:
        with open(fname) as infile:
            outfile.writelines(["-"*80, "\n", fname, "\n", "-"*80, "\n"])
            outfile.write("\n")
            outfile.write(infile.read())
            outfile.write("\n\n\n")
