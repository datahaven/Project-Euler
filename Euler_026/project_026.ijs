NB. l=: 'C:\Users\Adrian\Documents\Projects\Puzzles\ProjectEuler\project_026.ijs'
NB. load l

NB. Denominator is 1000 ... with 999 zeros
d =: 1,999999$0

NB. oo for One Over
NB. x oo y gives the xth digit of 1/y
ooOLD =: 4 : 0
dd =. x{d

remainder =. y | dd
div =. <. dd % y

NB. Modify next digit of d to add in the carry
nd =. (1+x){d
nd =. nd + remainder * 10
d =: nd ((1+x)})d
NB. return div
div
)



NB. x oo y where x is digit position;previous carry
NB. Brackets needed (3;4) oo 998
oo =: 4 : 0
dp =. {. > x
pc =. {: > x
dd =. (pc * 10) + dp{d
remainder =. y | dd
div =. <. dd % y
div;remainder
)

NB. doDiv shows 1/y to 2000 dps
doDiv =: 3 : 0
rem =. 0
out =. 0
for_v. (i.2000) do.
fn =. (v;rem) oo y
rem =. {: > fn
out =. out , {. > fn
end.
NB. drop off the extra 0 we added, and also drop off
NB. the one that will always be first.
NB. (A bit hacky, but then isn't all of this?)
}. }. out
)

NB. All of them from d=1 to 1000
NB. doAllDiv =: doDiv ("0) 1+i.1000

NB. x isLen y returns true if y starts with repeated
NB. sequence of length x
isLen =: 4 : 0
a =. (x*2){.y
b =. x{.y
c =. x}.a
*/ (=/)> (b;c)
)

NB. Give up - write this the crappy way
isAllLen =: 3 : 0
b =. doDiv y
a =. 0
for_v. (5+i.994) do.
 a =. v isLen b 
 if. a do.
   v
   break.
 end.
end.
)

NB. This is slow if run for all integers 0 to 1000.
NB. (Probably still within the minute limit, though.)
NB. Much faster when we know the answer is a cycle of
NB. 982 digits using >./ to find the max answer.
NB. Then we can narrow the fn down to this and visually see
NB. the answer
al =: isAllLen ("0) 980+i.20

NB. 0 108 0 982 0 0 0 138 0 462 0 495 0 110 0 0 0 166 0 6
NB.         === is the correct answer!