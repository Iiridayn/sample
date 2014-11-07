from euler import divisors
from math import ceil

# see https://en.wikipedia.org/wiki/Abundant_number for assertions
# see http://oeis.org/A048242/b048242.txt for pre-generated complete list

bound = 28123 # all int > can be written as sum of two abundant
abundant = set()
for i in range(12, bound):
	if i in abundant: continue

	check = sum(sorted(list(divisors(i)))[:-1])
	if check > i:
		abundant.add(i)
	if not check < i:
		# multiples of perfect and abundant numbers are abundant
		j = i+i
		while j <= bound:
			abundant.add(j)
			j += i
abundant = list(abundant)
abundant.sort()

summed = set()
for i in abundant:
	for j in abundant:
		s = i+j
		if s > bound: break;
		summed.add(s)
print(sum(set(range(1, bound+1)) - summed))
