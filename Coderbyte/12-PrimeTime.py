# Challenge: Check if short is prime. 10 points, 12-15? mins
# I return the wrong value for num = 1 :/. Caught me in a later challenge.

# resubmitted this one due to a simple typo,
# so the time is a bit off. Line 19 had
# "n % i" instead of "num % i" and I hadn't
# re-tested since that edit. I think I was
# around 12-15 mins on this one oringally.
# I decided to be clever instead of lazy...
def PrimeTime(num):
  if num in [2,3]: return "true"
  if not num % 2: return "false"
  if not num % 3: return "false"

  # credit: http://stackoverflow.com/a/1801446/118153
  # have read this algorithm various places, on clock though
  i = 5
  w = 2
  while i**2 < num: # only check to sqrt(num)
    if num % i == 0: return "false"
    i += w
    w = 6 - w

  return "true"

# keep this function call here
# to see how to enter arguments in Python scroll down
print PrimeTime(raw_input())
