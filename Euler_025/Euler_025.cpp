// Project_025.cpp 
// Adrian Dale 10/01/2009
//
// What is the first term in the Fibonacci sequence to contain 1000 digits?
// ...
// Answer: 4782nd

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// Using code from Project 020 to add big ints together.
// Hopefully that'll make this a lot simpler


int main(int argc, char* argv[])
{
	const int maxDigits = 1000;

	vector<int> prevTotal1;
	prevTotal1.resize(maxDigits);
	fill(prevTotal1.begin(), prevTotal1.end(), 0);
	prevTotal1[0] = 1;

	vector<int> prevTotal2;
	prevTotal2.resize(maxDigits);
	fill(prevTotal2.begin(), prevTotal2.end(), 0);
	prevTotal2[0] = 1;

	vector<int> newFib;
	newFib.resize(maxDigits);
	fill(newFib.begin(), newFib.end(), 0);
	
	for( int fibCount = 3; ; ++fibCount )
	{
		// Add previous results together to get new result
		int carry = 0;
		for( int sumPos=0; sumPos < newFib.size(); ++sumPos )
		{
			newFib[sumPos] = prevTotal1[sumPos] + prevTotal2[sumPos] + carry;
			if ( newFib[sumPos] > 9 )
			{
				carry = newFib[sumPos] / 10;
				newFib[sumPos] %= 10;
			}
			else
				carry = 0;

			// Is this the first number to carry over to the 1000th digit?
			if ( carry != 0 && sumPos == 998 )
			{
				cout << "FINAL fibCount=" << fibCount << endl;
				return 0;
			}
		}

		prevTotal1 = prevTotal2;
		prevTotal2 = newFib;

		if ( fibCount % 1000 == 0 )
			cout << "fibCount=" << fibCount << endl;
	}

	return 0;
}

