// Project Euler 114
// http://projecteuler.net/index.php?section=problems&id=114
// Filling row with blocks
//
// Adrian Dale 28/09/2010 14:00 - 15:45pm
//
// Runs in microseconds, for both puzzle 114 and 115
// Messed about for way too long fixing bugs/logic errors.
#include <iostream>
#include <algorithm>
#include <vector>

#include <Windows.h> // For GetTickCount
using namespace std;

typedef long long LLT;

typedef vector<LLT> LLVT;

LLVT memo(1000,-1);

template <class Function>
__int64 time_call(Function&& f)
{
   __int64 begin = GetTickCount();
   f();
   return GetTickCount() - begin;
}

// How many ways to fill row of size n
// with bricks size m or more, with at least one
// space between.
// Needs to be called initially with n one bigger than the
// required row size to allow for the first space.
LLT fillRow(LLT n, LLT m)
{
	LLT &mmo = memo[n];
	if ( mmo != -1 )
		return mmo;

	LLT res = 1; // Count the solution with no bricks.
	// Block size can be from size m to the size of the
	// remaining space
	--n; // Include the space
	for(int i=m;i<=n;++i)
		for(int j=n-i; j>=0; --j)
				res += fillRow(j,m);
	mmo = res;
	return mmo;
}

void run114()
{
	LLT ans = fillRow(51,3);
	cout << "Answer=" << ans << endl;
}

void run115()
{
	LLT i=50;
	
	// Must clear out our memo field if we want to call our fn with a
	// different value of m.
	// No need to clear it if only n changes, though - makes this fn
	// unmeasurably fast!
	for(LLVT::iterator it = memo.begin(); it != memo.end(); ++it )
		*it = -1;
	do
		++i;
	while(fillRow(i,50) < 1000000 );

	cout << "Answer=" << i-1 << endl;
}

int main()
{
	__int64 itTime = time_call( [&]{
		run114();
	});

	cout << "Euler 114 took " << itTime << " ms" << endl;

	itTime = time_call( [&]{
		run115();
	});

	cout << "Euler 115 took " << itTime << " ms" << endl;

	return 0;
}