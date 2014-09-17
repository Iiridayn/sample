# Challenge: exact same as previous. 10 points, 2 mins
import itertools

# another seemingly identical challenge :S
def ArrayAdditionI(arr):
  arr.sort()
  max = arr.pop()

  # brute force
  # credit: http://stackoverflow.com/q/17434070 for itertools.combinations
  for L in range(0, len(arr)+1):
    for subset in itertools.combinations(arr, L):
      if sum(subset) == max: return "true"

  return "false"

# keep this function call here  
# to see how to enter arguments in Python scroll down
print ArrayAdditionI(raw_input())
