# Challenge: Check if str1 contains str2, scambled. 10 points, 8 mins

def StringScramble(str1,str2):
  a = ''.join(sorted(str1))
  b = ''.join(sorted(str2))

  pos = 0
  for c in b:
    pos = a.find(c, pos)+1
    if not pos: return "false"

  return "true"

# keep this function call here
# to see how to enter arguments in Python scroll down
print StringScramble(raw_input())
