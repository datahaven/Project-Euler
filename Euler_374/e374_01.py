
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
	
# Much faster version of P(n) about twice the speed
# http://homepages.ed.ac.uk/jkellehe/partitions.php
def accelAsc(n):
    a = [0 for i in range(n + 1)]
    k = 1
    a[0] = 0
    y = n - 1
    while k != 0:
        x = a[k - 1] + 1
        k -= 1
        while 2*x <= y:
            a[k] = x
            y -= x
            k += 1
        l = k + 1
        while x <= y:
            a[k] = x
            a[l] = y
            yield a[:k + 2]
            x += 1
            y -= 1
        a[k] = x + y
        y = x + y - 1
        yield a[:k + 1]

# Slower version
#http://code.activestate.com/recipes/218332-generator-for-integer-partitions/		
def P(n):
	# base case of recursion: zero is the sum of the empty list
	if n == 0:
		yield []
		return

	# modify partitions of n-1 to form partitions of n
	for p in P(n-1):        
		yield [1] + p
		if p and (len(p) < 2 or p[1] > p[0]):
			yield [p[0] + 1] + p[1:]

def isDP(partition):
    s = set()
    for x in partition:
        if x in s: return False
        s.add(x)
    return True

	
def distinctPartitions(n):
	partitions = accelAsc(n)
	return list(filter(isDP,partitions))
	
def maxPartProd(dp):
	maxprod=0
	maxpart=[]
	for p in dp:
		prod=1
		for x in p:
			prod = prod * x
		if prod > maxprod:
			maxprod = prod
			maxpart = p
	#return len(maxpart) * maxprod
	return (maxpart, maxprod)
	
# Does the calculation "for real" so I can see what the
# results look like
def slowTest():
	sum = 0
	for n in range(1,101):
		start = time.time()
		(maxpart, mp) = maxPartProd(distinctPartitions(n))
		sum = sum + mp
		elapsed = (time.time() - start)
		#print("n=", n, "mp=", mp, "sum=", sum, "time=", elapsed)
		print("n=",n,"mp=",mp,"len=", len(maxpart), maxpart)

def modreduce(pl):
	prod = 1
	for x in pl:
		prod = prod * x
		prod = prod % modnum
	return prod
	
# This recreates the pattern I found using slowTest
# Much faster but still won't handle N=10^14		
def doSeq():
	# NB skipping n=1..4 means start sum at 10
	sum = 10
	
	n = 5
	partlen = 2
	nmax = 100000
	while n<=nmax:
		partcount = 0
		partlist = list(range(2,2+partlen))
		incpos = partlen-1
		
		while partcount < partlen+2 and n<=nmax:
			mp = modreduce(partlist)
			sum = sum + (mp*partlen)
			sum = sum % modnum
			#print("n=",n,"mp=",mp,"sum=",sum)
			print("n=",n,"sum=",sum)
			
			partlist[incpos] = partlist[incpos] + 1
			incpos = incpos - 1
			if incpos < 0:
				incpos = partlen-1
			partcount = partcount + 1
			n = n + 1
		partlen = partlen+1

# Faster version of doSeq that uses calculation to
# get result.
# Still takes 3.5 seconds for 250000, which is ten times
# faster than before but still not going to happen for 10^14
def doSeqII():
	# NB skipping n=1..4 means start sum at 10
	sum = 10
	#sumset = set()
	n = 5
	partlen = 2
	#nmax = 100000000000000
	nmax = 1000000
	while n<=nmax:
		partcount = 0
		nfac = factorial(partlen+2)
		
		while partcount < partlen+1 and n<=nmax:
			mp = nfac // ((partlen-partcount+2) )
			sum = sum + (mp*partlen)
			sum = sum % modnum
			
			# doesn't seem to be a pattern.
			# only occasional overlaps found
			mpmod = (mp*partlen)%modnum
			#if mpmod in sumset:
			#	print("mpmod found:",mpmod)
				#exit()
			#else:
			#	sumset.add(mpmod)
				
			print("n=",n,"mpmod=",mpmod,"sum=",sum)
			#print("n=",n,"sum=",sum)
			
			partcount = partcount + 1
			n = n + 1
		
		# last row
		sum = sum + (partlen * nfac * (partlen+3))//(2*(partlen+2))
		sum = sum % modnum
		n = n + 1
		
		partlen = partlen+1
	
	return sum
		
# Use what I've discovered about Generalised Stirling numbers
# to eliminate inner while loop
def doSeqIII():
	# NB skipping n=1..4 means start sum at 10
	sum = 10
	
	n = 4
	partlen = 2
	nmax = 100000000000000
	#nmax = 100
	aprev = 5
	while n<=nmax:
		print("n=",n,"partlen=",partlen,"sum=",sum)
		nfac = factorial(partlen+1)
		anext = aprev * (partlen+2) + nfac
		#print("anext=",anext)
		
		
		sum = sum + (anext*partlen)
		sum = sum % modnum
			
		n = n + partlen + 2
		aprev = anext
		
		# last row
		#lastrow = (partlen * nfac * (partlen+1) * (partlen+3))//(2*(partlen+1))
		# cancel out (partlen+1)
		lastrow = (partlen * nfac * (partlen+3))//(2)
		#print("lastrow=",lastrow)
		sum = sum + lastrow
		sum = sum % modnum
		
		partlen = partlen+1
	
	# When this is done there will be some remainder left that we'll
	# need to calculate step by step.
	# I think next problem is to deal with the factorials of massive numbers
	print("Finished quick bit with last good n=",n-partlen-1)
	
	return sum

# doSeqIII works, so do experiments on this copy
def doSeqIV():
	# NB skipping n=1..4 means start sum at 10
	sum = 10
	n = 4
	partlen = 2
	nmax = 100000000000000
	nmax = 100
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
	
	print("Finished quick bit with last good n=",n-partlen-1,"partlen=",partlen,"nfac=",nfac)
	print("oldsum=",oldsum)
	n = n-partlen
	sum = oldsum
	partcount = 0
	partlen = partlen - 1
	
	while partcount < partlen+1 and n<=nmax:
		#mp = nfac // ((partlen-partcount+2) % modnum)
		mp = ModFacDiv(partlen+2, partlen-partcount+2)
		sum = sum + (mp*partlen)
		sum = sum % modnum
			
		#print("n=",n,"sum=",sum)
			
		partcount = partcount + 1
		n = n + 1
	
	print("sum=",sum,"n=",n, "pl=",partlen, "pc=", partcount)
	return sum

# Generalised Stirling Number	
def GSNo():
	aprev=0
	n=1
	nmax=10
	while n<=nmax:
		nfac = factorial(n)
		anext = aprev*(n+1) + nfac
		print("n=",n,"anext=",anext)
		aprev = anext
		n = n + 1
		
# doSeqIV works, so do experiments on this copy
def doSeqV():
	# NB skipping n=1..4 means start sum at 10
	sum = 10
	n = 4
	partlen = 2
	nmax = 100000000000000
	nmax = 100
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
	
	mp = nfac // (partlen+2)
	#print("mp=",mp)
	#mp = ModFacDiv(partlen+2,partlen+2)
	#print("mp=",mp)
	while partcount < partlen+1 and n<=nmax:
		#mp = nfac // ((partlen-partcount+2) % modnum)
		mp = ModFacDiv(partlen+2, partlen-partcount+2)
		sum = sum + (mp*partlen)
		sum = sum % modnum
		
		mp = (mp * (partlen-partcount+2) * modnum) 
		mp = (mp // (partlen-partcount+1)) % modnum
		mp = mp % modnum
		#print("n=",n,"sum=",sum)
			
		partcount = partcount + 1
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

# Use this (from comeoncodeon site) to see how many times
# our prime no modnum appears in n!
# Then I think we need to multiply by p^k mod modnum before
# we recalculate mp
# NOT SURE ABOUT THIS - not the right thing to calculate?
def countFact(n):
	k=0
	while n > 0:
		k = k + (n // modnum)
		n = n // modnum
	return k
  
	
def MFTest():
	i = 20
	nfac = factorial(14) % modnum
	while i>2:
		#mfd = ModFacDiv(14,i)
		#mymfd = (nfac // i)
		#print("mfd(14,",i,")=",mfd,"mymfd=",mymfd)
		print("fm(",i,")=",factMod(i),countFact(i))
		i = i - 1
		

		
# print( distinctPartitions(5) )
#print( maxPartProd(distinctPartitions(10)) )
#slowTest()

start = time.time()
doSeqV()
MFTest()
elapsed = (time.time() - start)
print("time=",elapsed,"seconds")


# 100000 is 8.5 second
# 250000 is 33.6 seconds.
# 10^14 is WTF seconds