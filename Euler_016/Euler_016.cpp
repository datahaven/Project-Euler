// Project_016.cpp 
// Adrian Dale 10/01/2009
//
// 2^(15) = 32768 and the sum of its digits is 3 + 2 + 7 + 6 + 8 = 26.
//
// What is the sum of the digits of the number 2^(1000)?

// Answer: 1366

#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

// This works by simulating a large integer as a vector of digits
// We then set that number to 1.
// 2^1000 is the same as doubling that integer 1000 times
int main(int argc, char* argv[])
{
	vector<int> bigNum;

	bigNum.resize(1000);
	fill(bigNum.begin(), bigNum.end(), 0);
	bigNum[0] = 1;

	for( int i=0; i<1000; ++i )
	{
		// Double bigNum by adding it to itself
		int carry = 0;
		
		for( int sumPos=0; sumPos < bigNum.size(); ++sumPos )
		{
			bigNum[sumPos] += bigNum[sumPos] + carry;
			if ( bigNum[sumPos] > 9 )
			{
				carry = bigNum[sumPos] / 10;
				bigNum[sumPos] %= 10;
			}
			else
				carry = 0;
		}

	}

	cout << "Sum of digits=" << accumulate( bigNum.begin(), bigNum.end(), 0 ) << endl;

	return 0;
}

