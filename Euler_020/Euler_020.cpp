// Project_020.cpp 
// Adrian Dale 10/01/2009
//
// n! means n × (n − 1) × ... × 3 × 2 × 1
//
// Find the sum of the digits in the number 100!

// Answer: 648

#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

// This is the same sort of thing as problem 16
// So a nice bit of code reuse. Should probably work out how to
// do big number multiplication but multiple addition works fine here.

// This works by simulating a large integer as a vector of digits
// We then set that number to 1.
// We could write code for multiplication, but addition works as well.
// 100! is 99! x 100, or 99! + 99! + .... + 99!, 100 times
// And we already have 99! from previous loop
int main(int argc, char* argv[])
{

	const int fact = 100;

	vector<int> prevTotal;

	const int maxDigits = 1000;

	prevTotal.resize(maxDigits);
	fill(prevTotal.begin(), prevTotal.end(), 0);

	vector<int> runningTotal;
	runningTotal.resize(maxDigits);
	fill(runningTotal.begin(), runningTotal.end(), 0);
	runningTotal[0] = 1;
	

	for( int i=1; i<=fact; ++i )
	{
		// Add previous result to running total i - 1 times.
		// (i-1 since running total already contains the first previous result)
		for( int j=i-1; j>0; --j )
		{
			int carry = 0;
			
			for( int sumPos=0; sumPos < prevTotal.size(); ++sumPos )
			{
				runningTotal[sumPos] += prevTotal[sumPos] + carry;
				if ( runningTotal[sumPos] > 9 )
				{
					carry = runningTotal[sumPos] / 10;
					runningTotal[sumPos] %= 10;
				}
				else
					carry = 0;
			}
		}

		prevTotal = runningTotal;
	}

	for( vector<int>::reverse_iterator it = runningTotal.rbegin(); it != runningTotal.rend(); ++it )
		cout << *it;
	cout << endl;

	cout << "Sum of digits=" << accumulate( runningTotal.begin(), runningTotal.end(), 0 ) << endl;

	return 0;
}

