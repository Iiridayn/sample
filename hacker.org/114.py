a,b = 1,1
for i in range(3,1500001):
	a,b = b,a+b

ans = ""
s = str(b)
for i in range(0, len(s), 20000):
	ans += s[i]
print(ans)
