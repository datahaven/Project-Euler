NB. l=: 'C:\Users\Adrian\Documents\Projects\Puzzles\ProjectEuler\project_046.ijs'
NB. load l

NB. First n primes
np =: 3 : 'p: i.y'

NB. First n doubled squares
ndsq =: 3 : '+:(^&2) i.y'

NB. Table of all of them summed up
ps =: (np +/ ndsq)

NB. List of nos in the table
pl =: 3 : '~.,ps y'

NB. Above list but with even numbers and primes removed
plf =: 3 : '(#~(0&p:))(#~2&|) pl y'

NB. Above list sorted.
NB. List is now all odd non-prime numbers that can be made of
NB. a prime and a doubled square for first y primes and squares 
splf =: 3 : '/:~ plf y'

NB. All odd composite numbers less than u
oc =: 3 : '(#~(0&p:))(#~2&|) 2+ i.y'

NB. Next Composite
nc =: 3 : 0
a =. y + 2
while. 1 p: a do.
a =. a + 2
end.
a
)

NB. Is NOT Consecutive Composite Pair 
inc =: 3 : 0
'a b' =. y
-.b = nc a
)

NB. Invalid Pairs
ip =: 3 : '(2 inc\ splf y) # (2 <\ splf y)'

NB. ip 1000 shows first invalid pair is 5775 5781
NB. ip 1500 stays the same
NB. Therefore I tried nc 5775 as the answer and it was accepted.
NB. Bit cheaty as I wasn't certain but website OK'd it.

NB. olegyk's answer
goldb=: e. (i.&.(p:^:_1) ,@:(+/) (>:@i.@<.)&.(%:@-:)@<:)
olegyk=: (#~ -.@goldb"0) (>:@+: -. p:)2+i.10000
NB. 5777 5993 

NB. Another one
is=.+./@:(=<.)@:%:@:-:@:(- p:@:i.@(_1&p:))
Edushka =: {. (#~ -.@is"0) (#~ -.@(1&p:)) >: +: >:i.3000
NB. 5777 