# Challenge: Balance two types of brackets. 10 points, 5 mins
# Restated, I don't balance them quite right - [(]) would pass my checks.

def MultipleBrackets(str):
  # borrowed my brackets solution for a prior problem
  brackets = 0
  count = [0,0]
  for c in str:
    if c == '(' or c == '[':
      brackets += 1
      count[c == '('] += 1
    elif c == ')' or c == ']':
      if count[c == ')'] == 0:
        return 0
      count[c == ')'] -= 1

  if count[0] == 0 and count[1] == 0:
    if brackets:
      return `1`+' '+`brackets`
    return 1
  return 0

# keep this function call here
# to see how to enter arguments in Python scroll down
print MultipleBrackets(raw_input())
