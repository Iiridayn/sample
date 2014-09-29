#!/usr/bin/python
import urllib.request
import re

sess = '816c2591088f3eb8ea48cb6935def90d'

foo = urllib.request.urlopen('http://bright-shadows.net/challenges/programming/xor/tryout.php?PHPSESSID='+sess)
bar = foo.read()
print(bar)
baz = re.search('b\'The "text" is: \\\\\'([^\']+)\\\\\'\'$', str(bar))
text = baz.group(1)
print(bytes(text, 'utf-8'))

xor = list(b'C12W4BERT954')
max = len(xor)
idx = 0
text = list(bytes(text, 'utf-8'))
textxor = []
for a in text:
    if (idx >= max):
        idx = 0
    b = xor[idx]
    idx+=1
    textxor.append(a^b)

text = bytes(textxor)
print(text)
solution = urllib.parse.urlencode({'solution': text})
quux = urllib.request.urlopen('http://bright-shadows.net/challenges/programming/xor/solution.php?'+solution+'&PHPSESSID='+sess)
print(quux.read())
