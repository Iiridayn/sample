import sys,itertools
from euler import primes

num = int(sys.argv[1])
print(next(itertools.islice(primes(), num-1, num)))
