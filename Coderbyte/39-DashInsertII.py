# Challenge: As previous, but also '*' between evens. 10 points, 7 mins
# You can see that I fixed my mistake from the prior challenge before moving on to this one.

def DashInsertII(num):
  out = ''
  state = 0 # 1 = prev number was odd, 2 = even
  # str(num) and `num` are slightly different
  for i in str(num):
    if i == '0':
      state = 0
    elif int(i) % 2:
      if state == 1:
        out += '-'
      state = 1
    else:
      if state == 2:
        out += '*'
      state = 2
    out += i

  return out

# keep this function call here
# to see how to enter arguments in Python scroll down
print DashInsertII(raw_input())
