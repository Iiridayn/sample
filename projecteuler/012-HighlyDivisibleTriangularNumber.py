import sys
from math import ceil

def triangular():
    i = 1
    number = 0
    while True:
        number += i
        i += 1
        yield number

def divisors(num):
    '''Naive function to calculate divisors of an integer'''
    div = {1,num}
    for i in range(2, ceil(pow(num,0.5))):
        if not num % i:
            div.add(i)
            div.add(num//i)
    return div

target = int(sys.argv[1])
for number in triangular():
    div = divisors(number)
    if len(div) > target:
        print(number, sorted(list(div)))
        break
