import sys,itertools
sys.path.append("../projecteuler")
from euler import primes

low = int(sys.argv[1])
high = int(sys.argv[2])
print(sum(itertools.islice(primes(), low-1, high)))
