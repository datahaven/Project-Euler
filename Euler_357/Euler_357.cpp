// Project Euler 357
// Prime Generating Integers
// Adrian Dale 16/03/2019

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<unsigned long> primes;

bool isPrime(unsigned long n)
{
	return binary_search(primes.begin(), primes.end(), n);
}

// http://code.activestate.com/recipes/576559-fast-prime-generator/
vector<unsigned long> get_primes(unsigned long max) {
	vector<unsigned long> primes;
	char *sieve;
	sieve = new char[max / 8 + 1];
	// Fill sieve with 1  
	memset(sieve, 0xFF, (max / 8 + 1) * sizeof(char));
	for (unsigned long x = 2; x <= max; x++)
		if (sieve[x / 8] & (0x01 << (x % 8))) {
			primes.push_back(x);
			// Is prime. Mark multiplicates.
			for (unsigned long j = 2 * x; j <= max; j += x)
				sieve[j / 8] &= ~(0x01 << (j % 8));
		}
	delete[] sieve;
	return primes;
}

int main()
{
	cout << "Generating Primes..." << endl;
	
	primes = get_primes(100100000);
	cout << "Finished generating primes." << endl;
	
	// Basic brute force solution - whole thing takes around ten seconds
	unsigned long long sumN = 0;
	unsigned long count = 0;
	for (unsigned long n = 1; n <= 100000000; ++n)
	{
		bool allOK = true;
		for (unsigned long d = 1; d <= 10000 && d <= n; ++d)
		{
			if (n % d == 0)
			{
				if (isPrime(d + (n / d)) == false)
				{
					allOK = false;
					break;
				}
			}
		}
		if (allOK)
		{
			sumN += n;
			++count;
		}
	}
	cout << "sum=" << sumN << endl;
	cout << "count=" << count << endl;
}
