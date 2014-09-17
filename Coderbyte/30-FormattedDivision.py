# Challenge: Divide two numbers, output w/commas and 4 decimals. 10 points, 6 mins

def FormattedDivision(num1,num2):
  return "{:,.4f}".format(float(num1)/num2)

# keep this function call here
# to see how to enter arguments in Python scroll down
print FormattedDivision(raw_input())
