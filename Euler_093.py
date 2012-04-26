# Project Euler 93
# Adrian Dale
# 07/06/2011
#
# You are the 2705th person to have solved this problem.
# Answer: 1258
# Took about 2.5hrs to write the code
import itertools
import sys
import math

operators = ['+','-','*','/']

def op(d1, o1, d2):
   result = 0
   if o1 == '+':
      result = float(d1)+float(d2)
   elif o1 == '-':
      result = float(d1)-float(d2)
   elif o1 == '*':
      result = float(d1)*float(d2)
   else:
      result = float(d1)/float(d2)
   return result

def check(d1,d2,d3,d4,o1,o2,o3):
   results = []

   try:
      r1 = op(op(op(d4,o1,d3),o2,d2),o3,d1)
   except ZeroDivisionError:
      r1 = 0

   try:
      r2 = op(op(d4,o1,d3),o2,op(d2,o3,d1))
   except ZeroDivisionError:
      r2 = 0

   try:
      r3 = op(d4,o1,op(d3,o2,op(d2,o3,d1)))
   except ZeroDivisionError:
      r3 = 0

   try:
      r5 = op(op(d4,o1,op(d3,o2,d2)),o3,d1)
   except ZeroDivisionError:
      r5 = 0

   try:
      r6 = op(d4,o1,op(op(d3,o2,d2),o3,d1))
   except ZeroDivisionError:
      r6 = 0

   results.append(r1)
   results.append(r2)  
   results.append(r3)
   results.append(r5)
   results.append(r6)

   return results

def allOperators(a,b,c,d):
   totals = {}
   for o1 in operators:
      for o2 in operators:
         for o3 in operators:
            digits = [a,b,c,d]
            for (pa,pb,pc,pd) in itertools.permutations(digits,4):
                results = check(pa,pb,pc,pd,o1,o2,o3)
                for r in results:
                   if r > 0 and math.fabs(r-math.floor(r)) < 0.01:
                      totals[int(r)] = 1
   maxconsec = 0
   for i in range(1,3026):
      if i in totals:
         maxconsec += 1
      else:
         break
  
   return maxconsec
               
def allOperations():
   bestconsec = 0
   for a in range(0,10):
      for b in range(a+1,10):
         for c in range(b+1,10):
            for d in range(c+1,10):
               consec = allOperators(a,b,c,d)
               if consec > bestconsec:
                  bestconsec = consec
                  print( a,b,c,d, bestconsec)
                              
allOperations()
