import math

def p(n,r):
    return math.factorial(n)/math.factorial(n-r)

def c(n,r):
    return p(n,r)/math.factorial(r)

# number of card querying, number of card in deck, total drawn, deck size
def h(q, n, d, s=60):
    return (c(n, q)*c(s-n, d-q))/c(s, d)
