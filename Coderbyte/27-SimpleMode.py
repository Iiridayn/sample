# Challenge: Find the mode of an array of numbers. 10 points, 5 mins
# I later learned that I might have said "for x in set(arr):" instead.

def SimpleMode(arr):
  mode = None
  count = 1
  for x in arr:
    # don't care about duplicates, small data sets
    # coding time dominates execution time, etc
    xcount = arr.count(x)
    if xcount > count:
      mode = x
      count = xcount

  return mode if count > 1 else -1

# keep this function call here
# to see how to enter arguments in Python scroll down
print SimpleMode(raw_input())
