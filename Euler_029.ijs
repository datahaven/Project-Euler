NB. l=: 'C:\Users\Adrian\Documents\Projects\Puzzles\ProjectEuler\project_029.ijs'
NB. load l

NB. Ridiculously easy! Got it in couple of minutes.
 answer =: #~.,^/~2+i.99
NB. Answer is 9183
   
NB. #~.,^/~>:>:i.99x
NB. +/~:, (>:>:i.99)^(99 99 $ >:>:i.99)

NB. Should perhaps have had x on the end of mine to specify 
NB. extended precision integers? Worked anyway ...

NB. So, the thing I've learned is to do nested for loops you can
NB. often use u/~ instead to build a table. Good pattern to spot.