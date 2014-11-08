import sys,itertools
from euler import primes

num = int(sys.argv[1])

factors = []
for p in itertools.takewhile(lambda x: x<num, primes()):
    if p*p > num: break
    if not num % p:
        factors.append(p)

print(factors)
