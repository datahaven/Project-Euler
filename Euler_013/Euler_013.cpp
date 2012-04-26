// Project_013.cpp 
// Adrian Dale 10/01/2009
//
// Work out the first ten digits of the sum of the following one-hundred 50-digit numbers.
// ...
// Answer: 5537376230

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// Using code from Project 020 to add big ints together,
// the only really less than simple bit is reading the numbers in.
//
// This could be far more elegant, I'm sure, but I wrote it as a quick hack.
// C++ wasn't the best choice of language for this, really, but I want to practice.
// Like, if I were cleverer, I might have noted I only needed to add the first eleven
// digits. Damn! I thought of that but sort of dismissed it as too much thought.
int main(int argc, char* argv[])
{
	vector<char> prevTotal;

	const int maxDigits = 1000;

	prevTotal.resize(maxDigits);
	fill(prevTotal.begin(), prevTotal.end(), '\0');

	vector<char> runningTotal;
	runningTotal.resize(maxDigits);
	fill(runningTotal.begin(), runningTotal.end(), '\0');

	// Read the file
	ifstream puzFile( "InputFile.txt" );
	string fileline;

	while( getline(puzFile, fileline) )
	{
		istringstream sr(fileline);
		int digit = 50;
		// Note that counting backwards is a messly little fix that I spotted when
		// I got the number back to front. Should have used iterators, after all.
		while(	sr >> prevTotal[digit] )
			prevTotal[digit--] -= '0';
	
		// Add previous result to running total 
		int carry = 0;
		for( int sumPos=0; sumPos < prevTotal.size(); ++sumPos )
		{
			runningTotal[sumPos] += prevTotal[sumPos] + carry;
			if ( runningTotal[sumPos] > 9 )
			{
				carry = runningTotal[sumPos] / 10;
				runningTotal[sumPos] %= 10;
			}
			else
				carry = 0;
		}
	}

	// Just dump out the whole number - don't bother to look for
	// the start of the first ten digits as requested, as we can
	// do this visually easily enough
	for( vector<char>::reverse_iterator it=runningTotal.rbegin();
		it != runningTotal.rend(); ++it )
	{
		cout << static_cast<char>(*it + '0');
	}

	return 0;
}
