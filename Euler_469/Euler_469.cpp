// Adrian Dale
// 03/05/2014
// Project Euler 469 - Empty Chairs
//
// Notes and working/test code for this are in files
// Euler_469A.cpp and Euler_469B.cpp which I've kept just to show
// how much effort went into solving this. Something that isn't readily 
// apparent based on this tiny bit of code.

#include <iostream>
#include <iomanip>
#include <map>
using namespace std;

map<int, long double> Memo;

// This version uses a memo for E(n).
// It returns expected number of free chairs, NOT fraction of free chairs.
// It works by recursively sub-dividing the problem into two smaller ones,
// which in turn can be recursively sub-divided, until we reach answers we've
// already seen before. We seed the Memo with our known values for n=1,2,3,4
long double traverse(int N)
{
	auto memoValue = Memo.find(N);
	if (memoValue != Memo.end())
		return memoValue->second;

	long double result = 0.0;
	int A = 1;
	int B = N - 3;
	int childCount = 0;

	// Could possibly speed this up with symmetry trick, but it seems more than
	// fast enough already. This would take advantage of the fact that A and B
	// are symmetrical. eg 1 4, 2 3, 3 2, 1 4
	while (A <= N - 3)
	{
		result += (traverse(A + 1) + traverse(B + 1));
		++A; --B;
		++childCount;
	}

	result = result / static_cast<long double>(childCount);
	Memo[N] = result;
	return result;
}

void solve()
{
	long double prevResult = 0.0;
	Memo.clear();
	Memo[0] = 0;
	Memo[1] = 0;
	Memo[2] = 1;
	Memo[3] = 2;
	Memo[4] = 2;
	// Keep trying successive values of n until the next one doesn't
	// change the result at our desired precision level.
	// This works because the answer converges on a single number as
	// n increases. Hence we don't need to calculate for n==10^18
	for (int n = 5; n <= 10000000; ++n)
	{		
		long double result = traverse(n) / static_cast<long double>(n);
		if (fabs(result - prevResult) < 0.000000000000001)
		{
			cout << "N=" << n << setprecision(14) << " Result=" << (result) << endl;
			break;
		}
		prevResult = result;	
	}
}

int main()
{
	solve();

	return 0;
}
