# Challenge: Permute the digits of num to create a minimally smaller number. 9 points, 16 mins.
# Lost a point for time. Maybe should have just brute forced it :/.

def PermutationStep(num):
  if num < 10: return -1
  str = `num`

  next = ''
  for i in range(2, len(str)+1):
    next = str[:-i]
    digits = sorted(list(str[-i:]))
    next += digits.pop() + ''.join(digits)
    if int(next) > num: return next

  return -1

# keep this function call here
# to see how to enter arguments in Python scroll down
print PermutationStep(raw_input())
