# Challenge: Check if num is a power of 2. 10 points, 3 mins

def PowersofTwo(num):
  while num % 2 == 0:
    num = num >> 1
  return 'true' if num == 1 else 'false'

# keep this function call here
# to see how to enter arguments in Python scroll down
print PowersofTwo(raw_input())
