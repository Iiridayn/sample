# Challenge: Swap case, then swap single digit numbers bracketing unbroken letters. 8 points, 19 mins
# I lost 2 points on time - I made several false starts :/

# This is horribly ugly
def SwapII(str):
  str = str.swapcase()

  # assume single digit numbers only for now
  numbers = range(ord('0'), ord('9')+1)
  out = []
  last = None
  letter = False
  for i in range(0, len(str)):
    c = str[i]
    if ord(c) in numbers:
      if last is None:
        last = i
      elif letter:
        tmp = c
        c = out[last]
        out[last] = tmp
        last = None
      else:
        last = None
      letter = False
    elif str[i].isalpha():
      letter = True
    else:
      letter = False
    out.append(c)

  return ''.join(out)

# keep this function call here
# to see how to enter arguments in Python scroll down
print SwapII(raw_input())
