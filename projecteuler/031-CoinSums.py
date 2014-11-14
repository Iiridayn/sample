import sys

coins = [200, 100, 50, 20, 10, 5, 2, 1]
goal = int(sys.argv[1])

count = 0
i = 0
done = 0
v = []
while done < len(coins):
    while sum(v) < goal:
        v.append(coins[i])
    if sum(v) == goal:
        count += 1
        #print(v)
    if v.pop() == coins[i]:
        i += 1
    if i >= len(coins):
        i -= 1
        while v:
            j = coins.index(v.pop())
            if j < i:
                i = j+1
                break
    if not v:
        done += 1
        i = done

print(count)
