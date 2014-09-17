# Challenge: Test if sequence is Aritmetic or Geometric. It won't be both (all same number). 10 points, 13 mins

def ArithGeo(arr):
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
print ArithGeo(raw_input())
