# Challenge: Compute the factorial. 10 points, 3 mins

def FirstFactorial(num):
  fact = 1;
  for i in range(2, num+1):
    fact *= i
  return fact

# keep this function call here
# to see how to enter arguments in Python scroll down
print FirstFactorial(raw_input())
