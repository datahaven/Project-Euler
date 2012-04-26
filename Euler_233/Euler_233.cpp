// Euler 233
// Adrian Dale 29/08/2010 and 09/10/2011
//
// Useful for "Expressing n as sum of squares:
// http://www.math.mtu.edu/mathlab/COURSES/holt/dnt/represent3.html
// http://mathforum.org/library/drmath/view/60738.html
//
// Handy hints I'd pretty much already got:
// http://stackoverflow.com/questions/623435/project-euler-problem-233
//
// Do I need Schinzel's Theorem? - No
// http://mathworld.wolfram.com/SchinzelsTheorem.html
//
// Code is currently at the point discussed in the Euler Forum
// http://forum.projecteuler.net/viewtopic.php?f=50&t=1464
// I'm getting correct answers for 38m but not for 10^10 or 10^11
// Final clue that solved that came from this spoiler
// http://d.hatena.ne.jp/jeneshicc/20090326/1238074227
//
// Answer=271204031455541309
// Takes about a minute to run.
// Congratulations, the answer you gave to problem 233 is correct.
// 
// You are the 471st person to have solved this problem.
//
// You have earned 1 new award:
//
//    Fibonacci Fever: Solve the first twelve Fibonacci numbered problems
//
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

typedef unsigned long long ull;

vector<ull> primes;
vector<ull> pmod; // primes of form 4k+1

const unsigned long MAX_PRIMES = 5000000;
const ull N = 100000000000ull;

// From http://code.activestate.com/recipes/576559-fast-prime-generator/
// and altered to make primes a global variable to save it being
// copied
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

// Returns the sum of all possible n, starting from the base
// and considering all non-4k+1 prime multiples
ull countAll(ull base)
{
	// Include the base itself
	ull nsum = base;

	// Try multiples of the base
	for(ull k=2; k*base <= N; ++k)
	{
		// is k a multiple of a pmod value?
		bool kmult = false;
		for(ull i=0; i<pmod.size(); ++i)
		{
			if (k % pmod[i] == 0)
			{
				kmult = true;
				break;
			}
			if (k<pmod[i])
				break;
		}
		
		if (kmult==false)
		{
			ull wsum = base*k;
			if (wsum <= N)
				nsum += wsum;	
		}
	}

	return nsum;
}

ull solve123()
{
	// Store a list of all the primes where p[i]%4==1
	for(int i=0; i<primes.size(); ++i)
		if (primes[i]%4 == 1)
			pmod.push_back( (ull)primes[i] );

	ull solcount=0;
	ull nsum = 0;
	for(int a=0; a<pmod.size(); ++a)
	{
		ull a3 = pmod[a]*pmod[a]*pmod[a];
		if (a3 > N)
			break;
		
		for(int b=0; b<pmod.size(); ++b)
		{
			if (b==a)
				continue;

			ull b2 = pmod[b]*pmod[b];
			if (a3*b2 > N)
				break;

			for(int c=0; c<pmod.size(); ++c)
			{
				if (a==c || b==c)
					continue;
				ull c1 = pmod[c];
				if (a3*b2*c1 > N)
					break;
				ull base = a3*b2*c1;

				nsum += countAll(base);
			}
		}
	}
	return nsum;
}

// Same as solve123 but for a^3 * b^7
ull solve37()
{
	ull nsum = 0;
	for(int a=0; a<pmod.size(); ++a)
	{
		ull a3 = pmod[a]*pmod[a]*pmod[a];
		if (a3 > N)
			break;
		
		for(int b=0; b<pmod.size(); ++b)
		{
			if (b==a)
				continue;

			ull b7 = pmod[b]*pmod[b]*pmod[b]*pmod[b]*pmod[b]*pmod[b]*pmod[b];
			if (a3*b7 > N)
				break;

			ull base = a3*b7;
			
			nsum += countAll(base);		
		}
	}
	return nsum;
}

// Same solve37 but for a^2 * b^10
// would be tidier to combine these.
ull solve210()
{
	ull nsum = 0;
	for(int a=0; a<pmod.size(); ++a)
	{
		ull a2 = pmod[a]*pmod[a];
		if (a2 > N)
			break;
		
		for(int b=0; b<pmod.size(); ++b)
		{
			if (b==a)
				continue;

			ull b10 = pmod[b]*pmod[b]*pmod[b]*pmod[b]*pmod[b]*
				pmod[b]*pmod[b]*pmod[b]*pmod[b]*pmod[b];
			if (a2*b10 > N)
				break;

			ull base = a2*b10;
			nsum += countAll(base);	
		}
	}
	return nsum;
}

int main()
{
	get_primes(MAX_PRIMES);
	
	ull nsum = solve123();
	nsum += solve37();
	nsum += solve210();
	cout << "Answer=" << nsum << endl;
}

