# Challenge: Check if num is in the Fibbonacci seq. 10 points, 2 mins
# most other people used a nifty multiple assignment trick to save memory.

def FibonacciChecker(num):
  fib = [0,1]

  while fib[-1] < num:
    fib.append(fib[-2]+fib[-1])

  return 'yes' if num == fib[-1] else 'no'

# keep this function call here
# to see how to enter arguments in Python scroll down
print FibonacciChecker(raw_input())
