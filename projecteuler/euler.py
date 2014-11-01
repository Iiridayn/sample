from math import ceil

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

def divisors(num):
    '''Naive function to calculate divisors of an integer'''
    div = {1,num}
    for i in range(2, ceil(pow(num,0.5))):
        if not num % i:
            div.add(i)
            div.add(num//i)
    return div
