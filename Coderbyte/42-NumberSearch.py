# Challenge: sum single digit numbers in string / by # letters (not chars). 10 points, 8 mins
# I didn't know about sum(array) yet

def NumberSearch(str):
  nums = filter(lambda c: c.isdigit(), str)
  letters = filter(lambda c: c.isalpha(), str)
  sum = 0
  for x in nums:
    sum += int(x)
  return int(round(float(sum)/len(letters)))

# keep this function call here
# to see how to enter arguments in Python scroll down
print NumberSearch(raw_input())
