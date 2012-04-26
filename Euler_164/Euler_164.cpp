// Project Euler 164
// How many 20 digit numbers n (without any leading zero) 
// exist such that no three consecutive digits of n have 
// a sum greater than 9?
//
// Adrian Dale 30/09/2010
//
// Takes microseconds to come up with 378158756814587
// which is the correct answer. Took ten or fifteen minutes to write
// Quite slow to run without the memo table, though.
// I'm sure there are other clever solutions possible if you come up
// with a state machine, or recurrence relationship.
// There's an awesome pair of solutions that run at compile time using
// either macros or templates.
#include <iostream>
#include <Windows.h>
using namespace std;
typedef long long LLT;

LLT memo[30][10][10];

LLT solve(int n, int p1, int p2)
{
	LLT &m = memo[n][p1][p2];
	if ( m != 0 )
		return m;

	LLT sol = 0;
	
	if ( n == 1 )
	{
		m = max(9-p1-p2, 0);
		return m;
	}
	for(int i=0;i<=9;++i)
		if ( p1+p2+i < 10 )
			sol += solve(n-1,p2, i);
	m = sol;
	return m;
}

// From Microsoft example code:
// Calls the provided work function and returns the number of milliseconds 
// that it takes to call that function.
template <class Function>
__int64 time_call(Function&& f)
{
   __int64 begin = GetTickCount();
   f();
   return GetTickCount() - begin;
}

void run(int n)
{
	memset(memo, 0, sizeof(memo));
	cout << "Answer(" << n << ")=" << solve(n, 0, 0);
}

int main()
{
	__int64 runtime;
	for(int i=1; i<=20; ++i )
	{
		runtime = time_call( [&]{ run(i); });
		cout << " in " << runtime << " ms" << endl;
	}
	return 0;
}