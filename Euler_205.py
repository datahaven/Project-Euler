# Project Euler 205
# Adrian Dale
# 08/06/2011
#
# Not expecting the Monte Carlo simulation
# to be able to converge quickly enough to our
# required level of accuracy, but it should at least
# give me an idea of what the answer should be so that
# I can validate my proper analytical solution
# Looks like it is about 0.5732 - 0.5733
#
# Actual answer is 0.5731441
# You are the 4956th person to have solved this problem.
#
# Nice one, took well under an hour to write and produced
# the correct result first time. Runs in about a second
import sys
import random

# Returns 1 if Peter wins, 0 otherwise
def simulateRoll():
   psum = 0
   for p in range(0,9):
      psum += random.randint(1,4)

   csum = 0
   for c in range(0,6):
      csum += random.randint(1,6)

   if psum > csum:
      return 1
   return 0

def simulate():
   simcount = 0
   pwins = 0
   while True:
      simcount += 1
      pwins += simulateRoll()
      prob = float(pwins) / float(simcount)
      if simcount % 10000 == 0:
         print( simcount, prob)

# simulate()

sums = []

# Recursively works out all possible sums
# for our dice and stores Peter's in sums[0]
# and Colin's in sums[1]
def dicesum(diceleft, sumsofar, dicesize):
   if diceleft == 0:
      # Store a count in a dictionary
      if dicesize == 4:
         dpos = 0
      else:
         dpos = 1
      if sumsofar in sums[dpos]:
         sums[dpos][sumsofar] += 1
      else:
         sums[dpos][sumsofar] = 1
      return
   for d in range(1,dicesize+1):
      dicesum(diceleft-1, sumsofar+d, dicesize)

def calculateProbability():
   pall = sum( sums[0].values() )
   call = sum( sums[1].values() )
   prob = 0
   for ptot,pval in sums[0].items():

      # Prob that p rolls that total
      pprob = float(pval)/float(pall)

      # Multiply by sum of probs that
      # c gets a worse score
      cprob = 0
      for ctot,cval in sums[1].items():
         if ctot >= ptot:
            break
         cprob += float(cval)/float(call)
   
      pprob *= cprob

      prob += pprob

   return prob
      
sums.append({})
sums.append({})
dicesum(9,0,4)
dicesum(6,0,6)

print (calculateProbability())
