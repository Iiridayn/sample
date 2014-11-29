import itertools,sys
from euler import is_prime

circular = {2,3,5,7} # 1 digit primes
for d in range(2, int(sys.argv[1])+1):
    for c in itertools.product(['1','3','7','9'], repeat=d):
        p = ''.join(list(c))
        if int(p) in circular:
            continue
        cycle = set([int(p)])
        for i in range(0,len(p)):
            if not is_prime(int(p)):
                cycle = set()
                break
            p = p[1:]+p[0]
            cycle.add(int(p))
        circular |= cycle

print(sorted(list(circular)))
print(len(circular))
