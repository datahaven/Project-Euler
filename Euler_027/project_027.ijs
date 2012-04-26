NB. l=: 'C:\Users\Adrian\Documents\Projects\Puzzles\ProjectEuler\project_027.ijs'
NB. load l

NB. eulerFn takes three digits (a b n) and returns
NB. n^2 + an + b
efn =: 3 : 0
'a b n' =. y
(*: n) + (a*n) + b
)

NB. x primeCount y gives the number of consecutive primes
NB. for coefficients x and y
pc =: 4 : 0
a =. 0
while. 1 do.
 ev =. efn x, y, a
 if. -. 1 p: ev do.
  break.
 end.
 a =. >: a
end.
a
)

NB. primeCountTable - prime counts from -999 to +999
NB. pct =: pc"0/~ _999+i.1999

NB. max prime count - happens to be 71. Takes a while to produce.
NB. Using while loop messes performance up
NB. mpct =: >./ , pct

pc2 =: 4 : 0
a =. 0
while. 1 do.
 ev =. efn x, y, a
 if. -. 1 p: ev do.
  break.
 end.
 a =. >: a
end.
a;x;y
)

NB. pct2 =: pc2"0/~ _999+i.1999