from math import ceil

def primes():
    '''credit: http://stackoverflow.com/a/3796442/118153 erat3'''
    import itertools as it
    D = { 9: 3, 25: 5 }
    yield 2
    yield 3
    yield 5
    MASK= 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0,
    MODULOS= frozenset( (1, 7, 11, 13, 17, 19, 23, 29) )

    for q in it.compress(
            it.islice(it.count(7), 0, None, 2),
            it.cycle(MASK)):
        p = D.pop(q, None)
        if p is None:
            D[q*q] = q
            yield q
        else:
            x = q + 2*p
            while x in D or (x%30) not in MODULOS:
                x += 2*p
            D[x] = p

def fibs():
    '''Simple fibonacci generator'''
    a,b = 0,1
    while True:
        yield a
        yield b
        a,b = b,a+b

def is_prime(n):
    '''Semi-efficient naive trial division'''
    # credit: https://en.wikipedia.org/wiki/Primality_test
    if n <= 3:
        return n >= 2
    if not n % 2 or not n % 3:
        return False

    for i in range(5, ceil(pow(n,0.5)), 6):
        if not n % i or not n % (i+2):
            return False
    return True

def divisors(num):
    '''Naive function to calculate divisors of an integer'''
    div = {1,num}
    for i in range(2, ceil(pow(num,0.5))):
        if not num % i:
            div.add(i)
            div.add(num//i)
    return div
