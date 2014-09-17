# Challenge: Add multi-digit numbers in string. 10 points, 4 mins
# Still using that annoying +' ' trick. Other than pasting code, how should I do it right?

def NumberAddition(str):
  sum = 0
  num = ''
  for c in str+' ': # lazy
    if c.isdigit():
      num += c
    elif num:
      sum += int(num)
      num = ''
  return sum

# keep this function call here
# to see how to enter arguments in Python scroll down
print NumberAddition(raw_input())
