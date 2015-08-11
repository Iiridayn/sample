import itertools
from euler import is_prime
# Plan - there are 9! permutations of 9 digits - pandigital for n = 9.
# search these for the largest prime - if not in n = 9, count down

def max_pandigital_prime():
    for n in range(9, 0, -1):
        for p in itertools.permutations(range(n, 0, -1), n):
            value = int(''.join(map(str, p)))
            if is_prime(value):
                return(value)

print(max_pandigital_prime())
