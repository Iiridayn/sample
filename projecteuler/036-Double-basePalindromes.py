import sys,itertools
from math import ceil

digits = int(sys.argv[1])

pal = []
for d in range(1, digits+1):
    mid = ceil(d/2)
    for c in itertools.product([str(i) for i in range(0,10)], repeat=mid):
        if c[0] == '0':
            continue
        p = ''.join(list(c))
        if d % 2:
            p = p+p[::-1][1:]
        else:
            p = p+p[::-1]

        p = int(p)
        b = bin(p)[2:]
        if b == b[::-1]:
            pal.append(p)

print(pal)
print(sum(pal))
