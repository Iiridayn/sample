#!/usr/bin/python
import sys

def t2a (t):
    a = ""
    for i in range(0, len(t), 6):
        p = 0
        for c in t[i:i+6]:
            p = p*3 + int(c)
        a += chr(p)
    return a

print(t2a(sys.argv[1]))
