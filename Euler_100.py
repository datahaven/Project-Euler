# Project Euler 100
# Adrian Dale
# 08/06/2011
#
# You are the 3693rd person to have solved this problem.
# Answer: 756872327473
#
# I needed the hint to "look for a pattern" - typed values I'd
# found for r into sloane and got the formula for triangle numbers
import sys
import math

target = 10**12

def isperfect(n):
   if (n & 7 == 1) or (n & 31 == 4) or (n & 127 == 16) or (n & 191 == 0):
      return True
   return False

def getb(r):
   sqr = (8*r*r)+1
   root = math.sqrt(sqr)
   
   introot = int(root)

   if (introot*introot) != sqr:
      return 0
   sol1 = (1 + 2*r + introot)/2
   return sol1

def getbII(r):
   sqr = (8*r*r)+1
   root = math.sqrt(sqr)
   sol = (1+2*r+root)/2.0
   return sol

def solve():
   r = 293116000000
   #r = 292893000000
   r = 1
   while r < target:
      r += 1
      
      b = getb(r)

      if r%1000000 == 0:
         roughb = getbII(r)
         #print "Trying r=",r,roughb,roughb+r
         if roughb+r < target:
            r += 100000

      if b == 0:
         continue
      
      print( "b=",b,"r=",r,"b+r=",b+r)
      if b+r < target:
         continue
      print ("RESULT: b=",b,"r=",r,"b+r=",b+r)
      return

def test01():
   x = 1
   while x < 10000000000:
      xsqr = x*x
      if xsqr%8 == 1:
         rsqr = (xsqr-1)/8
         if isperfect(rsqr) == True:
            root = math.sqrt(rsqr)
            introot = int(root)
            if (introot*introot) == rsqr:
               print(x,root)
      x += 1

def test02():
   x=1
   while x < 10:
      print (x,8*x*x+1,(8*x*x)+(16*x)+9)
      x+=1

def triangle(n):
   if n == 0:
      return 0
   if n == 1:
      return 1
   return 6*triangle(n-1) - triangle(n-2)

def test03():
   for a in range(0,100):
      r = triangle(a)
      b = getb(r)
      if b+r > 10**12:
         print ("RESULT: b=",b,"r=",r,"b+r=",b+r)
         return 

test03()

#solve()

