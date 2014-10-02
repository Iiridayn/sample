import sys
from euler import primes

num = int(sys.argv[1])

factors = []
for p in primes(num):
    if p*p > num: break
    if not num % p:
        factors.append(p)

print(factors)
