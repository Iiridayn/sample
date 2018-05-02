import math,sys

def fib(i):
	'''Approximate the i'th Fibonacci number'''
	sqrt5 = pow(5, 0.5)
	phi = (1 + sqrt5)/2
	return math.floor(pow(phi, i) / sqrt5 + 0.5)

print(math.log1p(fib(int(sys.argv[1]))))
