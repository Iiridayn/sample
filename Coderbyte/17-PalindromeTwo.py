# Challenge: Palindrome tester, ignoring punctuation and spaces. 10 points, 4 min
# Since my old code handled it I just snagged it, upgraded it, tested it. I always tested my code, even copy-pasta - worth it for the 80-20 rule.

def PalindromeTwo(str):
  p = filter(lambda c: c.isalnum(), str.lower())
  # below clever trick from other palindrome solutions on site
  # not as efficient, but simple
  return "true" if p == p[::-1] else "false"

# keep this function call here  
# to see how to enter arguments in Python scroll down
print PalindromeTwo(raw_input())
