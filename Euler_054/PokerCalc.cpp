#include "PokerDeck.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Poker calculations - since repurposed for solving Project Euler 54
// Adrian Dale 20101005 and earlier

CPokerDeck Deck;

// See which pair of hole cards beats the other pair most often
int CompareHoles( int h1a, int h1b, int h2a, int h2b )
{
	int cards1[7];
	int cards2[7];

	cards1[0] = h1a;
	cards1[1] = h1b;

	cards2[0] = h2a;
	cards2[1] = h2b;

	int h1WinCount = 0;
	int h1LoseCount = 0;
	int h1DrawCount = 0;

	// Now deal out all possible boards and count up the results
	for(int c1=0;c1<52;c1++)
	{
		//cout << "c1=" << c1 << endl;
		for(int c2=c1+1;c2<52;c2++)
		{
			//cout << "c2=" << c2 << endl;
			//cout << "W=" << h1WinCount << " L=" << h1LoseCount << " D=" << h1DrawCount << endl;
			for(int c3=c2+1;c3<52;c3++)
			{
				for(int c4=c3+1;c4<52;c4++)
				{
					for(int c5=c4+1;c5<52;c5++)
					{
						// Sneaky multi-nested loop technique!
						// Note that we don't care about the order of the flop, so
						// we start each loop from after the previous card.
						// It's still humungously slow to run, though. Don't think there's
						// much I can do about that

						// First throw away flops that contain one of our hole cards
						if ( c1 == h1a || c2 == h1a || c3 == h1a || c4 == h1a || c5 == h1a ||
							 c1 == h2a || c2 == h2a || c3 == h2a || c4 == h2a || c5 == h2a ||
							 c1 == h1b || c2 == h1b || c3 == h1b || c4 == h1b || c5 == h1b ||
							 c1 == h2b || c2 == h2b || c3 == h2b || c4 == h2b || c5 == h2b )
						{
							continue;
						}

						// Flop cards can't match eachother, either
						if ( c1 == c2 || c1 == c3 || c1 == c4 || c1 == c5 ||
							             c2 == c3 || c2 == c4 || c2 == c5 ||
										             c3 == c4 || c3 == c5 ||
													             c4 == c5 )
						{
							continue;
						}

						// Now rank both hands
						cards1[2] = c1; cards1[3] = c2; cards1[4] = c3; cards1[5] = c4; cards1[6] = c5;
						cards2[2] = c1; cards2[3] = c2; cards2[4] = c3; cards2[5] = c4; cards2[6] = c5;

						int rank1 = 0;
						int subrank1 = 0;
						if ( Deck.FindBestHand( cards1, 7, &rank1, &subrank1 ) != 1 )
						{
							cout << "ERROR in FindBestHand" << endl;
							return -1;
						}
						int rank2 = 0;
						int subrank2 = 0;
						if ( Deck.FindBestHand( cards2, 7, &rank2, &subrank2 ) != 1 )
						{
							cout << "ERROR in FindBestHand" << endl;
							return -1;
						}

						// Update counts based on results
						if ( rank1 > rank2 )
							h1WinCount++;
						else if ( rank1 < rank2)
							h1LoseCount++;
						else
						{
							if ( subrank1 > subrank2 )
								h1WinCount++;
							else if ( subrank1 < subrank2 )
								h1LoseCount++;
							else
								h1DrawCount++;
						}
					}
				}
			}
		}
	}
	// Results are in
	double wldTot = h1WinCount + h1LoseCount + h1DrawCount;
	cout << "W=" << h1WinCount << " L=" << h1LoseCount << " D=" << h1DrawCount 
		   <<  " " << ((double)h1WinCount / wldTot ) * 100.0 << " "
		   <<  " " << ((double)h1LoseCount / wldTot ) * 100.0 << " "
		   <<  " " << ((double)h1DrawCount / wldTot ) * 100.0 << " "
		   << endl;
	return 1;
}

// This used to be in main() before I repurposed this to solve
// Project Euler
int oldCode()
{
	cout << "Poker Calculations - by Adrian Dale" << endl;

	// This is a mess
	// Aces are 12, 25, 38, 51
	// Cards are 0-51. Suit is arbitrary
	// 0 ==2 ... 8== 10 .. 9 == J ... 12 == A
	int cards[7];
	cards[0] = 38;
	cards[1] = 51;
	cards[2] = 14;
	cards[3] = 15;
	cards[4] = 16;

	cards[5] = 5;
	cards[6] = 6;

	// AA vs AA = 37210 37210 1637884 Win Lose Draw
	// CompareHoles(12, 25, 38, 51);

	// AA vs KK = 1409786 293155 9363

	// AA vs AKs = W=1573815 L=115345 D=23144
	// CompareHoles(12, 25, 38, 50);

	// AA vs AKo = W=1588007 L=99530 D=24767
	// CompareHoles(12, 25, 38, 11);

	// AA vs AQo = W=1580626 L=107153 D=24525
	// CompareHoles(12, 25, 38, 10);

	// AKo vs AQo = W=1236270 L=393268 D=82766 72.1992  22.9672  4.8336
	// CompareHoles(12, 50, 25, 49);

	// AA vs 45s W=1430149 L=273125 D=9030 83.5219  15.9507  0.52736

	// 77 vs 45s                   W=1010360 L=573463 D=128481 59.0059  33.4907  7.5034
	CompareHoles(5, 17, 14, 15); 


	// 77 vs 45s                   W=969875 L=619168 D=123261 56.6415  36.1599  7.19855
	CompareHoles(5, 17, 27, 28);

	return 0;
}

// Too lazy to type longer names!
typedef string::size_type SST;

int parseCard(string &in)
{
	// Aces are 12, 25, 38, 51
	// Cards are 0-51. Suit is arbitrary
	// 0 ==2 ... 8== 10 .. 9 == J ... 12 == A
	char digit = in[0];
	char suit = in[1];
	int card = 0;
	switch (suit)
	{
	case 'H':
		card = 0; break;
	case 'D':
		card = 13; break;
	case 'S':
		card = 26; break;
	case 'C':
		card = 39; break;
	default:
		cout << "ERROR invalid suit" << endl;
	}

	switch (digit)
	{
	case 'A':
		card += 12; break;
	case 'K':
		card += 11; break;
	case 'Q':
		card += 10; break;
	case 'J':
		card += 9; break;
	case 'T':
		card += 8; break;
	default:
		card += digit - '2';
	}
	return card;
}

void rankHand(string &in, int &r, int &sr)
{
	Deck.RankHand( parseCard(in.substr(0*3,2)),
				parseCard(in.substr(1*3,2)),
				parseCard(in.substr(2*3,2)),
				parseCard(in.substr(3*3,2)),
				parseCard(in.substr(4*3,2)), &r, &sr);
}

// Solves this: http://projecteuler.net/index.php?section=problems&id=54
// Glad I didn't have to rewrite all this tedious code just for that!
// Answer is that P1 wins 376 hands. This would be great code to write some
// automated tests for.
void solveEuler()
{
	ifstream hands("poker.txt");

	string input;
	int p1win = 0;
	while(!hands.eof())
	{
		getline(hands, input);
		int mainR1, subR1, mainR2, subR2;
		rankHand(input.substr(0,14), mainR1, subR1);
		rankHand(input.substr(15), mainR2, subR2);
		if ( mainR1 > mainR2 )
			p1win++;
		if ( mainR1 == mainR2 && subR1 > subR2 )
			p1win++;
	}
	cout << "Player 1 wins " << p1win << " hands" << endl;
}

int main(int argc, char *argv[])
{
	solveEuler();
	return 0;
}
