import fileinput

names = []
for line in fileinput.input():
	names += line.strip('"').split('","')
names.sort()

total = 0
base = ord('A')-1
for i in range(0, len(names)):
	total += (i+1)*sum([ord(c)-base for c in names[i]])
print(total)
