// Project_005.cpp 
// Adrian Dale 09/01/2009
//
// 2520 is the smallest number that can be divided by each of the numbers from 1 to 10 without any remainder.
//
// What is the smallest number that is evenly divisible by all of the numbers from 1 to 20?
//
// Answer=232792560

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	int i=20;
	bool found = false;
	while( !found )
	{
		// Since no. must be even as it is divisible by 2
		i += 2;

		// (Turns out I could actually add 20. Doh!)

		// Or even better (not by me!):
		/*
		This does not require programming at all. 
		Compute the prime factorization of each number from 1 to 20, 
		and multiply the greatest power of each prime together:

20 = 2^2 * 5
19 = 19
18 = 2 * 3^2
17 = 17
16 = 2^4
15 = 3 * 5
14 = 2 * 7
13 = 13
11 = 11

All others are included in the previous numbers.

ANSWER: 2^4 * 3^2 * 5 * 7 * 11 * 13 * 17 * 19 = 232 792 560
		*/


		found = true;
		int j=20;
		while ( j > 2 )
		{
			if ( i % j != 0 )
			{
				found = false;
				break;
			}
			--j;
		}		
	}

	cout << "Answer=" << i << endl;

	return 0;
}