import sys

memoize = {1: 1} # number: length of collatz sequence
def collatz(num):
    i = 0
    numbers = []
    while num not in memoize:
        numbers.append(num)
        i += 1

        if num % 2:
            num = 3*num+1
        else:
            num //= 2

    i += memoize[num]

    for num,length in zip(numbers,range(i,0,-1)):
        memoize[num] = length

    return i

testset = list(range(1, int(sys.argv[1])+1))
for i in testset: collatz(i)
print(max(testset, key=(lambda i: memoize[i])))
