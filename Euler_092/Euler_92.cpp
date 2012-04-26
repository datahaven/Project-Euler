// Project Euler 92
// http://projecteuler.net/index.php?section=problems&id=92
// Square Digit Number Chains
//
// Adrian Dale 28/09/2010
//
// Just over twenty minutes to code, test and get the final answer out.
// Minor improvements are possible - like only using a memo of size 600.
// (Since max digit sum would be for 9999999, which is 576.) This would
// require an extra check at the start of checkSoD
// ie
// if (i > 576)
//   return checkSoD(sumSqD(i));
// I've added this in but it wasn't entirely my idea.
//
// Recursive fn was quick to write but I suspect iterative would be 
// faster to execute:
// Answer=8581146
// Iterative version took 8595 ms
// Answer=8581146
// Recursive version took 9938 ms
// or, more sensibly, using Release build, rather than Debug
// Iterative version took 812 ms
// Recursive version took 842 ms
#include <iostream>
#include <vector>
#include <Windows.h> // For GetTickCount
using namespace std;

typedef vector<int> VIT;

VIT memo(600, 0);

// Get the sum of the squares of the digits of i
int sumSqD(int i)
{
	int nsos = 0;
	while(i)
	{
		int nd = i % 10;
		nsos += nd * nd;
		i /= 10;
	}
	return nsos;
}

// Returns true if digit chain ends with 89, false otherwise
int checkSoD(int i)
{
	if (i>576)
		return checkSoD(sumSqD(i));

	int &m = memo[i];
	if ( m == 1 || m == 89 )
		return m;

	// We haven't seen this i before, so process it now
	m = sumSqD(i);
	return checkSoD(m);
}

// Returns true if digit chain ends with 89, false otherwise
// The iterative version of the above function.
// Roughly similar execution time.
int checkSoDI(int i)
{
	if (i>576)
		i = sumSqD(i);

	do
	{
		int &m = memo[i];
		if ( m == 1 || m == 89 )
			return m;
		m = sumSqD(i);
		i = m;
	}
	while( i != 1 && i != 89 );
	return i;
}

template <class Function>
__int64 time_call(Function&& f)
{
   __int64 begin = GetTickCount();
   f();
   return GetTickCount() - begin;
}

int runIterative()
{
	int count = 0;
	for( int i=1; i<10000000; ++i )
		if (checkSoDI(i) == 89)
			++count;
	cout << "Answer=" << count << endl;
	return 0;
}

int runRecursive()
{
	int count = 0;
	for( int i=1; i<10000000; ++i )
		if (checkSoD(i) == 89)
			++count;
	cout << "Answer=" << count << endl;
	return 0;
}

int main()
{
	__int64 itTime = time_call( [&]{
		runIterative();
	});

	cout << "Iterative version took " << itTime << " ms" << endl;

	__int64 recTime = time_call( [&]{
		runRecursive();
	});

	cout << "Recursive version took " << recTime << " ms" << endl;

	return 0;
}