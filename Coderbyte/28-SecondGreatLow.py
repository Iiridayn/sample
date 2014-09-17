# Challenge: Return second lowest and second greatest number. 9 points, 3 mins. Failed 2 test cases, due to not handling duplicates.

def SecondGreatLow(arr):
  arr.sort()
  return `arr[1]`+' '+`arr[-2]`

# keep this function call here
# to see how to enter arguments in Python scroll down
print SecondGreatLow(raw_input())
