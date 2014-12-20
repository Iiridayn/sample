# solve a+b+c = x for all integer a,b,c where a^2 + b^2 = c^2
# brute force - check all integers a,b for which c is integer

limit = 1000+1
squares = [0]
for i in range(1, limit):
    squares.append(i*i)

# go from squares - ~ 1000 bytes storage, better O time
solutions = [0]*limit
for a in range(1, limit-1):
    for b in range(1, a+1):
        csq = squares[a]+squares[b]
        try:
            c = squares.index(csq)
        except ValueError:
            continue
        if a+b+c <= limit:
            solutions[a+b+c] += 1

print(solutions.index(max(solutions)))
