# Challenge: GCD. 10 points, 3 mins

# just going to implement my lisp in Python:
# (defun gcd1 (a b) (if (= b 0) a (gcd1 b (mod a b))))
def Division(num1,num2):
  if not num2: return num1
  return Division(num2, num1%num2)

# keep this function call here
# to see how to enter arguments in Python scroll down
print Division(raw_input())
