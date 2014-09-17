# Challenge: sum multiples of 3,5 < num. 9 points, 7 mins. 2 failed tests.
# Failed 31 and 17. I wasn't supposed to double count 15, 30, etc. This is another case where trying to be clever (minimize cycles) bit me :/.

def ThreeFiveMultiples(num):
  sum = 0

  for i in range(1, (num-1)//3+1):
    sum += 3*i
  for i in range(1, (num-1)//5+1):
    sum += 5*i

  return sum

# keep this function call here
# to see how to enter arguments in Python scroll down
print ThreeFiveMultiples(raw_input())
