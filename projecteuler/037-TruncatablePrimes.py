import itertools
from euler import is_prime

def truncatables():
    truncatable = []
    for d in itertools.count(2):
        # similar to 035 - 1379 only digits allowed - wrong; can start w/2,5
        for c0 in ['2','3','5','7']:
            for c in itertools.product(['1','3','7','9'], repeat=d-1):
                p = c0+''.join(list(c))
                if int(p) in truncatable or not is_prime(int(p)):
                    continue

                trunc = True
                for i in range(1,d):
                    if not is_prime(int(p[i:])) or not is_prime(int(p[:d-i])):
                        trunc = False
                        break
                if trunc:
                    truncatable.append(int(p))
                if len(truncatable) == 11:
                    return truncatable

truncatable = truncatables()
print(truncatable)
print(sum(truncatable))
