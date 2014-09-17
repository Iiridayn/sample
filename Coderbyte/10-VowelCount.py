# Challenge: count the vowels except y. 10 points, 3 min

def VowelCount(str):
  return len(filter(lambda c: c in ['a','e','i','o','u'], str.lower()))

# keep this function call here
# to see how to enter arguments in Python scroll down
print VowelCount(raw_input())
