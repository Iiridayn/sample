import sys

limit = int(sys.argv[1])+1
powers = set()
for a in range(2, limit):
	for b in range(2, limit):
		powers.add(a**b)

print(len(powers))
