# Challenge: Reduce; 'ac' => 'b', 'aa' can't reduce, etc. Return min length via any possible reduction. 10 points, 12 mins
# Brute forced it. Fit recursion fairly well this time.

def StringReduction(str):
  lengths = [len(str)]

  # skipping last letter
  for i in range(0, len(str)-1):
    if str[i] != str[i+1]:
      letter = 'a'
      if str[i] == 'a' or str[i+1] == 'a':
        letter = 'b'
        if str[i] == 'b' or str[i+1] == 'b':
          letter = 'c'
      lengths.append(StringReduction(str[0:i]+letter+str[i+2:]))

  return min(lengths)

# keep this function call here
# to see how to enter arguments in Python scroll down
print StringReduction(raw_input())
