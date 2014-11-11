import sys

power = int(sys.argv[1])

# Establish a loose upper search bound - x digits, all 9's
high = None
digits = 0
while True:
	digits += 1
	high = digits*pow(9, power)
	if high < pow(10, digits):
		break
#print(digits, high)

powered = []
for i in range(2, high):
	if i == sum([pow(int(c), power) for c in str(i)]):
		powered.append(i)

print(powered)
print(sum(powered))
