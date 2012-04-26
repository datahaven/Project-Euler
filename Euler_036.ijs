NB. l =: 'd:\users\adrian\documents\projecteuler\project36.ijs'
NB. load l

NB. "."0 '123'
NB. Turns 123 into 1 2 3

NB. #: 123
NB. Turns 123 into a binary number 1 0 1 1 etc

NB. |. 1 2 3 is 3 2 1
NB. isPalindromic
isp =: 3 : 0
*./ y ="1 |. y
)

NB. isp ("1) 1 2 3 2 1 is 1

tst =: 3 : 0
bin =. #: y
dec =. "."0 ": y
(isp bin) * (isp dec)
)

answer =: +/ (#~tst("0)) 1+i.1000000

NB. "pro" answer
answer2 =: +/ I. ((-:|.)@#: *. (-:|.)@":)"0 i.1e6