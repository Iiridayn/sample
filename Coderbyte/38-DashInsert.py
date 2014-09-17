# Challenge: Insert '-' between every odd number. 10 points, 3 mins, 1 failed test case
# Failed test case was because `num` put an 'L' on the end of the string. str(num) worked fine, but I didn't want to resubmit and lose the original time.

def DashInsert(num):
  out = ''
  state = 0 # 1 = prev number was odd
  for i in `num`:
    if int(i) % 2:
      if state == 1:
        out += '-'
      state = 1
    else:
      state = 0
    out += i

  return out

# keep this function call here
# to see how to enter arguments in Python scroll down
print DashInsert(raw_input())
