// Euler 300
//
// Adrian Dale 04/08/2010
/*
This is the first time my code made it into the first fifty solvers .
It's rather untidy and it takes half an hour to run on my quad core AMD Phenom II X4 955 3.2 GHz processor.

Note that implementation runs in parallel on all CPU cores, so will slow down your PC a lot.

I'm sure I can get it faster, but it had a bug in one of the optimisations which I discovered by removing it from the code,
slowing it down in the process.

Basically my code goes through all possible foldings for each string, omitting strings that are the reverse of one 
already processed and with a few optimisations to the folding process. I'm surprised that other people seem to have 
used a similar sounding approach but in much faster times. I'll investigate this later, I think.
*/
#include <iostream>
#include <iomanip>
#include <ppl.h>
#include <Windows.h>
using namespace Concurrency;
using namespace std;

const int proteinLength = 15;

int pathsTested = 0;

void constructPath(char *ts, char path[], int proteinNumber, int &EndX, int &EndY);
void displayTestSpace( char *ts );

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4
#define END_PROTEIN_PATH 0

// Basically returns the position'th bit of protein number
inline char getProteinType(int proteinNumber, int position)
{
	return (proteinNumber >> position & 1) ? 'H' : 'P';	
}

int scorePath( char *ts )
{
	int score = 0;
	for( int y=0; y<proteinLength*2; ++y)
	{
		for( int x=0; x<proteinLength*2; ++x)
		{
			char cellValue = *(ts+x+(proteinLength*y*2));
			if ( cellValue == 'H')
			{
				if (x < (proteinLength*2)-1)
				{
					cellValue = *(ts+x+(proteinLength*y*2)+1);
					if ( cellValue == 'H')
					{
						++score;
					}
				}
				if (y < (proteinLength*2)-1)
				{
					cellValue = *(ts+x+(proteinLength*(y+1)*2));
					if ( cellValue == 'H')
					{
						++score;
					}
				}
			}
		}	
	}

	return score;
}

int continuePath( char path[], int proteinNumber, int maxScore )
{
	int pathLength = 2;
	for(int i=0; i<proteinLength-2 && path[i] != END_PROTEIN_PATH;++i)
		++pathLength;
	
	char testSpace[proteinLength*2][proteinLength*2];
	int EndX;
	int EndY;
	constructPath(&testSpace[0][0], path, proteinNumber, EndX, EndY);

	bool pathFinished = false;

	// - We can prune if path ends in trail of zeros as we don't
	// care where they end up, so we can score them early.
	// Or can we? Won't this allow impossible paths to get scored?
	bool zeroTail = true;
	for( int i=pathLength; i<proteinLength; ++i )
	{
		if ( getProteinType(proteinNumber, i) == 'H' )
		{
			zeroTail = false;
			break;
		}
	}

	// TODO - Test knocking this out. Is this the bug?
	// TODO - sort it out. This WAS the bug!
	//if ( zeroTail )
	//	pathFinished = true;

	// Can we prune if path begins with zeros?
	// ie just start from the first non-zero?
	if ( pathLength == 2 )
	{
		int zeroHead = 0;
		for( int i=0; i<proteinLength; ++i )
		{
			if ( getProteinType(proteinNumber, i) == 'P' )
				++zeroHead;
			else
				break;
		}
		for( int i=zeroHead; i<proteinLength && i>2 && i<pathLength; ++i )
		{
			++pathLength;
			path[i-3] = RIGHT;
			// if (i<=proteinLength-2)
				path[i+1-3] = END_PROTEIN_PATH;
		}
		// Need to reconstruct path if above prune altered it
		if (zeroHead > 2 )
			constructPath(&testSpace[0][0], path, proteinNumber, EndX, EndY);
	}

	if ( pathLength == proteinLength )
	{
		// We've finished our path, so now we need to score it
		//++pathsTested;
		pathFinished = true;
		//displayTestSpace(&testSpace[0][0]);	
	}

	if (pathFinished)
	{
		int pathScore = scorePath(&testSpace[0][0]);
		//cout << "SCORE====>"<< pathScore << endl;
		if ( pathScore > maxScore )
			maxScore = pathScore;
		return maxScore;
	}

	// - We can prune our search if the path has gone too far away from
	// the start square. Optimal solution will be bundled up
	// TODO - is this causing problems? Works for n=8, n=12 but code fails for n=15 and I suspect this.
	// However, I get the same answer for 0+, 1+ and 2+, so I'm not so sure.
	// Testing suggests it is working fine.
//	if ( EndX - (proteinLength-1) > 0+(proteinLength / 2) || EndY - (proteinLength-1) > 0+(proteinLength / 2) )
//	{
//		return maxScore;
//	}
	


	// Path not finished, so we need to grow it another step.
	// Try each direction
	if ( testSpace[EndY][EndX+1] == '.')
	{
		char newPath[proteinLength];
		memcpy(newPath, path, proteinLength);
		newPath[pathLength-2] = RIGHT;
		newPath[pathLength-1] = END_PROTEIN_PATH;
		maxScore = continuePath( newPath, proteinNumber, maxScore );
	}

	if ( testSpace[EndY-1][EndX] == '.')
	{
		char newPath[proteinLength];
		memcpy(newPath, path, proteinLength);
		newPath[pathLength-2] = UP;
		newPath[pathLength-1] = END_PROTEIN_PATH;
		maxScore = continuePath( newPath, proteinNumber, maxScore );
	}

	// A symmetry optimisation
	if ( pathLength > 2 )
	{
		if ( testSpace[EndY][EndX-1] == '.')
		{
			char newPath[proteinLength];
			memcpy(newPath, path, proteinLength);
			newPath[pathLength-2] = LEFT;
			newPath[pathLength-1] = END_PROTEIN_PATH;
			maxScore = continuePath( newPath, proteinNumber, maxScore );
		}

		if ( testSpace[EndY+1][EndX] == '.')
		{
			char newPath[proteinLength];
			memcpy(newPath, path, proteinLength);
			newPath[pathLength-2] = DOWN;
			newPath[pathLength-1] = END_PROTEIN_PATH;
			maxScore = continuePath( newPath, proteinNumber, maxScore );
		}
	}
	return maxScore;
}

void displayTestSpace( char *ts )
{
	for( int y=0; y<proteinLength*2; ++y)
	{
		for( int x=0; x<proteinLength*2; ++x)
		{
			cout << *(ts+x+(proteinLength*y*2));
		}
		cout << endl;
	}
}

// Code from the internet
// http://graphics.stanford.edu/~seander/bithacks.html#BitReverseObvious
unsigned int reverseBits( unsigned int v )
{
	unsigned int lastBit = (v & static_cast<int>(pow(2.0,proteinLength-1))) >> (proteinLength-1);
	unsigned int r = 0;
	for( int i=0; i<proteinLength-1; ++i)
	{
		unsigned int bit = v & 1;
		r |= bit;
		r <<= 1;
		v >>= 1;
	}
	return r |= lastBit;
}

void constructPath(char *ts, char path[], int proteinNumber, int &EndX, int &EndY)
{
	memset(ts,'.', proteinLength * proteinLength * 4);

	int X = proteinLength-1;
	int Y = proteinLength-1;
	*(ts+X+(proteinLength*Y*2)) = getProteinType(proteinNumber, 0);
	++X;
	*(ts+X+(proteinLength*Y*2)) = getProteinType(proteinNumber, 1);

	for(int i=0; i<proteinLength-2 && path[i] != END_PROTEIN_PATH;++i)
	{
		switch(path[i])
		{
		case UP:
			--Y;
			break;
		case DOWN:
			++Y;
			break;
		case LEFT:
			--X;
			break;
		case RIGHT:
			++X;
			break;
		}
		*(ts+X+(proteinLength*Y*2)) = getProteinType(proteinNumber, i+2);
	}

	EndX = X;
	EndY = Y;
}

int optimalFolding(int proteinNumber)
{
	int maxScore = 0;
	char path[proteinLength] = {END_PROTEIN_PATH};
	maxScore = continuePath(path, proteinNumber, maxScore);
	// cout << "Max Score(" << proteinNumber << ") = " << maxScore << endl;
	return maxScore;
}

void allOptimalFolds()
{
	int foldSum = 0;
	int maxPerm = static_cast<int>(pow(2.0,proteinLength));
#if 0
	// Single threaded version
	for( unsigned int i=0; i<maxPerm; ++i )
	{
		unsigned int revi = reverseBits(i);
		if ( revi == i )
			foldSum += optimalFolding(static_cast<int>(i));
		else if ( revi > i )
			foldSum += 2 * optimalFolding(static_cast<int>(i));

		if ( i%100 == 0 )
			cout << "Testing: " << i << endl;
	}
#endif
	combinable<int> localSum;
	parallel_for( 0, maxPerm, [&](unsigned int i) {
		unsigned int revi = reverseBits(i);
		if ( revi == i )
			localSum.local() += optimalFolding(static_cast<int>(i));
		else if ( revi > i )
			localSum.local() += 2 * optimalFolding(static_cast<int>(i));

		if ( i%100 == 0 )
			cout << "Testing: " << i << endl;
	});
	foldSum = localSum.combine(plus<int>());

	cout << "Sum of folds = " << foldSum << endl;
	cout << "Paths tested = " << pathsTested << endl;
	cout << "Average = " << setprecision(20) << static_cast<double>(foldSum) / static_cast<double>(maxPerm) << endl;
}

// From Microsoft example code:
// Calls the provided work function and returns the number of milliseconds 
// that it takes to call that function.
template <class Function>
__int64 time_call(Function&& f)
{
   __int64 begin = GetTickCount();
   f();
   return GetTickCount() - begin;
}

int main()
{
	// Tests getProteinType
	/*for( int j=32700; j<32768; ++j)
	{
	for( int i=0; i<proteinLength; ++i)
		cout << getProteinType(j,i);
	cout << endl;
	}*/

	//char testSpace[proteinLength*2][proteinLength*2];
	//char testPath[proteinLength] = {END_PROTEIN_PATH};
	// constructPath(&testSpace[0][0], testPath, 0);
	
	// optimalFolding(0);
	
	
	__int64 elapsedTime;
	
	elapsedTime = time_call([&] {
		allOptimalFolds();
	});
	
	// Tests against the example - twelve bit length,
	// path number 3309. Answer 9 is correct
	//char path[proteinLength] = {END_PROTEIN_PATH};
	//continuePath(path, 3309, 0);
	


	// Tests reverseBits fn
	/*for(unsigned int i=32000; i<32768; ++i)
	{
		cout << i << " " << reverseBits(i) << endl;
	}*/

	cout << "Time elapsed = " << elapsedTime << " ms" << endl;
	return 0;
}