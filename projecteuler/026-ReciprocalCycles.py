import sys
# only need a single repetition; the math will cycle from there.
# However - the repetition is not the digit, but the remainder.
# printing the digits will be off if the final remainder is 10
# but the count is correct
longest = 0
value = None
for i in range(2, int(sys.argv[1])):
	candidate = []
	r = 1
	while True:
		d,r = divmod(r*10, i)
		if not r: break # recurring 0's

		if r in candidate:
			candidate = candidate[candidate.index(r):]
			break
		candidate.append(r)
	if not r: continue

	length = len(candidate)
	if length > longest:
		longest = length
		value = i

print(value)
