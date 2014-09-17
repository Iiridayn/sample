# Challenge: Check if parens are balanced. 10 points, 3 mins
# I caught a clever optimization by pushing the if into the elif when comparing other solutions after solving it.

def BracketMatcher(str):
  count = 0
  for c in str:
    if c == '(':
      count += 1
    elif c == ')':
      count -= 1
    if count < 0:
      return 0
  return 1 if count == 0 else 0

# keep this function call here
# to see how to enter arguments in Python scroll down
print BracketMatcher(raw_input())
