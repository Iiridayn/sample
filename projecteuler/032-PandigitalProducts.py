import itertools
# upper bound < 99*999 = 98901 - this is 10 digits
# lower bound > 1*1111 = 1111 - total 9 digits
# worth noting: 99*99 and 9*999 are both < 9 digits total, 111*111 > 9
# 9! = 362880 - but checking permutations should be faster
products = set()
for p in itertools.permutations("123456789", 9):
    target = sorted(p[5:])
    tests = [
        (p[0:1],p[1:5]), (p[0:4],p[4:5]),
        (p[0:2],p[2:5]), (p[0:3],p[3:5]),
    ]
    for test in tests:
        product = int(''.join(test[0]))*int(''.join(test[1]))
        if sorted(str(product)) == target:
            products.add(product)
print(products)
print(sum(products))
