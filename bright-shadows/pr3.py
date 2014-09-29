#!/usr/bin/python
import urllib.request
import re

sess = '816c2591088f3eb8ea48cb6935def90d'

foo = urllib.request.urlopen('http://bright-shadows.net/challenges/programming/dit_dit_dah/tryout.php?PHPSESSID='+sess)
bar = foo.read()
print(bar)
baz = re.search('b\'The "text" is: \\\\\'([^\']+)\\\\\'\'$', str(bar))
text = baz.group(1)

text = text.replace('2', ' ').replace('1', '.').replace('0', '-')
print(bytes(text, 'utf-8'))
text = text.split(' ')

morse = {
    '.-': b'a',
    '-...': b'b',
    '-.-.': b'c',
    '-..': b'd',
    '.': b'e',
    '..-.': b'f',
    '--.': b'g',
    '....': b'h',
    '..': b'i',
    '.---': b'j',
    '-.-': b'k',
    '.-..': b'l',
    '--': b'm',
    '-.': b'n',
    '---': b'o',
    '.--.': b'p',
    '--.-': b'q',
    '.-.': b'r',
    '...': b's',
    '-': b't',
    '..-': b'u',
    '...-': b'v',
    '.--': b'w',
    '-..-': b'x',
    '-.--': b'y',
    '--..': b'z',
    '.-.-.-': b'.',
    '--..--': b',',
    '..--..': b'?',
    '-..-.': b'/',
    '.--.-.': b'@',
    '.----': b'1',
    '..---': b'2',
    '...--': b'3',
    '....-': b'4',
    '.....': b'5',
    '-....': b'6',
    '--...': b'7',
    '---..': b'8',
    '----.': b'9',
    '-----': b'0'
}

solution = []
for m in text:
    solution.append(morse[m])

solution = b''.join(solution)

print(solution)
solution = urllib.parse.urlencode({'solution': solution})
quux = urllib.request.urlopen('http://bright-shadows.net/challenges/programming/dit_dit_dah/solution.php?'+solution+'&PHPSESSID='+sess)
print(quux.read())
