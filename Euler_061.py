# Project Euler 61
# Adrian Dale
#
# You are the 6800th person to have solved this problem.
# Sum = 28684

# Previous experimentation told me which ranges yield
# numbers between 1000 and 9999, although it's not too much
# trouble to do this calculation in the code, either
# Four digit numbers are split into pairs:
# (first two, last two digits)
TriNos = [ divmod(n*(n-1)/2, 100)     for n in range(46,141+1)]
SqrNos = [ divmod(n*n, 100)           for n in range(32,99+1) ]
PntNos = [ divmod(n*((3*n)-1)/2, 100) for n in range(26,81+1) ]
HepNos = [ divmod(n*((2*n)-1), 100)   for n in range(23,70+1) ]   
SepNos = [ divmod(n*((5*n)-3)/2, 100) for n in range(21,63+1) ]  
OctNos = [ divmod(n*((3*n)-2), 100)   for n in range(19,58+1) ]

AllLists = [TriNos, SqrNos, PntNos, HepNos, SepNos, OctNos]

# Remove any tuples where one or the other values is below ten,
# as these can't form part of a four digit no.
def ClearSingles():
   for list in AllLists:
      for (a,b) in list[:]:
         if a<10 or b<10:
            list.remove( (a,b) )

# All possible pairs that could follow on the end of our solution
def allPossibleNextMoves(sol):
  result = []
  if len(sol)==0:
    # Starting situation - could start with
    # any of the pairs in the first list
    for i in range(0, len( AllLists[0] )):
      result.append( (0,i) )
    return result

  # Get last pair of sol entry
  (la,lb) = sol[-1]
  (sola,solb) = AllLists[la][lb]

  # Now need to find lb as the first two digits
  # of entries from each list that's not yet been used
  for i in range(0,6):
    # Find the next list of pairs that doesn't
    # already feature in the solution
    listAlreadyUsed = False
    for solEntry in sol:
       (sa,sb) = solEntry
       if sa == i:
         listAlreadyUsed = True

    if listAlreadyUsed == False:
      # Find all the pairs that start with the last
      # two digits of the solution
      for k in range(0,len(AllLists[i])):
         (ka,kb) = AllLists[i][k]
         if ka == solb:
           result.append( (i,k) )

  return result

def printSolution(sol):
  solSum = 0
  print("Solution pairs:")
  for (x,y) in sol:
    (sa,sb) = AllLists[x][y]
    solSum = solSum + 100*sa + sb
    print(sa,sb,"/",)
  print()
  print("Sum =", solSum)


# Recursive depth first search for the answer
def Solve(sol):
  if len(sol) == 6:
    # Need to check last two digits wrap
    # around to match first two
    (fa,fb) = sol[0]
    (la,lb) = sol[5]
    (fsa,fsb) = AllLists[fa][fb]
    (lsa,lsb) = AllLists[la][lb]
    if lsb == fsa:
      printSolution(sol)
    return

  # get all possible next sols
  allPossible = allPossibleNextMoves(sol)

  # Try solving for each of them
  for poss in allPossible:
    newsol = sol[:]
    newsol[len(sol):] =[poss]
    Solve(newsol)
    
ClearSingles()

Solve([])
