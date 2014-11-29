import itertools
# must be 2 digits or not a sum, so 1! and 2! are out - smallest is 2 digits
# 0! = 1; 9! = 362880; loose upper bound then is 9 digits (9!*9 = 3265920)
# but anything with a >= 5 < 3 digits won't work; skip those, etc
# should be faster to go from factorials -> numbers; fewer tests
fact = [1,1,2,6,24,120,720,5040,40320,362880] # precomputed 0-9

numbers = []
for digits in range(2,10):
    upper = pow(10,digits)
    for i in range(0,10):
        if fact[i] > upper:
            i -= 1
            break
    subset = fact[1:i+1] # 0! = 1!, don't need it twice

    candidates = itertools.combinations_with_replacement(subset, digits)
    for c in candidates:
        total = sum(list(c))
        if total < upper/10:
            continue # we only care about n digit numbers (or we get 2, etc)
        check = 0
        for d in str(total):
            check += fact[int(d)]
        if check == total:
            numbers.append(total)

print(sum(numbers))
