# Challenge: 100% identical to the last. 10 points, 1 min
# I copied and pasted with minimal testing.

def ArithGeoII(arr):
  if len(arr) < 3: return -1 # not a meaningful sequence

  diff = arr[1] - arr[0]
  div = arr[1] / arr[0]

  for x,y in zip(arr[1:], arr[2:]):
    if diff and y-x != diff: diff = None
    if div and y/x != div: div = None
    if not div and not diff: return -1

  return "Arithmetic" if diff else "Geometric"

# keep this function call here  
# to see how to enter arguments in Python scroll down
print ArithGeoII(raw_input())
