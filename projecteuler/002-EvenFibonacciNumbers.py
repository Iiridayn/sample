fib = [0,1]
while True:
    next = fib[-2]+fib[-1]
    if next > 4000000: break
    fib.append(next)

print(sum(item for item in fib if not item % 2))
