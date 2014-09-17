# Challenge: calculate how many times an overlapping region fits into the first rectangle. 0 points, time value not recorded :/.
# I didn't quite understand that I would be getting a string instead of an array of tuples, so I failed every test. Resubmitted with some string processing and it still doesn't accept any automated input, but it does accept manual test input fine.
from operator import itemgetter

# resubmission due to misunderstanding the input requirements :S
# actually, I have litterally no idea why this one isn't working >:/
def OverlappingRectangles(strArr):
  tuples = []
  for t in strArr[1:-1].split('),('):
    t = t.split(',')
    tuples.append((int(t[0]), int(t[1])))

  first = tuples[:4]
  second = tuples[4:]

  first.sort(key=itemgetter(0,1))
  second.sort(key=itemgetter(0,1))

  first = [first[0],first[-1]]
  second = [second[0],second[-1]]

  def area(rect):
    return (rect[1][0]-rect[0][0])*(rect[1][1]-rect[0][1])

  overlap = [(max(first[0][0], second[0][0]),max(first[0][1], second[0][1])),
             (min(first[1][0], second[1][0]),min(first[1][1], second[1][1]))]

  return area(first)/area(overlap)

# keep this function call here
# to see how to enter arguments in Python scroll down
print OverlappingRectangles(raw_input())
