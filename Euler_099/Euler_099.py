# Project Euler 99
# Adrian Dale
# 02/06/2011
#
# You are the 8844th person to have solved this problem.
#
# Answer is 709
# I cheated a bit as I just used the log formula to calculate
# the number of digits in each number, then we had two the
# same and as luck would have it the first one worked.
#
# Haven't bothered to investigate mathematically
# From forum:
#a^x > b^y (with a,b,x,y>0)
#<=>
#log(a^x) > log(b^y)
#<=>
#x log(a) > y log(b) 

import sys
import math


def f():
   linecount = 0

   maxlog = 0.0
   maxline = 0
   file = open('base_exp.txt','r')
   for line in file:
      linecount += 1
      a,b = line.split(',')
      a = int(a)
      b = int(b)

      log = b * math.log10(a) + 1.0
      if log >= math.floor(maxlog):
          maxlog = log
          maxline = linecount
          print(linecount, "a=",a,"b=",b,"ml=",maxlog)

   print(maxline)

f()

# olegyk's J version:
#   1+ I.*./ ({:@[ <: {:@] * ^.&{.) "1/~ T

# Nice short Python version - not mine!
#import math
#mv, ml = 0, 0
#for ln, li in enumerate(file('text-99')):
#b, e = map(int, li.split(','))
#v = e * math.log(b)
#if v > mv:
#mv, ml = v, ln + 1
#print ml
