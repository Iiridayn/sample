# Challenge: Capitalize first letter of each word. 10 points, 5 mins
# I could have used str.capwords or at least word.captialize instead.

def LetterCapitalize(str):
  words = []

  for word in str.split():
    words.append(word[0].upper()+word[1:])

  return ' '.join(words)


# keep this function call here  
# to see how to enter arguments in Python scroll down
print LetterCapitalize(raw_input())
