// Project Euler 139
// http://projecteuler.net/problem=139
//
// Adrian Dale 23/01/2014

/*
Congratulations, the answer you gave to problem 139 is correct.

You are the 2854th person to have solved this problem.

Not too tricky, given that I'd already solved Problem 75 which is
very similar.

The current code runs in plenty of time but is much slower than it
could be. This is due to me mis-reading the problem (not concentrating - doh!)
and thinking triangles could have a single side length up to 100,000,000
rather than the perimeter. Hence I generate a lot more triangles than I need to.

This is also why the perimeter check is in isTileable.

Wouldn't be too hard to go back and sort that out.
*/
#include <iostream>
#include <algorithm>
using namespace std;
typedef unsigned long long ull;


ull gcd(ull a, ull b)
{
	while (1)
	{
		a = a % b;
		if (a == 0)
			return b;
		b = b % a;

		if (b == 0)
			return a;
	}
}

inline void swap(ull &x, ull &y)
{
	ull t = x;
	x = y;
	y = t;
}

inline bool isTileable(ull a, ull b, ull c)
{
	// Sort a, b, c smallest to largest
	if (a > b) swap(a, b);
	if (b > c) swap(b, c);
	if (a > b) swap(a, b);

	return c % (b - a) == 0;
}

void countTriangles()
{
	ull count = 0;
	ull tileableCount = 0;
	
	// n and m can actually be a maximum of 7070 and 4999 respectively
	// and still meet the conditions.
	// (I found this out by putting in code to see what their max
	// values were, which is a bit cheaty, really!)
	for (ull n = 1; n < 10000; ++n) // sqrt of 100000000
	{
		for (ull m = n + 1; m < 10000; ++m)
		{
			if ((m - n) % 2 == 0 || gcd(m, n) != 1)
				continue;
			
			for (ull k = 1; true; ++k)
			{
				ull a = (m*m - n*n)*k;
				ull b = 2 * m*n*k;
				ull c = (m*m + n*n)*k;
				if (a + b + c < 100000000)
				{
					if (isTileable(a, b, c))
					{
						++tileableCount;
					}
					++count;
				}
				else
					break;
			}
		}
	}
	
	cout << "Total Pythagoras Triples=" << count << endl;
	cout << "Answer=" << tileableCount << endl;
}

int main()
{
	cout << "Project Euler 139 - Pythagorean Tiles" << endl;
	countTriangles();
	return 0;
}