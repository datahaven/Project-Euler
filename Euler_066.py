# Project Euler 66
# Adrian Dale
# 08/06/2011
#
# You are the 4768th person to have solved this problem.
# Answer: 661
import sys
import math

# Mostly derived from functions on wikipedia's page on
# Methods of computing square roots - continued fraction expansion
# Code keeps expanding the continued fraction for sqrt(D) until
# it finds an x/y that satisfies the equation
def PellMinimum(D):
   mc = 0
   dc = 1
   ac = int(math.floor(math.sqrt(D)))
   a0 = ac

   hn1 = 1
   hn2 = 0
   kn1 = 0
   kn2 = 1
   
   while True:
      mn = dc*ac-mc
      dn = (D-(mn*mn))/dc
      an = int(math.floor((a0+mn)/float(dn)))
           
      hn = ac*hn1 + hn2
      kn = ac*kn1 + kn2

      if (hn*hn)-(D*kn*kn) == 1:
         return hn

      mc = mn
      dc = dn
      ac = an

      hn2 = hn1
      hn1 = hn
      kn2 = kn1
      kn1 = kn

def solve():
   maxD = 0
   maxDval = 0
   for D in range(2,1001):
      
      # Ignore exception caused by perfect squares
      # The lazy way to solve the problem
      try:
         x = PellMinimum(D)
      except ZeroDivisionError:
         x = 0
      
      if x > maxDval:
         maxDval = x
         maxD = D
         
   return maxD

print( solve() )