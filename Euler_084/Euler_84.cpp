// Project Euler 84
// http://projecteuler.net/index.php?section=problems&id=84
// Monopoly - most common squares to land on, given a four sided dice.
//
// Adrian Dale 28/09/2010 12:23pm - 1:20pm
//
// Nothing fancy - just simulates a large number of moves
// and works out which squares got the most hits.
// It gives the answer Euler was expecting, although I'm a little
// suspicious. I suspect the code could be quite incorrect but still
// give the right answer.
// Would have been neat to get it quicker, too.
// eg to know when we've got a stable answer.
// Still, results are produced in around half a second, so it doesn't 
// matter all that much.
//
// Was pleased to find partial sort and my idea of sorting pairs, so
// I could keep the indices of the original unsorted items.
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <Windows.h> // For GetTickCount
using namespace std;

char B[] = "..C..R.H....U..R.C....H..R..U.G..C.RH...";
int CC = 0;
int CH = 0;
int Pos = 0;
const int DiceSize = 4;
int DoubleCount = 0;

vector<pair<int,int> > MT;

template <class Function>
__int64 time_call(Function&& f)
{
   __int64 begin = GetTickCount();
   f();
   return GetTickCount() - begin;
}

void makeNextMove()
{
	int d1 = rand() % DiceSize + 1;
	int d2 = rand() % DiceSize + 1;
	
	// See if we are going to jail for a third double
	// in a row
	if ( d1 == d2 )
		++DoubleCount;
	else
		DoubleCount = 0;
	if ( DoubleCount == 3 )
	{
		DoubleCount = 0;
		Pos = 10;
		return;
	}

	Pos = (Pos + d1 + d2) % 40;

	switch(B[Pos])
	{
	case '.':
		return;
	case 'G':
		Pos = 10;
		return;
	case 'C':
		// Community Chest
		CC = ++CC % 16;
		if ( CC == 0 )
			Pos = 0;
		else if ( CC == 1 )
			Pos = 10;
		return;
	case 'H': // cHance
		CH = ++CH % 16;
		switch(CH)
		{
		case 0:
			Pos = 0;
			return;
		case 1:
			Pos = 10;
			return;
		case 2:
			Pos = 11; // C1
			return;
		case 3:
			Pos = 24; // E3
			return;
		case 4:
			Pos = 39; // H2
			return;
		case 5:
			Pos = 5; // R1
		case 6:
		case 7:
			while(B[Pos] != 'R')
				Pos = ++Pos % 40; // Next railway company
			return;
		case 8:
			while(B[Pos] != 'U')
				Pos = ++Pos % 40; // Next utility company
			return;
		case 9:
			Pos = (Pos+37) % 40; // Back three squares
			return;
		default:
			return;
		}
		return;
	case 'R': // Rail or Utility - nothing to do
	case 'U':
		return;
	default:
		cout << "Error - unhandled square: " << Pos << endl;
	}
}

void run()
{
	// Initialise our Move Tally vector
	for(int i=0; i<40; ++i)
		MT.push_back(make_pair(0,i));

	for(int i=0; i<10000000;++i)
	{
		makeNextMove();
		// Tally where that move's left us
		MT[Pos].first++;
	}

	partial_sort(MT.begin(), MT.begin() + 3, MT.end(), greater<pair<int,int> >());

	cout << "Answer=" << setw(2) << setfill('0') << MT[0].second << setw(2) 
		 << MT[1].second << setw(2)  << MT[2].second << endl;

}

int main()
{
	__int64 itTime = time_call( [&]{
		run();
	});

	cout << "Took " << itTime << " ms" << endl;


	return 0;
}