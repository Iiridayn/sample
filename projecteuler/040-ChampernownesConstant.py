import sys,functools,operator

exponent = int(sys.argv[1])

# (really) dumb way - speed of solution > failing cleverness
champ = ''
number = 1
values = []
for i in range(0, exponent+1):
    index = pow(10, i)
    while(len(champ) < index+1):
        champ += str(number)
        number += 1
    values.append(int(champ[index-1]))
print(functools.reduce(operator.mul, values))
