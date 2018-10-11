import math

def isPerfectSquare(n):
    sq = long(math.sqrt(n))
    return (sq ** 2) == n

def is_square(apositiveint):
  x = apositiveint // 2
  seen = set([x])
  while x * x != apositiveint:
    x = (x + (apositiveint // x)) // 2
    if x in seen: return False
    seen.add(x)
  return True
  
def isTriangle(n):
    part = 1 + (8*n)
    if part % 2 == 0:
        return false
    return is_square(part)

# This works, but is way too slow to get to 40 terms
numTerms = 0
sum = 0
for n in range(1, 100000000):
    swapCount = ((n+1)*(n+1)) - 1
    if isTriangle(swapCount):
        numTerms = numTerms + 1
        sum = sum + n
        print n, swapCount
    if numTerms >= 40:
        break;
print "Sum=",sum