// Project Euler 259 - Reachable Numbers
// Adrian Dale 06/05/2014
//
// This takes 50 seconds to run on an AMD Phenom II X4 955 3.2Ghz CPU
// The memo code only took 2 seconds off the previous run time.
// I suspected too much time was wasted copying set<RationalType>'s around
// but changing to using pointers to elements in the Memo didn't speed things up
// all that much, anyway.
// C++ guarantees that addresses won't change.
//
// I tested a very similar (but non-memo) solution from the euler site and
// it is much faster than mine. I suspect therefore that mine is doing too much
// mucking around with gcd and lcm in the rational library.
// This is the sort of thing a profiler would find.
// 
// Uses boost to store rational numbers
#include <iostream>
#include <string>
#include <set>
#include <ctime>
#include <map>
#include <boost/rational.hpp>

using namespace std;
using namespace boost;

typedef unsigned long long ULL;
typedef rational<long long> RationalType;

map<int, set<RationalType> > Memo;

set<RationalType> *Reachable(int lb, int ub)
{
	int memoKey = lb + 10 * ub;
	auto memoIt = Memo.find(memoKey);
	if (memoIt != Memo.end())
		return &memoIt->second;

	set<RationalType> results;
	Memo[memoKey] = results;

	// Base case - range represents a single number
	if (lb == ub)
	{
		Memo[memoKey].insert(lb);
		return &Memo[memoKey];
	}

	// Recursively try combining all possible splits of ub and lb into two.
	for (int k = lb; k < ub; ++k)
	{
		set<RationalType> *lower = Reachable(lb, k);
		set<RationalType> *upper = Reachable(k + 1, ub);

		// Try each value from the lower range against each value from the
		// upper range
		for (auto itl = lower->begin(); itl != lower->end(); ++itl)
		{
			for (auto itu = upper->begin(); itu != upper->end(); ++itu)
			{
				// The four possible arithmetic combinations of the two values
				Memo[memoKey].insert(*itl + *itu);
				Memo[memoKey].insert(*itl - *itu);
				Memo[memoKey].insert(*itl * *itu);

				// Check for possible division by zero error
				if (*itu != 0)
					Memo[memoKey].insert(*itl / *itu);
			}
		}
	}

	// Add in lb..ub digits
	int combined = 0;
	int mult = 1;
	for (int k = ub; k >= lb; --k)
	{
		combined += k*mult;
		mult *= 10;
	}

	Memo[memoKey].insert(combined);

	return &Memo[memoKey];
}


void Solve()
{
	clock_t t;
	t = clock();
	
	// Kick off our set of recursive calls
	Memo.clear();
	set<RationalType> *results = Reachable(1, 9);
	ULL sum = 0;
	// Sum up the results
	for (auto rit = results->begin(); rit != results->end(); ++rit)
	{
		// Throw away negative numbers	
		if (*rit < 1)
			continue;
		// Throw away non-integer numbers
		if (rit->denominator() != 1)
			continue;
		
		sum += rit->numerator();
	}
	cout << "Answer=" << sum << endl;
		
	t = clock() - t;

	cout << "Time taken: " << t / CLOCKS_PER_SEC << " seconds" << endl;
}

int main()
{
	Solve();
	return 0;
}