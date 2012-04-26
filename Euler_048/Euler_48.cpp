// Euler 48
//
// Adrian Dale 04/08/2010
//
// The series, 1^(1) + 2^(2) + 3^(3) + ... + 10^(10) = 10405071317.
//
// Find the last ten digits of the series, 1^(1) + 2^(2) + 3^(3) + ... + 1000^(1000).
//
//
// Here is a J answer (not mine!)
// (10^10x)|+/^~1+i.1000x
// or
// _10{.":+/^~>:i.999x

#include <iostream>

using namespace std;

const int LAST_DIGITS = 10;
int sumSoFar[LAST_DIGITS];

int tenPow[LAST_DIGITS+1];

void plus_n( int *acc, int *n )
{
	int carry = 0;
	for(int i=0; i<LAST_DIGITS; ++i)
	{
		acc[i] += carry;
		acc[i] += n[i];
		if (acc[i] > 9)
		{
			carry = acc[i] / 10;
			acc[i] -= carry * 10;
		}
		else
			carry = 0;
	}
}

void build_n(int *acc, int n)
{
	for(int i=0; i<LAST_DIGITS; ++i)
	{
		acc[i] = (n % tenPow[i+1]) / tenPow[i];
	}
}

void times_n(int *acc, int n)
{
	int our_n[LAST_DIGITS];
	memcpy(our_n, acc, LAST_DIGITS*sizeof(int));
	//build_n(acc, 0);
	for(int i=0; i<n-1; ++i)
		plus_n(acc, our_n);
}

void power_n(int *acc, int n)
{
	build_n(acc, n);
	for( int i=0; i<n-1; ++i)
		times_n(acc, n);
}
int main()
{
	memset(sumSoFar, 0, sizeof(sumSoFar));

	tenPow[0] = 1;
	for(int i=1; i<sizeof(tenPow)/sizeof(int); ++i)
	{
		tenPow[i] = tenPow[i-1] * 10;
	}

	int n[LAST_DIGITS] = {0,0,0,0,0,0,0,0,0,0};

	
    for(int i=1; i<=1000; ++i )
	{
		power_n(n,i);
		/*for(int i=LAST_DIGITS-1; i>=0; --i)
		{
			cout << n[i];
		}
		cout << endl;*/

		plus_n(sumSoFar, n);
	}


	for(int i=LAST_DIGITS-1; i>=0; --i)
	{
		cout << sumSoFar[i];
	}
	cout << endl;
	return 0;
}
