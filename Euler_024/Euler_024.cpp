// Project_024.cpp
// Adrian Dale 10/01/2009
/*
A permutation is an ordered arrangement of objects.
For example, 3124 is one possible permutation of the digits 1, 2, 3 and 4.
If all of the permutations are listed numerically or alphabetically, we call it lexicographic order.
The lexicographic permutations of 0, 1 and 2 are:

012   021   102   120   201   210

What is the millionth lexicographic permutation of the digits 0, 1, 2, 3, 4, 5, 6, 7, 8 and 9?
*/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main(int argc, char* argv[])
{
	vector<int> digits;
	digits.resize(10);
	for( int i=0; i<10; ++i )
		digits[i] = i;

	// The handy quick (ie lazy) C++ way.
	for( int i=1; i<1000000; ++i )
		next_permutation( digits.begin(), digits.end() );

	// There may even be a property of the algorithm that lets us go
	// straight to the answer? Doing repeated next_permutation() calls
	// isn't all that fast.
	// I also screwed up the count the first time. We already have the 
	// first perm, so should only call next_permutation 999999 times to
	// get the millionth.

	for( int i=0; i<10; ++i )
		cout << digits[i];
	return 0;
}

