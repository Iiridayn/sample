# Challenge: Check if palindrome, ignore spaces. 10 points, 8 mins
# I was embarrassed to learn that p==p[::-1] would have worked instead

def Palindrome(str):
  p = filter(lambda c: c.isalnum(), str)
  for i in range(0, len(p)//2):
    if p[i] != p[-i-1]: return "false"
  return "true"

# keep this function call here
# to see how to enter arguments in Python scroll down
print Palindrome(raw_input())
