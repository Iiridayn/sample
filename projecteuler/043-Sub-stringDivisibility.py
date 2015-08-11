import itertools

numbers = []
for p in itertools.permutations(range(0, 10), 10):
    number = ''.join(map(str, p))
    if (int(number[1:4]) % 2
            or int(number[2:5]) % 3
            or int(number[3:6]) % 5
            or int(number[4:7]) % 7
            or int(number[5:8]) % 11
            or int(number[6:9]) % 13
            or int(number[7:10]) % 17):
        continue
    numbers.append(int(number))

print(sum(numbers))
