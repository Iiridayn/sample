# Challenge: Equal numbers of 'x' and 'o'. 10 points, 2 mins
# I thought my use of a signed int rather clever

def ExOh(str):
  tally = 0

  for c in str:
    if c == 'x': tally += 1
    elif c == 'o': tally -= 1

  return "true" if tally == 0 else "false"

# keep this function call here
# to see how to enter arguments in Python scroll down
print ExOh(raw_input())
