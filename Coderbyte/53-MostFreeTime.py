# Challenge: compute the longest free period in a schedule. 6 points, 25 mins.
# Lost 4 points on time (almost 5). I found the input very unpleasant.
from operator import itemgetter

def MostFreeTime(strArr):
  events = []

  # I strongly dislike the 12 hour format and try to avoid it in all parts of my life
  for event in strArr:
    times = event.split('-')
    time = times[0][:-2].split(':')
    start = 60*int(time[0])+int(time[1])+(60*12 if times[0][-2:] == 'PM'
 and int(time[0]) != 12 else 0)
    time = times[1][:-2].split(':')
    end = 60*int(time[0])+int(time[1])+(60*12 if times[1][-2:] == 'PM'
 and int(time[0]) != 12 else 0)
    events.append((start,end))

  events.sort(key = itemgetter(1))

  max = 0
  for i in range(1,len(events)):
    free = events[i][0] - events[i-1][1]
    if free > max:
      max = free

  return "{:02d}:{:02d}".format(max//60, max%60)

# keep this function call here
# to see how to enter arguments in Python scroll down
print MostFreeTime(raw_input())
