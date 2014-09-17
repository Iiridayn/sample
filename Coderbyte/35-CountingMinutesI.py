# Challenge: same as 33. 10 points, < 1 min

def CountingMinutesI(str):
  # convert to sane format
  times = str.split('-')
  minutes = []
  for t in times:
    hhmm = t[:-2].split(':')
    minutes.append(int(hhmm[0])*60+int(hhmm[1])+(720 if t[-2:] == "pm" else 0))
  diff = minutes[1] - minutes[0]
  return diff if diff >= 0 else diff+1440 # add a day

# keep this function call here
# to see how to enter arguments in Python scroll down
print CountingMinutesI(raw_input())
