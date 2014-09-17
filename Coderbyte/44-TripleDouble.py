# Challenge: Check if any triples in str1 are doubles in str2. 10 points, 7 mins

def TripleDouble(num1,num2):
  triples = set()
  num = -1
  count = 0

  for x in str(num1):
    x = int(x)
    if x != num:
      count = 1
      num = x
    else:
      count += 1
    if count == 3:
      triples.add(x)

  num = -1
  for x in str(num2):
    x = int(x)
    if x != num:
      num = x
    elif x in triples:
      return 1

  return 0

# keep this function call here
# to see how to enter arguments in Python scroll down
print TripleDouble(raw_input())
