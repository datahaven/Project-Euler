# Project Euler 340
# Adrian Dale
# 01/06/2011

import sys

a = 21**7
b = 7**21
c = 12**7

# Function f(n) is a non-recursive version of F() given
# in the problem definition.
# It is not used in the final calculation.
def f(n):
   return ((((((b-n)/a)+1)*3)+1)*(a-c))+b-((b-n)%a)

# Returns the summation of integers from 0 to n
def sumn(n):
   return (n*(n+1)) / 2

# Code was written by writing f(n) inside a while
# loop from 0 to b and progressively factoring out the
# inside of the loop until it disappeared
rpt = b/a  
rem = b-(rpt*a)
sum = sumn(rpt-1)*a
sum += rpt*(rem+1)
sum += b+1
sum *= 3
sum += b+1
sum *= a-c
sum += b*(b+1)
sum -= sumn(a-1)*rpt
sum -= sumn(rem)
   
print( sum%10**9 )

