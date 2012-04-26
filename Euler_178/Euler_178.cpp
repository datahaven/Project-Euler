// Project Euler 178
// Consider the number 45656.
// It can be seen that each pair of consecutive digits of 45656 has a difference of one.
// A number for which every pair of consecutive digits has a difference of one is called a step number.
// A pandigital number contains every decimal digit from 0 to 9 at least once.
// How many pandigital step numbers less than 10^(40) are there? 
//
// Adrian Dale 30/09/2010
//

#include <iostream>
using namespace std;
typedef long long LLT;

LLT memo[41][1024][11];

// n is no. of digits left to try
// d is a bit set of which digits we've already tried
// pd is the previous digit
LLT solve(int n, int d, int pd)
{
	LLT &m = memo[n][d][pd+1];
	if ( m != -1 )
		return m;

	LLT sol = 0;
	
	if ( n == 0 )
	{
		m = d == 1023 ? 1 : 0;
		return m;
	}
	if ( pd == -1 )
		for(int i=1;i<=9;++i)
			sol += solve(n-1, 1 << i, i);
	else
	{
		if (pd > 0)
			sol += solve(n-1, d | (1 << (pd-1)), pd-1);
		if (pd < 9)
			sol += solve(n-1, d | (1 << (pd+1)), pd+1);
	}
	m = sol;
	return m;
}

int main()
{
	LLT sum = 0;
	for( int i=0; i<=40; ++i )
	{
		memset(memo, -1, sizeof(memo));
		sum += solve(i, 0, -1);
	}
	cout << "Answer=" << sum << endl;;
	
	return 0;
}