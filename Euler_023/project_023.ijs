NB. l=: 'C:\Users\Adrian\Documents\Projects\Puzzles\ProjectEuler\project_023.ijs'
NB. load l

NB. Multiples the given combination of prime factors
NB. to produce a divisor
NB. eq 0 1 0 1 # 1 2 2 7 gives 14
m1 =: 4 : '*/ x #"1 y'

NB. Produces proper divisors for y
m2 =: 3 : 0
a =. 1,q:y
b =. #a
c =. (2^b)-1
d =. a"0 i.c
<}:~./:~(#:1+i.c) m1"1 d
)

NB. isAbundant y
isA =: 3 : 'y< > +/ &.> m2 ("0) y'

NB. I used this in allAbSums but was able to put it in
NB. directly
NB. allAb =: (#~ isA) 1+i.28123

allAbSums =: (#~(<&28124)) ~. , +/~ (#~ isA) 1+i.28123

m3 =: 3 : '(-.(y e.allAbSums)) # y'

result =: +/ m3 1+i.28123
