#!/usr/bin/python
import urllib.request
import re

sess = '816c2591088f3eb8ea48cb6935def90d'

foo = urllib.request.urlopen('http://bright-shadows.net/challenges/programming/get_started/tryout.php?PHPSESSID='+sess)
bar = foo.read()
baz = re.search('b\'The "text" is: \\\\\'([^\']+)\\\\\'\'', str(bar))
baz.group(1)

quux = urllib.request.urlopen('http://bright-shadows.net/challenges/programming/get_started/solution.php?solution='+baz.group(1)+'&PHPSESSID='+sess)
print(quux.read())
