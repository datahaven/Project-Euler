// Project_003.cpp 
// Adrian Dale 10/01/2009
//
// The prime factors of 13195 are 5, 7, 13 and 29.
//
// What is the largest prime factor of the number 600851475143 ?
//
// LPF(600851475143)=6857
//

// This code also solves Project 007 - find 10001st prime
// Answer: 104743
//

// As before, this is very brute force. It works but is a little slow.
// Takes a couple of minutes to run. There has to be a better way?
// Maybe writing an isPrime() fn, rather than pre-calculating all the primes.
// Helps because we know we probably won't need to check the earlier ones.

// Project 10
// The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.
//
// Find the sum of all the primes below two million.
// Answer: 142913828922

// Updated Notes: 23/04/2012
// While revisiting this and converting it so it would build in the current
// MS VC++ 2010 Express I spotted that although it gives the correct answer
// in Release mode for Problem 3, in Debug mode it throws an assertion.
// I haven't investigated this.
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

vector<bool> isPrime;

// Fill the prime array with all the primes
// up to n
// Uses "Sieve of Eratosthenes" method
//
// I speeded this up a lot by stepping by i in the loop,
// rather than by one.
// 
// http://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
//
// My idea of skipping all the even no's initially is discussed
// in the official solution and would still help quite a lot
// in terms of reduced storage needs.
void FillPrimeArray( int n )
{
	isPrime.resize(n);
	fill( isPrime.begin(), isPrime.end(), true );
	isPrime[0] = false;
	isPrime[1] = false;

	int i = 2;

	while( i < n )
	{
		if ( isPrime[i] )
		{
			for ( int j = i+i; j<n; j+=i )
			{
				// Does it help to check if j is already false in order
				// to save us a j%i operation?
				if ( j % i == 0 )
					isPrime[j] = false;
			}
		}

		if ( i % 50000 == 0 )
			cout << "i=" << i << endl;

		i++;
	}
}

unsigned __int64 LPF(unsigned __int64 n)
{
	int answer = 0;

	FillPrimeArray( static_cast<int>( floor( sqrt( static_cast<float>(n) + 0.5 ) )));

	
	// This code is WRONG - it finds the largest prime divisor.
	// eg for 13195 it returns 91 as 91*145=13195.
	// But 145 is not prime.
	//
	// For the challenge input 600851475143 we 
	// get 486847, which does not match the official answer
	// OK - not true. I had a bug. I hadn't put the isPrime[i] clause in. Doh!

	for( int i=isPrime.size(); i > 2; --i )
		if ( isPrime[i] && n % i == 0 )
		{
			answer = i;
			break;
		}
	

	return answer;
}

void PrimeStats()
{
	int iPrimeCount = 0;
	for( int i=0; i<isPrime.size(); ++i )
	{
		if ( isPrime[i] )
			iPrimeCount++;
		if ( iPrimeCount == 10001 )
			cout << "10001st prime is " << i << endl;
			// Answer is 104743 - take the first one that comes out.
			// Bit naff but could put a simple found bool in
	}

	cout << "No. of primes less than " << isPrime.size() << " is " << iPrimeCount << endl;

}

void Project10()
{
	__int64 iPrimeSum = 0;
	for( int i=0; i<isPrime.size(); ++i )
		if (isPrime[i])
			iPrimeSum += i;

	cout << "iPrimeSum=" << iPrimeSum << endl;
}

int main(int argc, char* argv[])
{
	// cout << "limit=" << numeric_limits<unsigned __int64>::max() << endl;

	

	// For project 7 - we get the 150000 from a bit of trial and
	// error with the stats fn.
	// FillPrimeArray(150000);
	//PrimeStats();


	// For project 3
	// cout << "LPF(13195)=" << LPF(13195) << endl;
	cout << "LPF(600851475143)=" << LPF(600851475143) << endl;

	// For project 10
	//FillPrimeArray(2000000);
	// Hmmm ... this would work, but would take ages!
	// OK, not so bad now I've got the sieve working properly!
	//Project10();

	return 0;
}
