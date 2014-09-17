# Challenge: Count duplicates in array. 10 points, 1 min
# Sometimes being clever pays off

def DistinctList(arr):
  return len(arr)-len(set(arr))

# keep this function call here
# to see how to enter arguments in Python scroll down
print DistinctList(raw_input())
