import sys

def number2words(num):
    '''Returns a string as numbers would be spoken, domain [0,1,000,000)'''
    words = []

    def hundred2words(num):
        digits = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"]
        tens = ["twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"]

        if num // 100:
            words.append(digits[num//100-1])
            words.append("hundred")

        ten = num%100
        if not ten: return
        if words: words.append("and")
        if ten < 20:
            words.append(digits[ten-1])
        else:
            word = tens[ten//10-2]
            if ten%10: word = word+"-"+digits[ten%10-1]
            words.append(word)

    if num // 1000:
        hundred2words(num//1000)
        words.append("thousand")
    if num % 1000:
        if words and num%1000 > 100: words[-1] = words[-1]+","
        hundred2words(num%1000)

    return "zero" if not words else " ".join(words)

#print(number2words(int(sys.argv[1])))
strings = map(number2words, range(1, int(sys.argv[1])+1))
filtered = ["".join(filter(lambda c: c.isalpha(), s)) for s in strings]
#print(list(zip(filtered, map(len, filtered))))
print(sum(map(len, filtered)))
