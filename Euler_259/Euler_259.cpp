// Project Euler 259 - Reachable Numbers
// Adrian Dale 06/05/2014
//
// This takes 52 seconds to run on an AMD Phenom II X4 955 3.2Ghz CPU
// It could be speeded up with some memo-isation of the Reachable function,
// but I didn't bother as it runs within the time limits.
//
// Uses boost to store rational numbers
#include <iostream>
#include <string>
#include <set>
#include <ctime>
#include <boost/rational.hpp>

using namespace std;
using namespace boost;

typedef unsigned long long ULL;
typedef rational<long long> RationalType;

// Return all Reachable rational numbers between lb and ub inclusive.
set<RationalType> Reachable(int lb, int ub)
{
	set<RationalType> results;

	// Base case - range represents a single number
	if (lb == ub)
	{
		results.insert(lb);
		return results;
	}

	// Recursively try combining all possible splits of ub and lb into two.
	for (int k = lb; k < ub; ++k)
	{
		set<RationalType> lower = Reachable(lb, k);
		set<RationalType> upper = Reachable(k + 1, ub);

		// Try each value from the lower range against each value from the
		// upper range
		for (auto itl = lower.begin(); itl != lower.end(); ++itl)
		{
			for (auto itu = upper.begin(); itu != upper.end(); ++itu)
			{
				// The four possible arithmetic combinations of the two values
				results.insert(*itl + *itu);
				results.insert(*itl - *itu);
				results.insert(*itl * *itu);

				// Check for possible division by zero error
				if (*itu != 0)	
					results.insert(*itl / *itu);	
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
	
	results.insert(combined);

	return results;
}

void Solve()
{
	clock_t t;
	t = clock();
	
	// Kick off our set of recursive calls
	set<RationalType> results = Reachable(1, 9);
	ULL sum = 0;
	// Sum up the results
	for (auto rit = results.begin(); rit != results.end(); ++rit)
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