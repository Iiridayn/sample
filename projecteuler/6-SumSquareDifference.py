import sys

top = int(sys.argv[1])
diff = pow(top*(top+1)//2,2)
for x in range(top+1):
    diff -= x*x
print(diff)
