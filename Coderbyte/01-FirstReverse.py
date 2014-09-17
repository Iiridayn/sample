# Challenge: reverse a string - 10 points, 5 mins

def FirstReverse(str):
  # this is horrible, but quick
  rev = ""
  for c in str:
    rev = c+rev
  return rev

# keep this function call here  
# to see how to enter arguments in Python scroll down
print FirstReverse(raw_input())
