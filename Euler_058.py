# Project Euler 58
# Adrian Dale
# 02/06/2011
#
# You are the 10050th person to have solved this problem.
#
# Answer 26241
#
import sys

# I borrowed the prime code - doesn't look horribly efficient,
# but I'm sure it'll do. I halved the run time from 14s to 7s
# by altering it to step through sieve in 2's

# prime numbers are only divisible by unity and themselves
# (1 is not considered a prime number)
 
def isprime(n):
   """check if integer n is a prime"""
   # range starts with 2 and only needs to go up the squareroot of n
   if n % 2 == 0:
      return False
   for x in range(3, int(n**0.5)+1,2):
      if n % x == 0:
         return False
   return True

def f():
   prime_diag_count = 0

   # Need to allow for the one in the centre
   diag_count = 1

   side = 3
   while True:
      diag_count += 4
      diag = side*side
      # No need to check first corner is prime as we
      # know it is a square. Shaved runtime from 20 seconds
      # to 14 seconds with this

      for x in range(0,3):
         diag = diag - side + 1
         if isprime(diag):
            prime_diag_count += 1
      
      ratio = float(prime_diag_count) / float(diag_count)
     
      if ratio < 0.1:
         return side;
   
      side += 2

print( f() )
