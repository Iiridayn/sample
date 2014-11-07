import sys

a,b = 1,1
i = 2
target = pow(10, int(sys.argv[1])-1)
while b < target:
	a,b = b,a+b
	i += 1
print(i)
