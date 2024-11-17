#!/bin/python3

import sys;

words = [line.rstrip() for line in sys.stdin];
#freq = [[0]*26, [0]*26, [0]*26, [0]*26, [0]*26]
freq = [0] * 26
for word in words:
    for index, letter in enumerate(word):
        #freq[index][ord(letter)-ord('a')] += 1
        freq[ord(letter)-ord('a')] += 1

if 0:
    for index, letters in enumerate(freq):
        print(index, ':')
        counts = [[], [], []]
        for letter, count in enumerate(letters):
            if count <= 2:
                counts[count].append(chr(ord('a') + letter))
        for count, letters_of_count in enumerate(counts):
            print("\t", count, ":", letters_of_count)
else:
    for letter, count in enumerate(freq):
        if count <= 2:
            print(chr(ord('a') + letter), count)
