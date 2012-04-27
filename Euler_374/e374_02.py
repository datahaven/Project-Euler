
# http://cs.utsa.edu/~wagner/knuth/fasc3b.pdf

import time
import operator
from functools import reduce
from math import factorial

modnum = 982451653

# my version of factorial uses modnum to keep the 
# number sizes down - works fine, but I think I'll not
# use this as it doesn't make sense to recalculate nfac each time
# when we only need to store previous version and multiply
# by next desired value
def ModFac(n):
	res = 1
	i=1
	while i<=n:
		res = (res * i) % modnum
		i = i + 1
	return res

# my version of factorial uses modnum to keep the answer smaller
# divides the answer by d.
# A bit crude - is this the best way to solve the issue I had?
# No - it is way too slow to repeatedly call this.
# First call is for n=14142136
def ModFacDiv(n,d):
	#print("mfd:",n,d)
	res = 1
	i=1
	while i<=n:
		if i != d:
			res = (res * i) % modnum
		i = i + 1
	#print(res)
	return res

MFDCi = 1
MFDCr = 1
# n! divided by d, where b is one less than
# the lowest value of d we will ever ask for
def ModFacDivCache(n,d,b):
	#print("mfdc:",n,d,b)
	res = 1
	i=1
	global MFDCi
	global MFDCr
	
	if MFDCi==1:
		while i<=b:
			res = (res * i) % modnum
			i = i + 1
		MFDCi = i
		MFDCr = res
		print("Ci=",MFDCi,"Cr=",MFDCr)
	else:
		i = MFDCi
		res = MFDCr
		
	while i<=n:
		if i != d:
			res = (res * i) % modnum
		i = i + 1
	#print(res)
	return res
	
# doSeqIV works, so do experiments on this copy
def doSeqV():
	# NB skipping n=1..4 means start sum at 10
	sum = 10
	n = 4
	partlen = 2
	nmax = 100000000000000
	#nmax = 100
	aprev = 5
	nfac = 6
	while n<=nmax:
		oldsum = sum # a bit crude - should just break in the right place
		#print("n=",n,"partlen=",partlen,"sum=",sum)
		#nfac = factorial(partlen+1) % modnum
		#nfac = ModFac(partlen+1)
		anext = aprev * (partlen+2) + nfac
		anext = anext % modnum
		
		sum = sum + (anext*partlen)
		sum = sum % modnum
			
		n = n + partlen + 2
		aprev = anext
		
		lastrow = (partlen * nfac * (partlen+3))//(2)
		#print("lastrow=",lastrow)
		sum = sum + lastrow
		sum = sum % modnum
		
		partlen = partlen+1
		nfac = (nfac * (partlen+1)) % modnum
	
	print("Finished quick bit with last good n=",n-partlen-1,"partlen=",partlen,"nfac=",nfac,"n=",n)
	print("oldsum=",oldsum)
	n = n-partlen
	sum = oldsum
	partcount = 0
	partlen = partlen - 1
	
	#mp = nfac // (partlen+2)
	#print("mp=",mp)
	#mp = ModFacDiv(partlen+2,partlen+2)
	#print("mp=",mp)
	mfdcstart = partlen-(nmax-n)
	partcount = nmax - n
	while n<=nmax:
		#mp = nfac // ((partlen-partcount+2) % modnum)
		#mp = ModFacDiv(partlen+2, partlen-partcount+2)
		mp = ModFacDivCache(partlen+2, partlen-partcount+2, mfdcstart)
		# Much faster, but still not enough!
		sum = sum + (mp*partlen)
		sum = sum % modnum
		
		print("n=",n,"mp=",mp,"sum=",sum)
			
		partcount = partcount - 1
		n = n + 1
	
	print("sum=",sum,"n=",n, "pl=",partlen, "pc=", partcount)
	return sum

# http://comeoncodeon.wordpress.com/tag/factorial/
def factMod(n):
	res = 1
	while n > 0:
		i=2
		m=n%modnum
		while i<=m:
			res = (res*i) % modnum
			i=i+1
		n = n // modnum
		if n %2 > 0:
			res = modnum - res
	return res


  
	
def MFTestOLD():
	i = 14142136
	nfac = factorial(14) % modnum
	while i>=12392314:
		#mfd = ModFacDiv(14142136,i)
		mfd=0
		mfdc = ModFacDivCache(14142136,i,12392313)
		mfdc = ModFacDivCache(14142136,12392314,12392313)
		#mymfd = (nfac // i)
		print("mfd(14,",i,")=",mfd,"mfdc=",mfdc)
		#print("fm(",i,")=",factMod(i),countFact(i))
		i = i - 1

def MFTest():
	
	i = 12392314
	while i < 12392330:
		mp = ModFacDiv(14142136,i)
		print("i=",i,"mp=",mp)
		#print("i=",i,"mp=",mp,"mp3=",mp3,mp4)
		i=i+1
		
def MFTest2():
	res = 1
	for x in range(12392315,14142137):
		res = (res * x)%modnum
	print( res)
		

		
# print( distinctPartitions(5) )
#print( maxPartProd(distinctPartitions(10)) )
#slowTest()

start = time.time()

doSeqV()
#MFTest2()

elapsed = (time.time() - start)
print("time=",elapsed,"seconds")


# 100000 is 8.5 second
# 250000 is 33.6 seconds.
# 10^14 is WTF seconds