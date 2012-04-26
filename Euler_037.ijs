
NB. l =: 'd:\users\adrian\documents\projecteuler\project37.ijs'
NB. load l


NB. 37 / 73 is the only two digit truncatable prime pair

NB. 137, 173 - no!

NB. from example: 3797
NB. 37, 73


NB.   ] \ '3797'
NB.3   
NB.37  
NB.379 
NB.3797
NB.   ] \. '3797'
NB.3797
NB.797 
NB.97  
NB.7

fn1 =: 3 : 0
a =. ": y
b =. < \ a
c =. < \. a
d =. b,c
*/ fn2 > d
)

fn2 =: 3 : 0
1 p: 0". y
)

fn3 =: 3 : 0
(#~fn1("0)) 10+i.y
)

answer =:  +/fn3 1000000
NB. 748317

NB. olegyk's answer:
NB. P=: p:i.100000
NB. digits=: #:~ 10&(>:@<.@^.#[)
NB. trunc=: 10&#.\ , 10&#.\.
NB. +/(#~ *./@:(e.&P)@trunc@digits"0) 4}.P
