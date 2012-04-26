// Project Euler 206
// Find the unique positive integer whose square has the form 1_2_3_4_5_6_7_8_9_0,
// where each “_” is a single digit.
// Adrian Dale 30/09/2010
//
// I wrote the brain dead version below with only one minor optimisation,
// which was to step in tens so we went down to 900 million loop iterations
// instead of 9 billion. That and using the largest and smallest possible 
// square roots of 19... and 10... as boundaries.
//
// Runs in a couple of seconds. I see with a bit more intelligence I could have
// taken even bigger steps as we know what the last digits should be. My check
// fn is pretty slick, I reckon as it exits as soon as there's something not right
// and it's not messing about doing string conversions.
//
// Really, though, this one was pretty simple. I'm getting more experienced at
// solving these - spotting when things fit in a long long, for example. Working 
// out what is/isn't solvable by which methods.
#include <iostream>
using namespace std;
typedef long long LLT;

inline bool check(LLT n)
{
	if ( n % 10 != 0 ) // Not actually needed
		return false;
	
	for(int i=9; i>=1; --i)
	{
		n /= 100;
		if ( n % 10 != i )
			return false;
	}
	return true;
}

int main()
{
	for( LLT i=1010101010; i<10096008862; i+= 10 )
	{
		LLT a2 = i*i;
		if ( check(a2) )
		{
			cout << "Answer=" << i << endl;
			break;
		}
	}
	
	return 0;
}