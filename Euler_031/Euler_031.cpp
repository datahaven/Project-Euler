// Project_031.cpp 
// Adrian Dale 10/01/2009

/*
In England the currency is made up of pound, £, and pence, p, and there are eight coins in general circulation:

    1p, 2p, 5p, 10p, 20p, 50p, £1 (100p) and £2 (200p).

It is possible to make £2 in the following way:

    1×£1 + 1×50p + 2×20p + 1×5p + 1×2p + 3×1p

How many different ways can £2 be made using any number of coins?

*/

// Answer: 73682

// I think the clue is in the above equation, although I only really spotted it
// with hindsight.
// Effectively one needs to count the number of solutions to the equation
// 1a + 2b + 5c + 10d + 20e + 50f + 100g + 200h = n (where n=200 here)
//
// (For speed we can omit the 200h and increment the final solution by one, if we
// don't want to implement a generic solution that will work for any n)
//
// We also know that the numbers will be bounded pretty low
// 0 <= a <= 200
// 0 <= b <= 100
// etc

#include <iostream>
using namespace std;

// Counts all possible ways to make change for n.
// note that it needs n to be divisible by all of
// the coins in order to work properly.
//
// This could easily be fixed by altering the divisions
// in the loops.
// I think we could also skip a loop and a lot checking by
// subtracting the last variable from n, rather than trying
// all the possibilities.
// Might need to as this naive attempt will do 2000,000,000
// iterations to find the answer
int makeChange(int n)
{
	int solutionCount = 0;

	for( int a=0; a<=n; ++a )
		for( int b=0; b<=n/2; ++b)
			for( int c=0; c<=n/5; ++c)
				for( int d=0; d<=n/10; ++d)
					for( int e=0; e<=n/20; ++e)
						for( int f=0; f<=n/50; ++f)
							for( int g=0; g<=n/100; ++g)
								for( int h=0; h<=n/200; ++h)
									if (  a + 2*b + 5*c + 10*d + 20*e +
										50*f + 100*g + 200*h == n )
									{
										solutionCount++;
										cout << solutionCount << " a=" << a << " b=" << b << " c=" << c << endl;
									}

	return solutionCount;
}

// Here's someone else's - much nicer than mine!
void ex31b(int m){
int count=0;
int a, b, c, d, e, f, g;
for( a=m ; a>=0 ; a -= 200 )
for( b=a ; b>=0 ; b -= 100 )
for( c=b ; c>=0 ; c -= 50 )
for( d=c ; d>=0 ; d -= 20 )
for( e=d ; e>=0 ; e -= 10 )
for( f=e ; f>=0 ; f -= 5 )
for( g=f ; g>=0 ; g -= 2 )
count++;
printf("ex31b: count=%d\n", count);
} 

int main(int argc, char* argv[])
{
	// cout << "solutionCount=" << makeChange(200) << endl;
	makeChange(200);
	return 0;
}

