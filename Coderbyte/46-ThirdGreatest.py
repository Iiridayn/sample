# Challenge: find 3rd largest word; first of third largest if multiple match. 10 points, 12 mins
# I had fun with this one :).

def ThirdGreatest(strArr):
  return filter(
    lambda s: len(s) == sorted(map(
      lambda s: len(s), strArr
    ))[-3],
    strArr
  )[-1]

# keep this function call here
# to see how to enter arguments in Python scroll down
print ThirdGreatest(raw_input())
