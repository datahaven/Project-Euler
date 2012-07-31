// Project Euler 86
// Exploring the shortest path from one corner of a cuboid to another
// Adrian Dale 31/07/2012
//
// 1818
//
// Congratulations, the answer you gave to problem 86 is correct.
//
// You are the 4361st person to have solved this problem.

#include <iostream>
#include <cmath>
using namespace std;

typedef unsigned long long ull;

ull countSolutions(ull M)
{
	ull solutionCount = 0;
	ull cubeCount = 0;

	for(ull x=1; x<=M; ++x)
	{
		for(ull y=x; y<=M; ++y)
		{
			for(ull z=y; z<=M; ++z)
			{
				++cubeCount;
				ull a = x*x + (y+z)*(y+z);
				ull b = y*y + (x+z)*(x+z);
				ull c = z*z + (y+x)*(y+x);
				ull shortestPathSq = min( min(a,b), c);
				ull intsq = static_cast<ull>(sqrt( static_cast<long double>(shortestPathSq)));
				if (intsq*intsq == shortestPathSq)
					++solutionCount;
			}
		}
	}

	return solutionCount;
}

/*
cs(99)=1975
cs(100)=2060
cs(1800)=986995
cs(1810)=994302
cs(1820)=1006727

cs(99)=1975
cs(100)=2060
cs(1811)=994302
cs(1812)=995963
cs(1813)=996221
cs(1814)=996221
cs(1815)=998665
cs(1816)=999460
cs(1817)=999850
cs(1818)=1000457
cs(1819)=1000457


Lazy cheating way - recompile to try out different M's until I get the right one.
Should really write code to do some sort of binary search.
Also right now code is too slow, although quick enough to get the answer eventually.
Problem could be in sqrt and casting being slow?

Smart people used pythagorean triples to solve this. I still like Brute Force, though!
Much quicker to program.
*/

int main()
{
	cout << "cs(99)=" << countSolutions(99) << endl;
	cout << "cs(100)=" << countSolutions(100) << endl;
	for(int m=1811; m<1820; m+=1)
		cout << "cs(" << m << ")=" << countSolutions(m) << endl;
}