import fileinput
# Plan - build triangle numbers generator, check all until next too large
# (could search w/leaping, but set of numbers is tiny)

def wordValue(word):
    '''Words are all normalized upper case'''
    base = ord('A') - 1
    value = 0
    for c in word:
        value += ord(c) - base
    return value

triangle = [1]
i = 2
top = 1

words = []
for line in fileinput.input():
    words.extend(line.strip('"').split('","'))

count = 0
for word in words:
    value = wordValue(word)
    while value > top:
        top += i
        triangle.append(top)
        i += 1
    if value in triangle:
        count += 1

print(count)
