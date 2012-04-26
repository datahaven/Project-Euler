// Euler 39
//
// Adrian Dale 30/08/2010
//
//
// I'm sure this could be done more neatly using STL.
// Maybe even a little parallelisation? But it's already pretty quick, running in
// just a couple of seconds.
// Was hoping to learn something from the answer that would apply to my other
// problems re circles and sum of squares.
#include <iostream>

using namespace std;

#define P 1000

int main()
{
	cout << "Euler 39 - Adrian Dale" << endl;

	int aiResults[P];
	
	for( int i=0; i<P; ++i )
		aiResults[i] = 0;

	for( int a=1; a < P; ++a)
		for( int b=a; b < P; ++b)
			for( int c=b+1; c < P; ++c)
			{
				int ab_2 = a*a + b*b;
				int c_2 = c*c;
				if ( ab_2 == c_2 )
				{
					int ip = a + b + c;
					if (ip < 1000)
						++aiResults[ip];

					if ( ip == 120 )
						cout << "a,b,c=" << a << " " << b << " " << c << endl;
				}
			}
	
	int maxP = 0;
	int maxpPos = -1;
	for( int i=0; i<P; ++i )
	{
		if ( aiResults[i] > maxP )
		{
			maxP = aiResults[i];
			maxpPos = i;
		}
	}

	cout << "p[120]=" << aiResults[120] << endl;

	cout << "DONE " << maxP << " " << maxpPos << endl;

	return 0;
}