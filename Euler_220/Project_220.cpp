// Project Euler 220
// Heighway Dragon
// Adrian Dale
// 16/6/2011
//
// You are the 1001st person to have solved this problem.
// (139776,963904) in 3810.29 seconds (1hr, 3min, 30seconds)
//
// Brute force code which walks through the sequence from 1 to
// 10^12, using optimised C++ code.
//
// Could possibly be slightly faster if I used a single array to
// eliminate the if statement. ie facing[l/r][facing]
//
// Wonder if using global vars would be faster?
//
// Also, what if x and y were combined into a single int and then
// only one lookup into the displacement table would be needed.
//
// There is, of course, a much quicker answer possible that does
// not involve brute force but I just wanted to try this out.
// Quick to write - computer does all the work. That's what it is for!
#include <iostream>
using namespace std;

int main()
{
	int x = 0;
	int y = 0;
	int facing = 0;

	// Array is extended out so we don't have to
	// calculate the new direction mod 4
	int newfacingL[] = {3,0,1,2,3,0,1,2};
	int newfacingR[] = {1,2,3,0,1,2,3,0};
	int newxf[] = {0,1,0,-1};
	int newyf[] = {1,0,-1,0};

	for( long long n=1; n<=1000000000000; ++n )
	{
		// true for L, false for R
		if ((((n & (-1*n)) << 1) & n) != 0)
			facing = newfacingL[facing];	
		else
			facing = newfacingR[facing];

		x += newxf[facing];
		y += newyf[facing];
	}
	cout << "(" << x << "," << y << ")" << endl;
	return 0;	
}
