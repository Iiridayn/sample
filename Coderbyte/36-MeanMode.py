# Challenge: Check if mode == mean, simple tests (1 mode, etc). 10 points, 3 mins
# copied my Mode code from earlier; still didn't know about sets

def MeanMode(arr):
  # section copied from my mode solution, same site
  mode = None
  count = 1
  for x in arr:
    # don't care about duplicates, small data sets
    # coding time dominates execution time, etc
    xcount = arr.count(x)
    if xcount > count:
      mode = x
      count = xcount

  if count > 1 and mode == sum(arr)/len(arr):
    return 1
  return 0

# keep this function call here
# to see how to enter arguments in Python scroll down
print MeanMode(raw_input())
