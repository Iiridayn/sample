# Challenge: Permuate digits of num, see if any are prime. 10 points, 5 mins, 1 failed test (num = 100)
# This is where the bug in PrimeTime when num = 1 got me.
import itertools

# copied from Prime Time challenge
def PrimeTime(num):
  if num in [2,3]: return True
  if not num % 2: return False
  if not num % 3: return False

  # credit: http://stackoverflow.com/a/1801446/118153
  # have read this algorithm various places, on clock though
  i = 5
  w = 2
  while i**2 < num: # only check to sqrt(num)
    if num % i == 0: return False
    i += w
    w = 6 - w

  return True

def PrimeChecker(num):
  str = `num`
  for subset in itertools.permutations(str):
    if PrimeTime(int(''.join(subset))):
      return 1
  return 0

# keep this function call here
# to see how to enter arguments in Python scroll down
print PrimeChecker(raw_input())
