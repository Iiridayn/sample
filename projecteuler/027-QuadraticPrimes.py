import sys
from euler import is_prime

bound = int(sys.argv[1])
high = 0
value = None
for a in range(-bound, bound+1):
	for b in range(-bound, bound+1):
		i = 0
		while True:
			if not is_prime(i*i+a*i+b):
				break
			i += 1
		if i > high:
			high = i
			value = (a,b)
print(high, value, value[0]*value[1])
