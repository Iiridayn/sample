# Challenge: Return nth prime. 7 points, 25 mins. All test cases correct.
# This happens when I rewrite 3 times due to attempted cleverness. I finally gave up and just used the sieve.

def PrimeMover(num):
  primes = [2,3]
  if num <= 2: return primes[num-1]

  i = 5
  while len(primes) < num:
    prime = True
    for p in primes:
      if not i%p:
        prime = False
        break
    if prime: primes.append(i)

    i += 2 # could be more efficient if I understood that 6k +- 1 thing I think

  return primes[-1]

# keep this function call here
# to see how to enter arguments in Python scroll down
print PrimeMover(raw_input())
