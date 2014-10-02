import sys,itertools
from euler import primes

num = int(sys.argv[1])
print(sum(itertools.takewhile(lambda p: p < num, primes())))
