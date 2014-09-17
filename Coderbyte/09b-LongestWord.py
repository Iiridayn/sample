# Challenge: Return first longest word in str. 10 points, 12 mins, 1 failed test ("123456789 98765432")

def LongestWord(sen):
  word = ""
  longest = ""
  state = 0 # 1 is in word

  for c in sen+' ': # too lazy to fix it right, just force the else on last word
    if  (ord(c) in range(ord('a'), ord('z')+1)
      or ord(c) in range(ord('A'), ord('Z')+1)):
      state = 1
      word += c
    else: # reached a non-word character
      if state == 1 and len(word) > len(longest):
        longest = word
      word = ""
      state = 0

  return longest

# keep this function call here
# to see how to enter arguments in Python scroll down
print LongestWord(raw_input())

