import sys

num = int(sys.argv[1])

def primes(search):
    primes = [2,3]
    yield primes[0]
    yield primes[1]

    i = 5
    while True:
        prime = True
        for p in primes:
            if p*p > i: break
            if not i%p:
                prime = False
                break
        if prime:
            primes.append(i)
            yield i
        i += 2
        if i*i > search: break

factors = []
for p in primes(num):
    if p*p > num: break
    if not num % p:
        factors.append(p)

print(factors)
