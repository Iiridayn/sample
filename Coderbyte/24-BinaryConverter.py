# Challenge: Convert string binary to decimal. 10 points, 7 mins
# I'm being "clever" again. No way that's going to bite me... :P

def BinaryConverter(str):
  binary = filter(lambda c: c in ['1', '0'], str)
  decimal = 0
  for c in binary:
    decimal = (decimal << 1) + (c == '1')
  return decimal

# keep this function call here
# to see how to enter arguments in Python scroll down
print BinaryConverter(raw_input())
