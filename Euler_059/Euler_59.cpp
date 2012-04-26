// Euler 59
//
// Adrian Dale
#include <iostream>
#include "cipher.h"

using namespace std;

void decrypt(unsigned char a, unsigned char b, unsigned char c)
{
	cout << a << b << c << ": ";

	int sum = 0;
	for ( int i=0; i<sizeof(ct); ++i )
	{
		unsigned char key;
		if (i % 3 == 0)
			key = a;
		else if ( i % 3 == 1 )
			key = b;
		else
			key = c;

		unsigned char value = key^ct[i];
		cout << static_cast<unsigned char>(value);
		sum += value;
	}
	cout << endl;

	cout << "sum=" << sum << endl;
}

int main()
{
	// Used this code to produce all possible decryptions of the first 60 letters of
	// the message. Sent the output to a text file and grepped through it for "the"
/*
	for (unsigned char a='a'; a<='z'; ++a)
	{
		for (unsigned char b='a'; b<='z'; ++b)
		{
			for (unsigned char c='a'; c<='z'; ++c)
			{
				decrypt(a,b,c);
			}
		}
	}
*/
	// Then used this to produce the final answer:
	decrypt('g','o','d');

	return 0;
}