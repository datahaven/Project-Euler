// http://projecteuler.net/index.php?section=problems&id=40
// Should be able to do this on paper, but I'm struggling!
//
// Adrian Dale

/*
An irrational decimal fraction is created by concatenating the positive integers:

0.123456789101112131415161718192021...

It can be seen that the 12^(th) digit of the fractional part is 1.

If d_(n) represents the n^(th) digit of the fractional part, find the value of the following expression.

d_(1) × d_(10) × d_(100) × d_(1000) × d_(10000) × d_(100000) × d_(1000000
*/

// I think this solution amounts to intellectual vandalism. The thuggish
// way to find the answer. Uses (ie wastes) heaps of memory and does a massive loop
// to find an answer that could almost certainly be calculated by pen and paper if one
// were sufficiently careful. I wasn't!
// Solves the problem in the most literal way possible.
#include <iostream>
using namespace std;

char digits[1005000];

int main()
{
	cout << "Euler 40" << endl;

	char *pDigit = digits;
	memset(digits, '\0', sizeof(digits));
	for (int i=1; i<1000000; ++i)
	{
		if (pDigit-digits > 1004000)
			break;

		sprintf(pDigit, "%d", i);
		
		if ( i < 10 )
			pDigit+=1;
		else if ( i < 100 )
			pDigit+=2;
		else if ( i < 1000 )
			pDigit+=3;
		else if ( i < 10000 )
			pDigit+=4;
		else if ( i < 100000 )
			pDigit+=5;
		else if ( i < 1000000 )
			pDigit+=6;
		else if ( i < 10000000 )
			pDigit+=7;
	}

	cout << digits[0] << digits[9] << digits[99] << digits[999] 
	     << digits[9999] << digits[99999] << digits[999999] << endl;
	
	cout << "Answer=" << ( (digits[0] - '0') * (digits[9] - '0') * (digits[99] - '0') *
							(digits[999] -'0') * (digits[9999] - '0') * (digits[99999] - '0') *
							(digits[999999] - '0') ) << endl;
	return 0;
}
