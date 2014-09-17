# Challenge: String-based run length encoding. 10 points, 5 mins

def RunLength(str):
  s = '' # state - current character
  n = 0 # number of said char found
  out = ""

  for c in str+' ': # cheating again :P, last char is being ignored.
    if n and c != s: # we have a thing, output it
      out += `n`+s
      n = 0
    s = c
    n += 1

  return out

# keep this function call here
# to see how to enter arguments in Python scroll down
print RunLength(raw_input())
