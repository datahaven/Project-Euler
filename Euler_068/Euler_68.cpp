// Project Euler 68
// Magic 5-gon Rings
//
// Adrian Dale
// 07/10/2011
//
// 6531031914842725
// You are the 5521st person to have solved this problem.
#include <algorithm>
#include <iostream>
using namespace std;

// cd is short for convertDigit
inline int cd(int d)
{
	return d==0 ? 10 : d;
}

// How the array is laid out:
//    "6"
//      \
//       0    2
//      /  \ /
//     7    1
//    /|    |
//   8 5----3----4
//     |
//     6
//
// Trys all permutations of digits 0-9 (not inc 6, which is fixed)
// We know answer starts with 6 as it is the largest possible smallest
// outside digit
void Solve()
{
	int p[] = {9,8,7,5,4,3,2,1,0};
	
	do
	{
		// 10 (ie our 0) can't be on an inside node,
		// otherwise string would be 17 chars, not 16.
		if ( p[0]==0 || p[1]==0 || p[3]==0 || p[5]==0 || p[7]==0 )
			continue;

		// Outside nodes must not be less than 6
		if (cd(p[2]) < 6 || cd(p[4]) < 6 || cd(p[6]) < 6 || cd(p[8]) < 6 )
			continue;

		int x = 6 + cd(p[0]) + cd(p[1]);
		if ( cd(p[2])+cd(p[1])+cd(p[3]) != x )
			continue;
		if ( cd(p[4])+cd(p[3])+cd(p[5]) != x )
			continue;
		if ( cd(p[6])+cd(p[5])+cd(p[7]) != x )
			continue;
		if ( cd(p[8])+cd(p[7])+cd(p[0]) != x )
			continue;
		
		cout << "Solution Found - sum=" << x << ": ";
		cout << "6" << cd(p[0]) << cd(p[1]);
		cout << cd(p[2]) <<	cd(p[1]) <<	cd(p[3]);
		cout << cd(p[4]) <<	cd(p[3]) << cd(p[5]);
		cout << cd(p[6]) <<	cd(p[5]) << cd(p[7]);
		cout << cd(p[8]) << cd(p[7]) << cd(p[0]);
		cout << endl;
		// Can either finish here, or continue loop to see if there are any
		// more solutions. First one found will be the largest. That's why
		// I'm searching in reverse order
	}
	while( prev_permutation( &p[0], &p[9] ) == true );
}

int main()
{
	Solve();

	return 0;
}