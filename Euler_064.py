# Project Euler 64
# Adrian Dale
# 08/06/2011
#
# You are the 5248th person to have solved this problem.
#
# Excellent work, datahaven! By solving your 100th problem
# you have earned a place among the top 2.35% of members
# and have advanced to level 3.
#
# Answer: 1322
#
# Took under 10 minutes to write code, using code from
# Project_066.py
# Runs in under a second
import sys
import math

def ContinuedFractionPeriod(D):
   mc = 0
   dc = 1
   ac = int(math.floor(math.sqrt(D)))
   a0 = ac

   hn1 = 1
   hn2 = 0
   kn1 = 0
   kn2 = 1
   
   cfr = []
   while True:
      cfr.append( (ac,dc,mc) )
      mn = dc*ac-mc
      dn = (D-(mn*mn))/dc
      an = int(math.floor((a0+mn)/float(dn)))
           
      mc = mn
      dc = dn
      ac = an

      if (ac,dc,mc) in cfr:
         return len(cfr)-1

def solve():
   oddperiodcount = 0
   for D in range(2,10001):
      
      # Ignore exception caused by perfect squares
      # The lazy way to solve the problem
      try:
         period = ContinuedFractionPeriod(D)
      except ZeroDivisionError:
         continue
      
      if period % 2 == 1:
         oddperiodcount += 1
         
   return oddperiodcount

print( solve() )