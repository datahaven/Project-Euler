// Project Euler 203
// Squarefree Binomial Coefficients
// Adrian Dale
// 10/01/2019

// Congratulations, the answer you gave to problem 203 is correct.
//
// You are the 7774th person to have solved this problem.
//
// This problem had a difficulty rating of 25 % .The highest difficulty rating you have solved so far is 100 %

#include <iostream>
#include <set>
#include <vector>

using namespace std;

typedef unsigned long long ULL;

const int MAX_ROWS = 51;
ULL gRows[MAX_ROWS][MAX_ROWS];

set<ULL> gDistinctNumbersInTriangle;

vector<ULL> gPrimesSquared;

void FillTriangle()
{
	//gRows[0][0] = 1;

	//gRows[1][0] = 1;
	//gRows[1][1] = 1;

	// Blank out all the triangle's cells.
	// I only need this because GetUniqueNumbers loops over the entire array.
	// Really this code could all go in one function and be done all at once.
	for (int r = 0; r < MAX_ROWS; ++r)
	{
		for (int c = 0; c < MAX_ROWS; ++c)
		{
			gRows[r][c] = 1ull;
		}
	}

	for (int r = 2; r < MAX_ROWS; ++r)
	{
		for (int c = 1; c < r ; ++c)
		{
			gRows[r][c] = gRows[r - 1][c - 1] + gRows[r - 1][c];
		}
	}
}

void GetUniqueNumbers()
{
	for (int r = 0; r < MAX_ROWS; ++r)
	{
		for (int c = 0; c < r; ++c)
		{
			gDistinctNumbersInTriangle.insert(gRows[r][c]);
		}		
	}

	//for (ULL x : gDistinctNumbersInTriangle)
	//{
	//	cout << x << endl;
	//}
	//cout << "Found " << gDistinctNumbersInTriangle.size() << " numbers" << endl;
}

// https://www.geeksforgeeks.org/c-program-for-sieve-of-eratosthenes/
void SieveOfEratosthenes(int n)
{
	// Create a boolean array "prime[0..n]" and initialize 
	// all entries it as true. A value in prime[i] will 
	// finally be false if i is Not a prime, else true. 
	bool prime[10000];
	memset(prime, true, sizeof(prime));

	for (int p = 2; p*p <= n; p++)
	{
		// If prime[p] is not changed, then it is a prime 
		if (prime[p] == true)
		{
			// Update all multiples of p 
			for (int i = p * 2; i <= n; i += p)
				prime[i] = false;
		}
	}

	// Print all prime numbers 
	for (int p = 2; p <= n; p++)
		if (prime[p])
		{
			//cout << p << " ";
			ULL p2 = p * p;
			gPrimesSquared.push_back(p2);
		}
}

bool IsSquareFree(ULL n)
{
	for (ULL p2 : gPrimesSquared)
	{
		if (n%p2 == 0)
			return false;
	}
	return true;
}

void TryForSquarefreeness()
{
	ULL sum = 0;
	for (ULL x : gDistinctNumbersInTriangle)
	{
		if (IsSquareFree(x))
		{
			sum += x;

			//cout << x << endl;
		}
	}
	cout << "Sum = " << sum << endl;
}

int main()
{
	FillTriangle();
	GetUniqueNumbers();
	// I know I only need the first 1000 or so primes as the largest prime
	// I need to test is sqrt of sqrt of 126410606437752
	SieveOfEratosthenes(5000);
	TryForSquarefreeness();
}