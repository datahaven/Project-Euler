NB. l=: 'C:\Users\Adrian\Documents\Projects\Puzzles\ProjectEuler\project_069.ijs'
NB. load l


phi =: 3 : 0
t1 =. 5 p: y
t2 =. y % t1
)

philist =: phi i.1000001

eqn =: 3 : 0
y > 5.53930
)

answer =: (eqn philist) # i.1000001

NB. J makes simple things like this very difficult.
NB. Took ages to work out how to do that last line

NB. Used >./ philist to get max value

NB. You are the 9371st person to have solved this problem.
NB. 510510

NB. Official answer:
totient=: * -.@%@~.&.q:
>: (i.>./) (% totient"0) >: i. 1e6

NB. or this one
(i.>./)(% 5&p:)i.1e6