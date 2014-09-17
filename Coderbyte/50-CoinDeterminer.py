# Challenge: make change using 1,5,7,9,11; input 1-250. 10 points, 11 mins. 1 failed test (100).
# seems to have hit an execution cutoff. This is why I usually try to avoid brute force solutions :[. I probably would have benefitted from starting big and backing down then up as needed instead of counting 100 pennies (never optmial)

def CoinDeterminer(num):
  # regular making change coins are at most /2 value
  # 14 is a good example of why not 5+9, or 11+1+1+1
  coins = [11,9,7,5,1]
  if num in coins: return 1

  min = 1000 # lazy infinity for problem space
  for c in coins:
    if c < num:
      count = 1 + CoinDeterminer(num - c)
      if count < min:
        min = count

  return min

# keep this function call here
# to see how to enter arguments in Python scroll down
print CoinDeterminer(raw_input())
