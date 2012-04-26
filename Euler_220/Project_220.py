# Project Euler 220
# Heighway Dragon
#
# Adrian Dale
# 16/06/2011

import sys

# Apply the dragon substitution rules to make
# D(n+1) from D(n), which is string s
def makeNextString(s):
   ns = ""
   for c in s:
      if c == "a":
         ns += "aRbFR"
      elif c == "b":
         ns += "LFaLb"
      else:
         ns += c
   return ns

# Repeatedly apply the dragon rule N times
# to make string Dn
def makeString(N):
   s = "Fa"
   i = 0
   while i < N:
      s = makeNextString(s)
      i += 1
   return s

# Returns the position (as a tuple x,y,facing)
# we'd be at if we did command c, from position pos
# returns pos when c is "a" or "b"
def newPos(c, pos):
   nx = pos[0]
   ny = pos[1]
   nf = pos[2]

   if c == "F":
      if nf == 0:
         ny = ny+1
      elif nf == 1:
         nx = nx+1
      elif nf == 2:
         ny = ny-1
      elif nf == 3:
         nx = nx-1
   elif c == "L":
      nf = (nf+3)%4
   elif c == "R":
      nf = (nf+1)%4
   return (nx,ny,nf)

# returns the position (x,y,facing),
# starting from pos in string s after
# tgt steps
def walkString(pos,s,tgt):
   
   i = 0
   stepcount = 0
   while i < len(s) and stepcount < tgt:
      c = s[i]
      pos = newPos(c, pos)
      if c == "F":
         stepcount += 1
      i += 1

   return pos

# This produces the correct answer for the 
# sample test case, but of course can't handle
# the proper size input
pos = (0,0,0)
s = makeString(10)
print( walkString(pos,s,500))
      