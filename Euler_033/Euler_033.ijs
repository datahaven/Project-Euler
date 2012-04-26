
NB. l =: 'd:\users\adrian\documents\projecteuler\project33.ijs'
NB. load l

f1 =: 4 : '<x, y'

NB. All possible num/denom pairs
f2 =: ,f1("0)/~ 10+i.89

testf2 =: ,f1("0)/~ 10+i.3