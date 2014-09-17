# Challenge: convert minutes to %h:%m (yeah, single digit). 10 points, 4 mins

def TimeConvert(num): 
  return `num//60`+':'+`num%60`

# keep this function call here
# to see how to enter arguments in Python scroll down
print TimeConvert(raw_input())
