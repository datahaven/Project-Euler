// Project Euler 098
// Word Anagrams / Square Numbers
// Adrian Dale 27/10/2011
//
// Congratulations, the answer you gave to problem 98 is correct.
//
// You are the 2906th person to have solved this problem.
//
// Return to Problems page or go to thread 98 in the forum.
//
// Found matching pair: 17689 18769 BOARD BROAD
//
// This rather dubious code does way too much messing around and
// converting from string to int, etc.
// I believe it would also not have worked properly if the correct
// answer had been SHEET/THESE which was the only word which had
// repeated letters.
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
using namespace std;

vector<string> Words;

vector< vector<string> > Anagrams;

set<int> WordLengths;

bool readFile()
{
	ifstream wordsFile;
	wordsFile.open("words.txt", ios::in);
	if (wordsFile.is_open() == false)
		return false;

	// For each line in the file
	while( wordsFile.good() )
	{
		// Read in the line
		string line;
		wordsFile >> line;
		
		stringstream lineStream(line);
		string word;
		// Split line on commas into words
        while(getline(lineStream,word,','))
        {
			// Remove quote chars from the words
			word.erase(remove(word.begin(), word.end(), '"'), word.end());
			// Add to our vector
            Words.push_back(word);
        }
	}

	wordsFile.close();

	cout << "Read " << Words.size() << " words" << endl;
	return true;
}

// NB Caused a horrible bug when I used references as
// this fn sorts w1 and w2
bool isAnagram(string w1, string w2)
{
	if (w1.size() != w2.size())
		return false;
	if (w1 == w2)
		return false;
	
	sort(w1.begin(), w1.end());
	sort(w2.begin(), w2.end());

	return w1==w2;
}

void findAnagrams()
{
	// Find all words that are an anagram of one or more other words.
	// for_each lambda version was neater!
	for( auto w1it = Words.begin(); w1it != Words.end(); ++w1it )
	{
		vector<string> anagram;
		for( auto w2it = Words.begin(); w2it != Words.end(); ++w2it )
		{
			if ( isAnagram(*w1it, *w2it) )
				anagram.push_back(*w2it);
		}
		if (anagram.size() != 0)
		{
			anagram.push_back(*w1it);
			sort(anagram.begin(), anagram.end());
			Anagrams.push_back(anagram);
		}
	}
	// When this finishes there will be duplicates, which this code removes
	sort(Anagrams.begin(), Anagrams.end());
	Anagrams.erase( unique(Anagrams.begin(), Anagrams.end()), Anagrams.end());

	cout << "Found " << Anagrams.size() << " anagrams" << endl;

	// This is a bit of a cheat - this code shows we have one special case to deal with:
	// POST, SPOT, STOP
	for_each( Anagrams.begin(), Anagrams.end(), [&](vector<string> &anagram) {
		if (anagram.size() != 2)
		{
			cout << "Anagram size is " << anagram.size() << " " << anagram[0] << endl;
		}
	});
	// So, simply remove that case and add it in manually
	Anagrams.erase( remove_if( Anagrams.begin(), Anagrams.end(), [&](vector<string> &anagram) {
						return anagram.size() != 2;
					}), Anagrams.end() );
	// Now all our words are in pairs, which should make solving easier.
	vector<string> t1;
	t1.push_back( string("POST") );
	t1.push_back( string("STOP") );
	Anagrams.push_back(t1);
	t1.clear();
	t1.push_back( string("POST") );
	t1.push_back( string("SPOT") );
	Anagrams.push_back(t1);
	t1.clear();
	t1.push_back( string("STOP") );
	t1.push_back( string("SPOT") );
	Anagrams.push_back(t1);

	// Remove all our words less than four chars long as puzzle definition already says
	// there will be a four or more digit answer
	Anagrams.erase( remove_if( Anagrams.begin(), Anagrams.end(), [&](vector<string> &anagram) {
		return anagram[0].size() < 4;
	}), Anagrams.end() );

	// Populate our WordLengths set - not 100% needed. Just me being flash!
	for_each( Anagrams.begin(), Anagrams.end(), [&](vector<string> &anagram) {
		WordLengths.insert( anagram[0].size() );
	});

	//for_each(Anagrams.begin(), Anagrams.end(), [&](vector<string> &anagram) {
	//	cout << anagram[0] << " " << anagram[1] << endl;
	//});

	cout << "Word Lengths: ";
	for_each( WordLengths.begin(), WordLengths.end(), [&](int wl) {
		cout << wl << " ";
	});
	cout << endl;
}

vector<int> Squares;

vector< pair<int,int> > SquarePairs;

bool isNumberAnagram(int sq1, int sq2)
{
	ostringstream sq1str;
	sq1str << sq1;

	ostringstream sq2str;
	sq2str << sq2;

	return isAnagram( sq1str.str(), sq2str.str() );
}

void generateSquareNumbers()
{
	for(int i=11; i<35000; ++i)
	{
		int i2 = i*i;
		set<int> digits;
		bool dupFound = false;
		int digitCount = 0;
		while( i2 > 0 )
		{
			int digit = i2%10;
			i2 /= 10;

			++digitCount;
			pair<set<int>::iterator,bool> ret;
			ret = digits.insert(digit);
			if ( ret.second == false )
			{
				dupFound = true;
				break;
			}
		}
		// Only include this square if we have words of that length in our list
		if (dupFound == false && WordLengths.find(digitCount) != WordLengths.end() )
			Squares.push_back(i*i);
	}
	// Not needed, I don't think
	sort(Squares.begin(), Squares.end());
	cout << "Found " << Squares.size() << " squares without repeated digits" << endl;
	//for_each(Squares.begin(), Squares.end(), [&](int sq) {
	//	cout << sq << " ";
	//});

	// Find the square pairs
	for_each(Squares.begin(), Squares.end(), [&](int sq1) {
		for_each(Squares.begin(), Squares.end(), [&](int sq2) {
			if (sq1==sq2)
				return;
			if (isNumberAnagram(sq1, sq2)==true)
			{
				//cout << "Found pair: " << sq1 << " " << sq2 << endl;
				if ( sq1 < sq2 )
					SquarePairs.push_back( make_pair<int,int>(sq1,sq2) );
			}
		});
	});

	cout << "Found " << SquarePairs.size() << " pairs of squares" << endl;
}

vector<int> makeMap( string &s1, string &s2 )
{
	vector<int> theMap;

	// Returns a "map" of the position in s2 of each letter in s1
	for(int i=0; i<s1.size(); ++i)
	{
		size_t s2pos = s2.find(s1[i]);
		//cout << s2pos << " ";
		theMap.push_back(s2pos);
	}
	return theMap;
}

void findMatches()
{
	for_each( Anagrams.begin(), Anagrams.end(), [&](vector<string> &anagram) {

		vector<int> wordMap = makeMap(anagram[0], anagram[1]);

		for_each(SquarePairs.begin(), SquarePairs.end(), [&](pair<int,int> sp) {
			ostringstream sq1str;
			sq1str << sp.first;

			ostringstream sq2str;
			sq2str << sp.second;

			vector<int> map1 = makeMap(sq1str.str(), sq2str.str());
			vector<int> map2 = makeMap(sq2str.str(), sq1str.str());

			if ( map1 == wordMap || map2 == wordMap )
			{
				cout << "Found matching pair: " << sp.first << " " << sp.second
					<< " " << anagram[0] << " " << anagram[1] << endl;
			}
		});

	});
}

int main()
{
	if (readFile() == false)
	{
		cout << "Error reading words.txt" << endl;
		return -1;
	}

	findAnagrams();

	generateSquareNumbers();

	//makeMap(string("INTRODUCE"),string("REDUCTION"));

	findMatches();

	return 0;
}