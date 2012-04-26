# Project Euler 80
# Adrian Dale
# 08/06/2011
#
# You are the 5151st person to have solved this problem.
#
# A bit cheaty because I used PARI/GP to calculate all the 
# roots, then this little program to sum up the digits
# Still took ages to get working due to nitpicking in problem
# definition - eg whether or not to count digits before decimal
# point; whether or not to round; and finally to only include the
# irrational numbers, which matters if you are including digits
# to the left of the decimal point
#
#  \l (turn logging on)
#  \p 105 (set precision bigger than needed)
#  for(n=1,100,print(n^0.5))
import sys

def digitsum(b):
   return sum(map(lambda x: ord(x)-ord('0'),b)) 

# Reads in the input file
def readinput(filename):
   sum = 0
   file = open(filename,'r')
   for line in file:
      (a,b) = line.split('.')
      ds = digitsum(b[0:99]) + int(a)
      print(ds)
      sum += ds
      
   return sum

print(readinput('Project_080.txt'))
