# Challenge: Return how many numbers are missing from min to max. 10 points, 3 mins

def Consecutive(arr):
  arr.sort()
  sum = 0
  prev = arr.pop(0)
  for i in arr:
    sum += i-prev-1
    prev = i
  return sum

# keep this function call here
# to see how to enter arguments in Python scroll down
print Consecutive(raw_input())
