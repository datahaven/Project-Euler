# Project Euler 57
# Adrian Dale
# 02/06/2011
#
# You are the 10458th person to have solved this problem.
# Answer: 153
#
# Got the neat digit counting code from
# http://stackoverflow.com/questions/2189800/length-of-an-integer-in-python
# NB I simplified it for the comparison, so be careful if re-using
#
# Got formula for next convergent of continued fraction representation
# of root 2 from wikipedia:
# http://en.wikipedia.org/wiki/Square_root_of_2
import sys
import math
import profile

def f():
   count = 0
   p = 1
   q = 1
   for n in range(0,1000):

      if int(math.log10(p)) > int(math.log10(q)):
         count += 1

      #newp = p + 2*q
      #newq = p + q
      #p = newp
      #q = newq

      # Looks nicer (and more pythonic)
      # written like this
      p,q = p+2*q, p+q

   return count

# profile.run('f()')
print(f())

###
# olegyk's J version:
#    f=: >:@%@>:
#    +/ >&(#@":)/@(2&x:)"0 f^:(<1000) 1x 
