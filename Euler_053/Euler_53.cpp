// Project Euler 53
// http://projecteuler.net/index.php?section=problems&id=53
// Adrian Dale 27/09/2010
//
// Fairly quick to implement - 20mins?
#include <iostream>
using namespace std;

typedef long long LLT;

// n!
// Works up to N==20 for long long version
// Works up to N==13 for unsigned/signed int version)
LLT fac(LLT N)
{
	LLT res = 1;
	for(LLT i=2;i<=N;++i)
		res *= i;
	return res;
}

// nCr
// The number of ways of picking r unordered outcomes from n possibilities
// n! / r!(n-r)!
LLT nCr(int N, int R)
{
	if ( R < N/2 )
		R = N - R;

	LLT res = 1;
	int n = N;
	while(n>R)
		res *= n--;
	return res / fac(N-R);
}

// Counts all the values of nCr supplied n that are greater than 1000.
// Only needs to find the first one that exceeds 1000000, then we know all the
// rest will be bigger as r increases until we reach r=n-r.
// This works because nCr == nC(n-r)
LLT countFac(LLT n)
{
	LLT r;
	bool found = false;
	for(r=0; r<n/2; ++r )
	{
		if ( nCr(n, r) > 1000000 )
		{
			found = true;
			break;
		}
	}
	if (found)
	{
		//cout << "For " << n << " we get " << n << "_C_" << r << " > 1000000" << endl;
		return n - 2*r + 1;
	}
	return 0;
}

int main()
{
	LLT sum = 0;
	// We've already been told that the first value to look at is 23
	for( LLT i=23; i<=100; ++i )
		sum += countFac(i);
	cout << "Answer=" << sum << endl;
	return 0;
}
