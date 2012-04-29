// Project Euler 381
// Adrian Dale
// 28/04/2012
//
// Congratulations, the answer you gave to problem 381 is correct.
//
// You are the 410th person to have solved this problem.
//
// 139602943319822
// Takes a couple of seconds to run.
//
// I worked out some of the stuff relating to Wilson's theorem by myself.
// Other bits I found on the net.
// (p-1)! % p == p - 1
// (p-2)! % p == 1
// Therefore ((p-1)! + (p-2)! ) % p == 0
// (p-3)! % p == (p-1)/2
//
// As it happened I'd already got all the bits I needed myself but the problem
// was in the modInverse code which was overflowing. Once I fixed that my
// original code worked.

#include <iostream>
#include <vector>
using namespace std;

// get_primes is from:
// http://code.activestate.com/recipes/576559-fast-prime-generator/

vector<unsigned long> primes;

void get_primes(unsigned long max){    
    char *sieve;
    sieve = new char[max/8+1];
    // Fill sieve with 1  
    memset(sieve, 0xFF, (max/8+1) * sizeof(char));
    for(unsigned long x = 2; x <= max; x++)
        if(sieve[x/8] & (0x01 << (x % 8))){
            primes.push_back(x);
            // Is prime. Mark multiplicates.
            for(unsigned long j = 2*x; j <= max; j += x)
                sieve[j/8] &= ~(0x01 << (j % 8));
        }
    delete[] sieve;
}

// modInverse and pow are from http://comeoncodeon.wordpress.com/2011/10/09/modular-multiplicative-inverse/
/* This function calculates (a^b)%MOD */
unsigned long long pow(unsigned long long a, unsigned long long b, unsigned long long MOD) {
	unsigned long long x = 1, y = a;
    while(b > 0) {
        if(b%2 == 1) {
            x=(x*y);
            if(x>MOD) x%=MOD;
        }
        y = (y*y);
        if(y>MOD) y%=MOD;
        b /= 2;
    }
    return x;
}
 
unsigned long long modInverse(unsigned long long a, unsigned long long m) {
    return pow(a,m-2,m);
}

// Not used in final code - just used for testing
unsigned long facNmodP(unsigned long n, unsigned long p)
{
	unsigned long fac = 1;
	for(unsigned long i=1; i<=n; ++i)
		fac = (fac * i) % p;
	return fac;
}

// Uses Wilson's theorem, so I don't have to calculate any factorials
// because I know (p-1)! % p is p-1 if p is prime.
// Can work out the other factorials from that by dividing them by
// p-1, p-2, etc
unsigned long long S(unsigned long p)
{
	unsigned long long sum = 0ull;
	
	unsigned long long sumpart = p-1;
	sum = sumpart;
	for(int k=1; k<=4; ++k)
	{
		unsigned long long mI = modInverse(p-k, p);
		sumpart = (sumpart * mI)%p;
		sum += sumpart;
		sum %= p;
	}
	
	return sum;
}

// Gives the same answers as S above, but I wrote this one after
// seeing the results in comments below in someone else's solution.
// The original S function was all my work, though.
unsigned long long S2(unsigned long long p)
{
	unsigned long long sum = 0ull;
	// ((p-1)! + (p-2)!) % p == 0
	// (p-3)! % p == (p-1)/2
	sum = (p-1) / 2;

	// (p-4)! == (p-3)!/(p-3)
	unsigned long long p4f = (sum * modInverse(p-3, p)) % p;

	// (p-5)! == (p-4!)/(p-4)
	unsigned long long p5f = (p4f * modInverse(p-4, p)) % p;

	sum = (sum + p4f) % p;
	sum = (sum + p5f) % p;
	return sum;
}

// I used this for testing - it isn't fast enough to calculate the final solution.
unsigned long oldS(unsigned long p)
{
	return (/*facNmodP(p-1, p) + facNmodP(p-2, p) + */ facNmodP(p-3, p) + facNmodP(p-4, p) + facNmodP(p-5, p))%p;
} 

unsigned long long solve()
{
	unsigned long long sum = 0ull;
	vector<unsigned long>::iterator it = primes.begin();
	int count = 0;
	for(it = it+2; it!=primes.end(); ++it)
	{
		unsigned long long Sres = S(*it);
		sum += Sres;

		// Here's how I test different versions of the functions against eachother.
		//unsigned long oldSres = oldS(*it);
		//if (oldSres != Sres)
		//{
		//	cout << "Error: " << *it << " " << oldSres << " " << Sres << endl;
		//	break;
		//}
		//cout << "S(" << *it << ")=" << Sres << " sum=" << sum << endl;
	}
	return sum;
	
}

int main()
{
	cout << "Filling primes array" << endl;
	get_primes(100000000);
	
	cout << "Done Primes!" << endl;

	cout << "Found " << primes.size() << " primes" << endl;

	cout << solve() << endl;
	
	return 0;
}