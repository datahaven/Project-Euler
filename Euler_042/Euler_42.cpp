// Euler Project 42
// http://projecteuler.net/index.php?section=problems&id=42
//
// Adrian Dale 30/08/2010
/*
The n^(th) term of the sequence of triangle numbers is given by, t_(n) = ½n(n+1);
so the first ten triangle numbers are:

1, 3, 6, 10, 15, 21, 28, 36, 45, 55, ...

By converting each letter in a word to a number corresponding to its alphabetical position 
and adding these values we form a word value. For example, the word value for SKY 
is 19 + 11 + 25 = 55 = t_(10). If the word value is a triangle number then we shall 
call the word a triangle word.

Using words.txt (right click and 'Save Link/Target As...'), a 16K text file 
containing nearly two-thousand common English words, how many are triangle words?
*/

// This works (of course!).
// Missed a neat trick to #include the file and build it into the code. Not exactly
// generic, though. Turns out you only need 27 numbers.
// Takes milliseconds anyway, so not much point optimising.
// Again, there's room to pull out a really flashy STL version.
#include <iostream>
#include <fstream>
#include <map>
#include <string>
using namespace std;

map<int,int> triangleNo;

// We're not likely to need this many
void GenerateTriangleNumbers(void)
{
	for(int i=0; i<1000; ++i)
	{
		triangleNo.insert(make_pair( (i*i+i)/2, i));
	}
}

int LetterCount( string &input )
{
	int wordSum = 0;
	for( string::iterator it = input.begin(); it != input.end(); ++it )
	{
		if ( *it >= 'A' && *it <= 'Z' )
		{
			wordSum += *it - 'A' + 1;
		}
	}
	return wordSum;
}

int main()
{
	GenerateTriangleNumbers();

	int triangleCount=0;

	string line;
	ifstream myfile ("words.txt");
	if (myfile.is_open())
	{
		while (! myfile.eof() )
		{
			getline(myfile,line, ',');
			
			map<int,int>::iterator it = triangleNo.find(LetterCount(line));
			if ( it != triangleNo.end() )
				++triangleCount;
		}
		myfile.close();
	}
	else
		cout << "Unable to open file"; 

	cout << "triangleCount=" << triangleCount << endl;

	return 0;
}
