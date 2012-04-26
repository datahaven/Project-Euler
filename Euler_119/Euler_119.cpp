// Project Euler 119
// http://projecteuler.net/index.php?section=problems&id=119
// Adrian Dale 29/09/2010
//
// Runs in microseconds. Took around fifty minutes to write, including
// time to investigate a couple of solutions that didn't work, such as
// using logarithms to get the number. A little bit of trial and error to
// get the magic number 100 for how many powers to try.
#include <iostream>
#include <iomanip>
#include <cmath>
#include <set>
using namespace std;

typedef long long LLT;

// Get the sum of the digits of i
LLT sumD(LLT i)
{
	int nsos = 0;
	while(i)
	{
		int nd = i % 10;
		nsos += nd;
		i /= 10;
	}
	return nsos;
}

// I tried to use this function but it doesn't work and would be too slow
// anyway. It did show me that the powers would all be low numbers, and of course
// digit sums are only a maximum of 9*no of digits.
bool isDigitPower(LLT n)
{
	LLT sum = sumD(n);
	long double logdiv = log(static_cast<long double>(n)) / log(static_cast<long double>(sum));
	if ( fabs(logdiv - floor(logdiv)) < 0.0000001 )
	{
		cout << n << " " << sumD(n) << " " <<  setprecision(4) << setw(4) << logdiv << " " << floor(logdiv) << " " << endl;
		return true;
	}
	return false;
}

void Run()
{
	set<LLT> ps;

	for( int i=0; i<100; ++i )
		for( int j=0; j<100; ++j )
		{
			LLT power = static_cast<LLT>(pow(static_cast<double>(i), static_cast<double>(j)));
			if ( power > 9 && sumD(power) == i )
				ps.insert(power);
		}

	// Answer is when n==30
	// Using a set conveniently results in the numbers being sorted
	int n = 1;
	for(set<LLT>::iterator it = ps.begin(); it != ps.end(); ++it )
		if ( n++ == 30 )
			cout << "Answer=" << *it << endl;
}

int main()
{
	Run();
	return 0;
}
