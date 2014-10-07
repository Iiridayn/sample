# too slow for large argv[1]
import sys

def collatz(num):
    '''Naive collatz sequence generator, assuming Collatz conjecture'''
    yield num
    while num > 1:
        if num % 2:
            num = 3*num+1
        else:
            num //= 2
        yield num

# use list comprehension to produce lengths of collatz sequences
# from 1 to argv[1]; find max and print
print(max([len(list(collatz(i))) for i in range(1, int(sys.argv[1])+1)]))
