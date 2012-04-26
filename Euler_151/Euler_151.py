# Project Euler 151
# Adrian Dale
# 04/06/2011
#

import sys
import copy

def buildGraph(envelope):
   print ("buildGraph(", envelope, ")")
  
   # For each sheet in the envelope
   envsize = len(envelope)
   for i in range(0,envsize):
      sheet = envelope[i]
      # create a new envelope without that sheet
      
      newenv = copy.deepcopy(envelope)
      del newenv[i]
      #print i,newenv, envelope, len(newenv), sheet

      # cut the selected sheet as per rules
      # and add pieces back to envelope
      if sheet == 2:
         newenv.append( 1 )
      if sheet == 4:
         newenv.append( 1 )
         newenv.append( 2 )
      if sheet == 8:
         newenv.append( 1 )
         newenv.append( 2 )
         newenv.append( 4 )
      
      # Continue to build graph for remaining sheets
      if len(newenv) > 0:
         buildGraph(newenv)
      
#buildGraph( [1] )
buildGraph( [1,2,4] )
