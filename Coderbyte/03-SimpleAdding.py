# Challenge: compute the sum of all numbers from 1 to num. 10 points, 1 minute
# I _really_ should have taken just a little more time to remember the n*(n-1)/2 trick :P

def SimpleAdding(num):
  return sum(range(1, num+1))

# keep this function call here 
# to see how to enter arguments in Python scroll down
print SimpleAdding(raw_input())
