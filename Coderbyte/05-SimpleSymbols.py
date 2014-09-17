# Challenge: each letter in str must be bracketed by '+'. Score 9, 11 mins
# Failed 2 test cases - forgot to add a check in the else - "if state == 2: return "false"

def SimpleSymbols(str):
  state = 0 # 1 = plus 2 = letter
  for c in str:
    if c == '+':
      state = 1
    elif  (ord(c) in range(ord('a'), ord('z')+1)
        or ord(c) in range(ord('A'), ord('Z')+1)):
      if state != 1: return "false" # False
      state = 2
    else:
      state = 0 # unexpected characters are fine, per case 1 there

  return "true" # True - seems to expect string returns instead of boolean

# keep this function call here
# to see how to enter arguments in Python scroll down
print SimpleSymbols(raw_input())
