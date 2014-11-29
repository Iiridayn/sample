from fractions import gcd

fractions = [] # (numerator, denominator)
for i in range(1,10):
    for j in range(i+1,10):
        frac = i/j
        for k in range(1,10):
            # (i*10+k)/(j*10+k) is not possible - can't simplify
            # (k*10+i)/(k*10+j) isn't either
            if frac == (i*10+k)/(k*10+j):
                fractions.append((i*10+k,k*10+j))
            if frac == (k*10+i)/(j*10+k):
                fractions.append((k*10+i,j*10+k))
print(fractions)
ans = [1,1]
for f in fractions:
    ans[0] *= f[0]
    ans[1] *= f[1]
div = gcd(ans[0],ans[1])
print(ans[0]//div,"/",ans[1]//div)
