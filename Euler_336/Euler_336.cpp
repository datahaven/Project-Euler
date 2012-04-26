// Project Euler 336
// Adrian Dale
// 23/05/2011
//
// Answer: CAGBIHEFJDK
//
// My code cheats a little in that I ran a first pass to find out what the maximum
// maximix number is, then I run again counting solutions up to that number.
// Not clever or especially efficient, but runs in a couple of seconds.
// Not convinced that mixcount is bugfree for all inputs but it works for this puzzle.
// Solution relies on next_permutation being able to run through 11! permutations
// (ie 39 million) and check each one very easily on a modern computer. Took a few mins
// to write code to check that this wouldn't take too long with a bit of string
// manipulation for each value and after that it seemed sensible to proceed with this
// approach.
// Also helpful that next_permutation produces permutations in lexicographical order
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

// NB pass by value as code messes with s
int mixcount(string s)
{
	string a = "ABCDEFGHIJK";
	int mcnt = 0;
	for( int i=0; i<a.length()-2; ++i )
	{
		// No flipping needed if coach is already in the right place
		if (a[i] == s[i])
			continue;
		
		// Find next coach we need
		string::size_type mpos = s.find(a[i]);
		// and if it isn't at end of list ...
		if ( mpos != s.length()-1 )
		{
			reverse( s.begin()+mpos, s.end() );
			++mcnt;
			reverse(s.begin()+i, s.end());
			++mcnt;
		}
		else
		{
			// next coach is at end of list
			reverse( s.begin(), s.end() );
			++mcnt;
		}
	}

	// Last two coaches need swapping
	if ( a[a.length()-2] != s[a.length()-2] )
	{
		++mcnt;
		// Don't need to actually swap the chars, but did it
		// like this so I could display string whilst testing
		// reverse(s.begin()+a.length()-2, s.end());
	}

	return mcnt;
}

int main()
{
	string s = "ABCDEFGHIJK";
	
	int maxmc = 0;
	int pcnt = 0;
	do
	{
		int mc = mixcount(s);
		maxmc = max(maxmc, mc);
		
		if ( mc == 19 ) // value calculated in previous run
		{
			++pcnt;
			if ( pcnt == 2011 )
				break;
		}
	} while (next_permutation( s.begin(), s.end() ) );
	
	cout << "maxmc=" << maxmc << endl;
	cout << s << endl;
	return 0;
}
