# Challenge: Check if 'a' and 'b' are exactly 3 characters apart. 10 points, 12 minutes
# Failed the test case "bzzza", as I wasn't careful with the spec.

ABCheck(str):
  i = 0
  while True:
    i = str.find('a', i)
    if i == -1: break

    if i+4 < len(str) and str[i+4] == 'b': return "true"
    i += 1
  return "false"

# keep this function call here
# to see how to enter arguments in Python scroll down
print ABCheck(raw_input())
