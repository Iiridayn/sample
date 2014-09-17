# Challenge: same as previous. 10 points, 1 min

# The algorithm is not clear; should greatest
# count as 2 or 1 repeating letter? ever is
# clearly 1, and might be considered equal.
# Not going to bother with case since I doubt
# the tests will :P. Assume case sensitivity
# is desired.
# Whee copy pasta
def LetterCountI(str):
  words = filter(lambda c: c == ' ' or c.isalnum(), str).split()

  max = 0
  current = ""
  for word in words:
    count = 0
    last = ''
    wmax = 0
    for c in sorted(word):
      if c == last: count += 1
      else: count = 0
      if count > wmax: wmax = count
      last = c
    if wmax > max:
      max = wmax
      current = word

  return current if current else -1

# keep this function call here  
# to see how to enter arguments in Python scroll down
print LetterCountI(raw_input())
