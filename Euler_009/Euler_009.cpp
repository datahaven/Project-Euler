// Project_009.cpp : Defines the entry point for the console application.
// http://projecteuler.net/index.php?section=problems&id=9
//
// Adrian Dale 09/01/2009
//
// A Pythagorean triplet is a set of three natural numbers, a < b < c, for which,
// a^(2) + b^(2) = c^(2)
//
// For example, 3^(2) + 4^(2) = 9 + 16 = 25 = 5^(2).
//
// There exists exactly one Pythagorean triplet for which a + b + c = 1000.
// Find the product abc.
//
// Answer: a=200 b=375 c=425 a*b*c=31875000
// 

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	// Could probably guess that a will be
	// a lot less than 998, but anyway ...
	for( int a=1; a<998; ++a )
		for( int b=a+1; b<999; ++b )
			for( int c=b+1; c<1000; ++c )
				if ( a*a + b*b == c*c && a + b + c == 1000 )
					cout << "a=" << a
							<< " b=" << b
							<< " c=" << c
							<< " a*b*c=" << a*b*c << endl;
	return 0;
}