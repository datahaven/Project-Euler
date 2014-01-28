// Project Euler 265 - Binary Circles
// http://projecteuler.net/problem=265
// Adrian Dale 28/10/2014
/*
Notes:
Looks suspiciously easy! A disproportionate number of people
have solved it compared to other nearby puzzles.
I think it can probably be solved by Brute Force.
(Which is what computers do best!)
Or this may be famous last words - there are still 2^27
different 32 bit numbers that start with 00000,
although there are only 2^5==32 subsequences.
Num must start 000001 - 67million of these

Could end up a little slow for brute force, but doesn't sound
completely out of range. End result is not totally unacceptable at
104 seconds.

Faster solutions use recursion, whereas I've gone for
the very basic "check everything" approach, without having to
think about the problem too much. There's also an interesting idea
where one checks all permutations that start with 000001 and have
16 1's and 16 0's. There are only 7million of these.

Trade off is that my code took less than an hour to develop. :-)

Congratulations, the answer you gave to problem 265 is correct.

You are the 2427th person to have solved this problem.

Return to Problems page.

*/
#include <iostream>
#include <set>
#include <bitset>
#include <ctime>
using namespace std;

typedef unsigned long long LL;

// Cached value to save recalculating
int twoPowN;

// Calculate 2^n - don't really need this but it's a bit
// nicer than hardcoding values, or using floats.
int twoPow(int n)
{
	int result = 1;
	for (int i = 1; i <= n; ++i)
	{
		result *= 2;
	}
	return result;
}

// Return the n digit subsequence starting at position p
// in test value tv
inline int getSubSeq(int tv, int n, int p)
{
	int shiftValue = twoPowN - n - p;
	if (shiftValue > 0)
		tv >>= shiftValue;
	else
		tv <<= -1 * shiftValue;
	tv &= twoPowN - 1;
	return tv;
}

inline bool isValidArrangement(int tv, int n)
{
	set<int> subseqs;
	for (int i = 0; i < twoPowN; ++i)
	{
		int ssval = getSubSeq(tv, n, i);
		if (subseqs.find(ssval) != subseqs.end())
			return false;
		subseqs.insert(ssval);
	}
	return true;
}

LL solve(int n)
{
	// Cache this for speed
	twoPowN = twoPow(n);

	int headValue = 1 << (twoPow(n) - n - 1);

	// Loop through all possible binary digit combinations
	// that can be tagged on after 0001
	LL uniqueSum = 0;
	for (int i = 0; i < headValue; ++i)
	{
		int testValue = headValue | i;

		if (isValidArrangement(testValue, n))
		{
			uniqueSum += testValue;
		}
	}
	return uniqueSum;
}

int main()
{
	cout << "Euler 265 - Binary Circles" << endl;

	time_t startTime;
	time(&startTime);

	cout << "Answer=" << solve(5) << endl;

	time_t endTime;
	time(&endTime);

	cout << "Processing Time = " << difftime(endTime, startTime) << " seconds" << endl;

	return 0;
}