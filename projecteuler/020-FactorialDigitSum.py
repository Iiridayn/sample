import sys
from math import factorial

# Python's native support for bignums is nice.
print(sum([int(i) for i in str(factorial(int(sys.argv[1])))]))
