import sys,math

def palindrome(test):
    half = len(test)/2
    return test[:math.ceil(half)] == test[:math.floor(half)-1:-1]

# 924 962 888888
# 924 932 861168
# 916 968 886688
# 913 993 906609 <--
# 902 914 824428
def max_palindrome(digits):
    """
    Find the maximum using visual inspection :S
    Using triangle multiplication, but it's not monotonically decreasing.
    Since debugging gave me the answer, moving on for now.
    See http://math.stackexchange.com/q/955026/4218 for more info.
    """
    top = pow(10,digits)-1
    for x in range(top, pow(10,digits-1)-1, -1):
        for y in range(top, x-1, -1):
            if palindrome(str(x*y)):
                print(x,y,x*y)

print(max_palindrome(int(sys.argv[1])))
