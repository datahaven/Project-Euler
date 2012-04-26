# Project Euler 63
# Adrian Dale
# 02/06/2011
#
# You are the 11786th person to have solved this problem
# Answer: 49
# Worked first time, too!
import sys

def intcount():
   count = 0
   n=1
   keepgoing = True
   while keepgoing:
      keepgoing = False
      for a in range(1,10):
         x = a**n
         if x >= 10**(n-1) and x < 10**n:
            count += 1
            keepgoing = True
      n += 1

   return count

print(intcount())
