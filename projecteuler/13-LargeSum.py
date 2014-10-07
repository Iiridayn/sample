# Compute the sum in Python from stdin
import sys

total = 0
for line in  sys.stdin:
    total += int(line)

print(str(total)[0:10])
