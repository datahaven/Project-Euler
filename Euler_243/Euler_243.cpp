// Euler 243
// Adrian Dale 08/10/2011
//
// You are the 1453rd person to have solved this problem.
// 892371480
//
// Not entirely sure how I solved this!
// Solution is in test02. A load of junk is elsewhere.
// It works because of
// "Therefore the values of n corresponding to particularly small values of the ratio
// are those n that are the product of an initial segment of the sequence of all primes."
// which is on wikipedia.
// At first I'd just calculated phi for product of primes, but answer is a product (any no.)
// of a product of primes.
//
// Impressive that I can get the euler_sieve size up to 480,000,000 unsigned ints in one
// array. Worth knowing!
#include <iostream>
#include <vector>
using namespace std;

typedef unsigned long long ull;

#if 0
typedef unsigned long long ull;
typedef unsigned int u32;

const u32 SIEVE_SIZE = 1000000;



// This is about as big as I can get it!
const u32 PHISIZE = 1000;
u32 gPhi[PHISIZE];
// Code by CFB from another Euler solution thread
// (Hexagonal Orchards)
void eulerphi(u32 n, u32 *phi)
{
  for (u32 i=1; i<=n; i++) {
    phi[i]=i;
  }
  for (u32 i=2; i<=n; i++) {
    if (phi[i]==i) {                // i is prime
      phi[i]-=1;                    // phi(i)=i-1
      for (u32 j=i+i; j<=n; j+=i) { // Handle all multiples of i
        phi[j]=phi[j]/i*(i-1);      // Apply (i-1)/i to this multiple of i
      }
    }
  }
}

int main()
{
	eulerphi(PHISIZE, gPhi);
	double minRatio = 1.0;
	for(int i=1; i<PHISIZE; ++i)
	{
		double R = (double)gPhi[i] / (double)(i-1);
		if ( R < 0.16358819555855 )
		{
			cout << "i=" << i << endl;
		}
		if ( R < minRatio )
		{
			minRatio = R;
			cout << "new min i=" << i << " mr=" << minRatio << endl;
		}
	}
	cout << "minRatio=" << minRatio << endl;
	cout << "phi(210)=" << gPhi[210] << endl;
	return 0;
}
#endif

vector<unsigned long> primes;
const unsigned long MAX_PRIMES = 100;

// I doubt I wrote this!
int phi(int n) 
{ 
	int result = n; 
	for(int i=2;i*i <= n;i++) 
	{ 
		if (n % i == 0) result -= result / i; 
		while (n % i == 0) n /= i; 
	} 
	if (n > 1) result -= result / n; 
	return result; 
} 


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

double getR(unsigned long long pp, int i)
{
	double phi = pp;
	for(int j=0; j<=i; ++j)
	{
		phi *= (1.0-(1.0/(double)primes[j]));
	}
	return phi / ((double)pp-1.0);
}

int test01(void){
    
    get_primes(MAX_PRIMES);
    // return 0;
    // Print out result.
    vector<unsigned long>::iterator it;
	unsigned long long prime_product = 1;
    for(int i=0; i < MAX_PRIMES; ++i)
	{
        //cout << *it << " ";
		prime_product *= primes[i];
		cout << "pp=" << prime_product << " R=" << getR(prime_product, i) << endl;
	}
    
    cout << endl;
    return 0;
}

int test02()
{
	cout << "phi(210)=" << phi(210) << endl;
	unsigned long long prime_product = 1;
    for(int i=0; i < MAX_PRIMES; ++i)
	{
        //cout << *it << " ";
		prime_product *= primes[i];
		for(int j=1; j<primes[i]; ++j)
		{
			double R= phi(j * prime_product) / ((j*prime_product)-1.0);
			if ( R < (15499.0/94744.0))
			{
				cout << "pp*j=" << j*prime_product << " R=" << R << endl;
				return 0;
			}
		}

		
	}
	return 0;
}

int main()
{
	get_primes(MAX_PRIMES);
	test02();
	return 0;
}