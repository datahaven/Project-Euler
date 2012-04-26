// Project Euler 33
//
// Adrian Dale 10/09/2010
//
// Produces 16/64, 26/65, 19/95, 49/98 and leaves it up to the reader to 
// produce the final answer for the puzzle.
// Fractions are 1/4, 2/5, 1/5, 4/8 which multiply to 8/800, or
// the expected answer, which is 1/100

#if 0
olegyk's J version:

x1=: <.@(%&10)@[
x2=: 10&|@[
y1=: <.@(%&10)@]
y2=: 10&|@]
 
t1=: (% = x1 % y2)*.(x2 = y1)
t2=: (% = x2 % y1)*.(x1 = y2)
 
   {:2 x:*/%/"1 (#~ (t1 +. t2)/"1) (</ #&(,/) ,"0/)~ 10+i.90
100
#endif

#include <iostream>
using namespace std;

// Special fn to save a bit of typing, which allows for when the divisor is 0
double doDiv(int n1, int n2)
{
	if (n2 == 0)
		return -1.0;
	return static_cast<double>(n1)/static_cast<double>(n2);
}

int main()
{
	for(int d=10; d<100; ++d)
	{
		for(int n=10; n<d; ++n)
		{
			int nd1 = n / 10;
			int nd2 = n % 10;
			int dd1 = d / 10;
			int dd2 = d % 10;

			// No need to consider cases where both digits of 
			// numerator or denominator would "cancel out"
			if ( nd1 == nd2 || dd1 == dd2 )
				continue;

			// No need to consider trivial cases
			if ( nd2 == 0 && dd2 == 0)
				continue;

			double reducedFraction = static_cast<double>(n) / static_cast<double>(d);

			// Can we cancel first digit of numerator?
			double cancelledFraction = 0.0;
			if ( nd1 == dd1 )
			{
				cancelledFraction = doDiv(nd2, dd2);
			}
			else if ( nd1 == dd2 )
			{
				cancelledFraction = doDiv(nd2, dd1);
			}

			if ( cancelledFraction == reducedFraction )
			{
				cout << nd1 << nd2 << "/" << dd1 << dd2 << endl;
			}

			// Ditto for second digit
			cancelledFraction = 0.0;
			if ( nd2 == dd1 )
			{
				cancelledFraction = doDiv(nd1, dd2);
			}
			else if ( nd2 == dd2 )
			{
				cancelledFraction = doDiv(nd1, dd1);
			}

			if ( cancelledFraction == reducedFraction )
			{
				cout << nd1 << nd2 << "/" << dd1 << dd2 << endl;
			}

			
		}
	}

	return 0;
}
