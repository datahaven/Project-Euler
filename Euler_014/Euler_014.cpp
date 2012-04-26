// Project_014.cpp
// Adrian Dale 10/01/2009
/*
The following iterative sequence is defined for the set of positive integers:

n => n/2 (n is even)
n => 3n + 1 (n is odd)

Using the rule above and starting with 13, we generate the following sequence:
13 => 40 => 20 => 10 => 5 => 16 => 8 => 4 => 2 => 1

It can be seen that this sequence (starting at 13 and finishing at 1) contains 10 terms.
Although it has not been proved yet (Collatz Problem), it is thought that all starting numbers finish at 1.

Which starting number, under one million, produces the longest chain?

NOTE: Once the chain starts the terms are allowed to go above one million.
*/

#include <iostream>
#include <map>

using namespace std;

map<__int64, int> solutionCache;

// Work out how many steps it takes from this number
// This appears to work fine up to about 110000 at which
// point we get a stack overflow.
// So, back to the drawing board ...
// Turns out it is because int can't hold a big enough
// number. __int64 is big enough but simple solution
// works much faster, anyway.
int stepCount(__int64 n)
{
	// cout << "stepCount(" << n << ")" << endl;

	// Do our obvious terminal case first
	if ( n==1 )
		return 1;

	// See if we already have a value for n
	map<__int64, int>::iterator it = solutionCache.find(n);
	if ( it != solutionCache.end() )
		return it->second;
	
	// No, we don't, so calculate it
	int steps;
	if ( n % 2 == 0 )
		steps = stepCount(n/2) + 1;
	else
		steps = stepCount(n*3 + 1) + 1;

	// Store the result for next time
	solutionCache[n] = steps;

	return steps;
}

// I bet this is slow!
// WRONG - It wins easily!
int newStepCount(int n)
{
	int steps = 1;

	if ( n == 1 )
		return steps;

	__int64 next_n = n;

	do {
		if ( next_n % 2 == 0 )
			next_n = next_n/2;
		else
			next_n = next_n*3 + 1;
		steps++;
		//cout << next_n << " ";
	} while ( next_n != 1 );

	return steps;
}

int main(int argc, char* argv[])
{
	try
	{

		int biggestCount = 0;
		int biggestI = 0;
		for( int i=2; i<1000000; ++i )
		{
			
				int count = newStepCount(i);
				if ( count > biggestCount)
				{
					biggestCount = count;
					biggestI = i;
				}
			

			if ( i%100000 == 0)
				cout << "i=" << i << endl;
		}

		cout << "biggestCount=" << biggestCount << " biggestI=" << biggestI << endl;

	}
	catch(char * str)
	{
		cout << "Oh, dear! " << str << endl;
	};

	return 0;
}
