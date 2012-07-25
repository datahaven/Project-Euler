// Project Euler 301 - Nim
// Adrian Dale 25/07/2012
//
// Info from http://www.cs.umd.edu/~gordon/ysp/nim.pdf
//
// 2178309 in well under a minute
//
// Congratulations, the answer you gave to problem 301 is correct.
//
// You are the 1991st person to have solved this problem.
//
#include <iostream>
using namespace std;

const unsigned int N = 0x40000000; // 2^30

void test01()
{
	unsigned int count = 0;

	for( unsigned int n=1; n<=N; ++n )
	{
		if ( (n ^ (2*n) ^ (3*n)) == 0 )
			++count;
	}
	cout << "count=" << count << endl;
}

int main()
{
	test01();
}