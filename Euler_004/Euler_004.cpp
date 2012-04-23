// Project_004.cpp 
//
// Adrian Dale 09/01/2009
//
// A palindromic number reads the same both ways.
// The largest palindrome made from the product of
// two 2-digit numbers is 9009 = 91 × 99.
//
// Find the largest palindrome made from the product of two 3-digit numbers.
//
// Answer: largest=906609

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// This is a rough solution. I'm surprised how slow it goes.
// Takes a few seconds to run - maybe all the string messing
// is actually pretty slow?
//
// There must be some mathematical way to narrow the search, too.
// Also, perhaps a better way to order the loops - the answer's not
// likely to be xxx x 100, say.
//
// And maybe a better way to check if the no. is palindromic?
// a mathematical solution?
int main(int argc, char* argv[])
{
	int largest = 0;
	for( int a=999; a > 99; --a )
		for( int b=a; b > 99; --b )
		{
			int product = a * b;

			ostringstream ostr;
			ostr << product;

			string prodStr = ostr.str();

			int checkEnd = prodStr.length() - 1;
			int checkStart = 0;
			bool isPalindrome = true;
			// Done like this so we can handle 5 or 6 digit numbers.
			// Answer is likely to be 6 digits, though.
			// Maybe hardcoding would be faster?
			while ( checkEnd > checkStart )
			{
				if ( prodStr[checkStart] != prodStr[checkEnd] )
				{
					isPalindrome = false;
					break;
				}
				checkEnd--;
				checkStart++;
			}

			if ( isPalindrome && product > largest )
				largest = product;
		}

	cout << "largest=" << largest << endl;

	return 0;
}
