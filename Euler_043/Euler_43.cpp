// Euler Project 43
// http://projecteuler.net/index.php?section=problems&id=43
//
// Adrian Dale 30/08/2010
/*
The number, 1406357289, is a 0 to 9 pandigital number because it is made up of 
each of the digits 0 to 9 in some order, 
but it also has a rather interesting sub-string divisibility property.

Let d_(1) be the 1^(st) digit, d_(2) be the 2^(nd) digit, and so on. 
In this way, we note the following:

    * d_(2)d_(3)d_(4)=406 is divisible by 2
    * d_(3)d_(4)d_(5)=063 is divisible by 3
    * d_(4)d_(5)d_(6)=635 is divisible by 5
    * d_(5)d_(6)d_(7)=357 is divisible by 7
    * d_(6)d_(7)d_(8)=572 is divisible by 11
    * d_(7)d_(8)d_(9)=728 is divisible by 13
    * d_(8)d_(9)d_(10)=289 is divisible by 17

Find the sum of all 0 to 9 pandigital numbers with this property.
*/

/* This works, but could be tidied up a little. I'd like to get into using STL properly,
   including the new lambda syntax. It also could have the repeated divTest calls moved
   to an array.
   Also we can use an array instead of a vector<int>.
   We don't need the solution count - although we're supposed to sum the answers, I
   just printed them and used Calculator to do them manually. solutionCount told me that
   this would be feasible.

1406357289
1430952867
1460357289
4106357289
4130952867
4160357289
 = 16695334890
*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> permDigits;

bool divTest(int d, int dv)
{
	int TestNo = permDigits[d-1] * 100 + permDigits[d] * 10 + permDigits[d+1];
	return TestNo % dv == 0;
}

int main()
{
	for(int i=0;i<10;++i)
		permDigits.push_back(i);

	int solutionCount = 0;
	
	do
	{
		if ( divTest(2,2) && divTest(3,3) && divTest(4,5) && divTest(5,7) &&
			 divTest(6,11) && divTest(7,13) && divTest(8,17) )
		{
			for( vector<int>::iterator it = permDigits.begin(); it < permDigits.end(); ++it )
			{
				cout << *it;
			}
			cout << endl;
			solutionCount++;
		}
	}
	while ( next_permutation(permDigits.begin(), permDigits.end()) );
	
	cout << "Found " << solutionCount << " solutions" << endl;

	return 0;
}
