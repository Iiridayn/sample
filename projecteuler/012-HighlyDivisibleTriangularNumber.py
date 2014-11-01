import sys
from math import ceil
from euler import divisors

def triangular():
    i = 1
    number = 0
    while True:
        number += i
        i += 1
        yield number

target = int(sys.argv[1])
for number in triangular():
    div = divisors(number)
    if len(div) > target:
        print(number, sorted(list(div)))
        break
