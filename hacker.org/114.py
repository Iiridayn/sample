import sys
sys.path.append("../projecteuler")
from euler import fib

f = str(fib(int(sys.argv[1])))
ans = ""
for i in range(0, len(f), 20000):
	ans += f[i]
print(ans)
