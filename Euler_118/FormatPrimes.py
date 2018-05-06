# Project Euler 118
# Adrian Dale
# 03/04/2018
#
# Parse my big list of primes (from the internet)
# so that it only includes numbers with all unique non-zero digits
def hasUniqueDigits(digits):
	return '0' not in digits and len(set(digits)) == len(digits)
	
primes = []
for line in open('primes_unformatted01.txt'):
	primesOnLine = [x.strip() for x in line.split('\t')]
	for prime in primesOnLine:
		if hasUniqueDigits(prime):
			primes.append(prime)
			print(prime)
#print len(primes)
# There are 43089 of these!