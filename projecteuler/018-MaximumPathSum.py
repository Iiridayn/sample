import fileinput,heapq
# also solves 67

# heappop returns smallest, so I'm negating the values since I want largest
triangle = []
for line in fileinput.input():
    triangle.append([[-int(x),0] for x in line.rstrip().split(" ")])

# dijkstra variation, normal heap - triangle is a DAG
# (fib heap constant probably too high to be worth programming time)
heap = [(triangle[0][0][0], 0, 0)] # (sum, line, column)
triangle[0][0][1] = triangle[0][0][0]

smallest = 0
while len(heap):
    node = heapq.heappop(heap)

    # we pushed same node onto queue with lower value - faster than removal
    # we might have some repeated work, but the lower sum should catch up
    if triangle[node[1]][node[2]][1] > node[0]:
        continue

    if len(triangle) <= node[1]+1:
        if node[0] < smallest:
            smallest = node[0]
        continue

    dist = triangle[node[1]+1][node[2]][0] + node[0]
    if dist < triangle[node[1]+1][node[2]][1]:
        heapq.heappush(heap, (dist, node[1]+1, node[2]))
        triangle[node[1]+1][node[2]][1] = dist

    dist = triangle[node[1]+1][node[2]+1][0] + node[0]
    if dist < triangle[node[1]+1][node[2]+1][1]:
        heapq.heappush(heap, (dist, node[1]+1, node[2]+1))
        triangle[node[1]+1][node[2]+1][1] = dist

print(-smallest)
