import sys

def ThreeFiveMultiples(num):
  numbers = set()

  for i in range(1, (num-1)//3+1):
    numbers.add(3*i)
  for i in range(1, (num-1)//5+1):
    numbers.add(5*i)

  return sum(numbers)

print(ThreeFiveMultiples(int(sys.argv[1])))
