import sys,itertools

num = int(sys.argv[1])

def primes():
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

print(next(itertools.islice(primes(), num-1, num)))
