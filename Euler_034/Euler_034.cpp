// Project_034.cpp
// Adrian Dale 10/1/2009
//
/*
145 is a curious number, as 1! + 4! + 5! = 1 + 24 + 120 = 145.

Find the sum of all numbers which are equal to the sum of the factorial of their digits.

Note: as 1! = 1 and 2! = 2 are not sums they are not included.
*/
// Answer: 40730 - note that the only other number with this property is 40585, but the
// puzzle asked for the sum of all answers, so we add 145, too, which this code
// also finds.

#include <iostream>

using namespace std;

bool isCurious( __int64 n )
{
	// Save bothering to work these out every time
	int factorial[] = { 1,1,2,6,24,120,720,5040,40320,362880 };

	// Keep dividing the number by 10 and taking the remainder
	// to break it down digit by digit.
	// Doesn't matter that it comes out in the "wrong" order as
	// we're interested in the sum of the factorials of the digits.
	__int64 divisor = n;
	__int64 digFacSum = 0;
	do
	{
		__int64 remainder = divisor % 10;
		digFacSum += factorial[remainder];
	} while ( (divisor = divisor / 10) > 0 );

	return digFacSum == n;
}

int main()
{
	// I'm guessing an upper bound here, but
	// really I ought to work it out.
	for( __int64 i=10; i < 100000000; ++i )
		if (isCurious(i))
			cout << i << " has the property" << endl;

	return 0;
}
