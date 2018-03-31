// Project Euler 434 - Rigid Graphs
// Adrian Dale
// 31/03/2018
//
// Congratulations, the answer you gave to problem 434 is correct.
//
// You are the 262nd person to have solved this problem.
//
// This problem had a difficulty rating of 75 %

// https://cms.math.ca/openaccess/cjm/v31/cjm1979v31.0060-0068.pdf
// Slightly easier to make out the text in this version, but it is a bit of a spoiler!
// https://www.johannesbader.ch/2013/09/project-euler-problem-434-rigid-graphs/
// I'd already found a link to the above paper before reading the spoiler site, though I hadn't
// realised the paper was available for free.
// I first attempted this problem in 2013, then came back to it today after flicking
// through "Graphs and their Uses" by Oystein Ore and recognising his discussion
// of the rigid grid problem. I then had to turn that into slightly more technical
// terminology to search for ways to count labelled connected bipartite graphs.
// There's an OEIS sequence for this A123260, but no code with it
#include <iostream>
using namespace std;

const  long long MODVALUE = 1000000033L;
const int MAX_N = 200;

long long PowersOfTwo[MAX_N * MAX_N + 1];
long long Memo[MAX_N + 1][MAX_N + 1];
long long FactorialMemo[MAX_N * MAX_N + 1];
long long nCrMemo[MAX_N + 1][MAX_N + 1];

long long Factorial( long long n)
{
	return FactorialMemo[n];
}

long long nCr( long long n,  long long k)
{
	// I think this division breaks the MOD code, so I've re-done the nCr calculation
	// to build it up without the division
	//return (Factorial(n) / ((Factorial(k) * Factorial(n - k)) % MODVALUE)) % MODVALUE;
	return nCrMemo[n][k];
}

 long long R( long long i,  long long j)
{
	if (Memo[i][j] != -1)
	{
		return Memo[i][j];
	}

	long long twoPower = PowersOfTwo[i*j];
	long long sum = 0L;
	for ( long long a = 1; a <= i; ++a)
	{
		for ( long long b = 0; b <= j; ++b)
		{
			if (a == i && b == j)
			{
				continue;
			}
			long long part1 = (nCr(i - 1, a - 1) * nCr(j, b)) % MODVALUE;
			long long part2 = (PowersOfTwo[(i - a)*(j - b)] * R(a, b)) % MODVALUE;
			sum += (part1 * part2) % MODVALUE;
			sum %= MODVALUE;
		}
	}
	Memo[i][j] = (twoPower - sum) % MODVALUE;
	return Memo[i][j];
}

long long S( long long N)
{
	// NB I could use my brains to do fewer iterations of this loop,
	// but since R(i,j) is cached in a memo it wouldn't
	// save an awful lot of time.
	long long result = 0;
	for ( long long i = 1; i <= N; ++i)
	{
		for ( long long j = 1; j <= N; ++j)
		{
			result += R(i, j);
			result %= MODVALUE;
		}
	}
	return result;
}

void Setup()
{
	// Set up powers of two cache
	int twoPower = 1;
	for (int i = 0; i <= MAX_N * MAX_N; ++i)
	{
		twoPower %= MODVALUE;
		PowersOfTwo[i] = twoPower;
		twoPower *= 2;
	}

	// Populate factorial memo
	long long factorial = 1;
	FactorialMemo[0] = 1L;
	for ( long long i = 1; i <= MAX_N * MAX_N; ++i)
	{
		factorial *= i;
		factorial %= MODVALUE;
		FactorialMemo[i] = factorial;
	}
	
	// Clear out results memo
	for (long long i = 0; i <= MAX_N; i++)
	{
		for (long long j = 0; j <= MAX_N; j++)
		{
			Memo[i][j] = -1;
		}
	}

	// Set up nCrMemo - I got this idea from Johannes' page, as it meant I
	// could calculate nCr without having to do division.
	// The caching doesn't make much difference for C++ ;-)
	for (long long i = 0; i <= MAX_N; i++)
	{
		for ( long long j = 0; j <= MAX_N; j++)
		{
			nCrMemo[i][j] = 0L;
		}
	}
	for (long long i = 0; i <= MAX_N; i++)
	{
		nCrMemo[i][0] = 1L;
	}
	for (long long n = 1; n <= MAX_N; ++n)
	{
		for (long long r = 1; r <= MAX_N; ++r)
		{
			nCrMemo[n][r] = (nCrMemo[n - 1][r - 1] + nCrMemo[n - 1][r]) % MODVALUE;
		}
	}
}

int main()
{
	Setup();
	cout << "Answer = " << S(100) << endl;
	return 0;
}