import sys
from euler import divisors

amicable = set()
for i in range(4, int(sys.argv[1])+1):
	if i in amicable: continue
	check = sum(sorted(list(divisors(i)))[:-1])
	if check == i: continue
	if i == sum(sorted(list(divisors(check)))[:-1]):
		amicable.add(i)
		amicable.add(check)
		print("Found "+str(i)+" and "+str(check));

print(sum(amicable))
