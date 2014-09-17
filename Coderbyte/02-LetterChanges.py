# Challenge: rot1 + captialize all vowels in created string. 10 points, 10 minutes, failed testcase: "coderbyte"
# I failed to understand ranges properly - I had thought they were inclusive; ord('y')+1 would have fixed it.

def LetterChanges(str):
  changed = ""

  for c in str:
    c = ord(c)

    if c in range(ord('a'), ord('y')) or c in range(ord('A'), ord('Y')):
      c = chr(c+1)
    elif c == ord('z'):
      c = 'a'
    elif c == ord('Z'):
      c = 'A'
    else:
      c = chr(c)

    if c in ['a','e','i','o','u']:
      c = c.upper()

    changed += c

  return changed

# keep this function call here  
# to see how to enter arguments in Python scroll down
print LetterChanges(raw_input())
