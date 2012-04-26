// Project Euler 166
// http://projecteuler.net/index.php?section=problems&id=166
// Number of "magic squares" with digits 0-9
// Adrian Dale 29/10/2010
//
// Rather crummy code but it does the job.
// A bit slow (19 seconds) and could potentially be halved by
// only counting half of the solutions and then doubling the answer where
// the solution is symmetrical. But fits within the one minute rule.
// Sounds like there are also some clever methods where one sorts out the
// top half and bottom half separately.
// Not to mention a way to remove another one of the loops that I've got.
#include <iostream>
#include <Windows.h>
using namespace std;

template <class Function>
__int64 time_call(Function&& f)
{
   __int64 begin = GetTickCount();
   f();
   return GetTickCount() - begin;
}

#define REP(a) for( int (a)=0; (a)<=9; ++(a) )
typedef long long LLT;

void run(void)
{
	LLT count = 0;

	REP(a1)
	REP(a2)
	REP(a3)
	REP(a4)
	{
		int topRowSum = a1+a2+a3+a4;
		REP(b1)
		REP(b2)
		REP(b3)
		{
			int b4 = topRowSum - b1 - b2 - b3;
			if ( b4 < 0 || b4 > 9)
				continue;
			REP(c1)
			REP(c2)
			REP(c3)
			{
				int c4 = topRowSum - c1 - c2 - c3;
				if ( c4 < 0 || c4 > 9)
					continue;

				int d1 = topRowSum - a1 - b1 - c1;
				int d2 = topRowSum - a2 - b2 - c2;
				int d3 = topRowSum - a3 - b3 - c3;
				int d4 = topRowSum - a4 - b4 - c4;
				if ( d1 < 0 || d2 < 0 || d3 < 0 || d4 < 0 )
					continue;
				if ( d1 > 9 || d2 > 9 || d3 > 9 || d4 > 9 )
					continue;
				if ( a1 + b2 + c3 + d4 != topRowSum )
					continue;
				if ( a4 + b3 + c2 + d1 != topRowSum )
					continue;
				++count;
			}
		}
	}
	cout << "Answer=" << count << endl;
}

int main()
{
	__int64 runtime = time_call( [&]{run();} );
	cout << "Took " << runtime << " ms" << endl;
	return 0;
}