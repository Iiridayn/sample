# Challenge: Caesar Cipher on n. 10 points, 7 mins

def CaesarCipher(str,num):
  a = ord('a')
  A = ord('A')

  cipher = ''
  for char in str:
    c = ord(char)
    if c in range(a, a+26): cipher += chr((c-a+num)%26+a)
    elif c in range(A, A+26): cipher += chr((c-A+num)%26+A)
    else: cipher += char

  return cipher

# keep this function call here
# to see how to enter arguments in Python scroll down
print CaesarCipher(raw_input())
