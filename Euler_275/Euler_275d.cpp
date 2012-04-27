// Project Euler 275 (Fourth Time Lucky?)
// Balanced Sculptures
//
// Adrian Dale 16/09/2010
#include <iostream>
#include <vector>
#include <set>
#include <bitset>
#include <string>
#include <assert.h>
#include <ppl.h>
#include <Windows.h>
#include <algorithm>

using namespace Concurrency;
using namespace std;

const int n=12;

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

typedef struct BLOCKDATATYPE
{
	int mBlocks[n];
	int mWeight;
	int mBlockCount;
	int mMiddle;
	int mLeftScore;
	int mRightScore;
};

typedef vector<BLOCKDATATYPE> BLOCKDATALISTTYPE;

//typedef struct POLYTYPE {
//	// Don't need size as this will always be depth when searching
//	// char mPolySize;
//	char mPolyX[n]; // x,y coords
//	char mPolyY[n];
//};

typedef pair<char,char> NEIGHBOURTYPE;

// Up, right, down, left
const int dirX[] = {0,1,0,-1};
const int dirY[] = {-1,0,1,0};

// Function Prototypes
int tryAllColPossibilities(BLOCKDATATYPE block);
int NEWtryAllColPossibilities(BLOCKDATATYPE block);
int NEWESTtryAllColPossibilities(BLOCKDATATYPE block);
bool checkBoard(char *board, int colCount, int middle, int maxHeight);
void showBoard(char board[n][n], int colCount);

// Called by enumerateBlockColumns
int enumerateBlockColumnsR(int w[], int digitNo, int sumsTo,
						vector<BLOCKDATATYPE> &blocks)
{
	if ( sumsTo == 0 )
	{
		BLOCKDATATYPE newBlock;
		memcpy(newBlock.mBlocks, w, sizeof(newBlock.mBlocks));

		// Calculate weight and block count
		newBlock.mWeight = 0;
		newBlock.mBlockCount = 0;
		for(int i=0;i<n && w[i]!=0 ;++i)
		{
			newBlock.mWeight += w[i]*(i+1);
			newBlock.mBlockCount++;
		}
		
		blocks.push_back(newBlock);

		return 0;
	}

	for(int i=1;i<=sumsTo;++i)
	{
		int copyw[n];
		memcpy(copyw,w,sizeof(copyw));
		copyw[digitNo] = i;
		enumerateBlockColumnsR(copyw, digitNo+1, sumsTo-i, blocks);
	}
	return 0;
}

// Calls a recursive fn to enumerate all possible numbers of 
// blocks per column for given width
void enumerateBlockColumns(int w, vector<BLOCKDATATYPE> &blocks)
{
	int width[n];
	memset(width, 0, sizeof(width));
	
	enumerateBlockColumnsR(width, 0, w, blocks);
}

BLOCKDATALISTTYPE vColumns;

int findAllPolys(BLOCKDATALISTTYPE::iterator itL, BLOCKDATALISTTYPE::iterator itR,
				int centrePillarHeight)
{
	// Gather the block counts per column from out of the separate structures
	// (ie L, middle, R) and put them in a single array of ints
	// Makes it easier to pass the values to another fn that I can now implement
	// separately, thus making testing easier
	BLOCKDATATYPE newBlock;

	memset(newBlock.mBlocks, 0, sizeof(newBlock.mBlocks));

	int colpos = 0;
	for(int i=n-1; i>=0; --i)
	{
		if ( itL->mBlocks[i] != 0 )
		{
			newBlock.mBlocks[colpos++] = itL->mBlocks[i];
		}
	}
	
	newBlock.mMiddle = colpos;
	newBlock.mBlocks[colpos++] = centrePillarHeight;
	
	for(int i=0; i<n; ++i)
	{
		if ( itR->mBlocks[i] != 0 )
		{
			newBlock.mBlocks[colpos++] = itR->mBlocks[i];
		}
	}

	newBlock.mBlockCount = colpos;
	newBlock.mLeftScore = 0;
	newBlock.mRightScore = 0;
	vColumns.push_back(newBlock);

	return 0;
}

#if 0
int getLeftScore(BLOCKDATATYPE block)
{
	char board[n][n];
	memset( &board[0][0], '.', n*n*sizeof(char) );

	int colCount = block.mMiddle+1;
	
	int maxHeight = 1;
	for(int c=0; c<colCount; ++c)
		maxHeight += block.mBlocks[c]-1;

	// Initialise each column on our board
	for(int c=0; c<colCount; ++c)
	{
		int blockCount = block.mBlocks[c];

		for(int i=0; i<maxHeight;++i)
		{
			if ( c == block.mMiddle )
			{
				// NB Sets up middle which is fixed
				for(int i=0; i<maxHeight;++i)
				{
					if ( i<blockCount )
						board[c][i] = 'X';
					else
						board[c][i] = '.';
				}
			}
			else
				if ( i<maxHeight-blockCount )
					board[c][i] = '.';
				else
					board[c][i] = 'X';
		}
	}

	int solutionCount = 0;
	bool running = true;

	int incCol = 0;
	int firstIncCol = 0;
	while(running)
	{
		if ( checkBoard(&board[0][0], colCount, block.mMiddle, maxHeight) )
			++solutionCount;

		// Increment solution
		bool carry = false;
		int incCol = 0;
		do
		{
			carry = next_permutation(&board[incCol][0], &board[incCol][maxHeight]);
			++incCol;

			if ( !carry && incCol >= colCount )
				running = false;
		}
		while( !carry && running );
	}
	// cout << "Left score=" << solutionCount << endl;

	return solutionCount;
}

// Suspiciously like getLeftScore.
// A tidy coder would merge the two!
int getRightScore(BLOCKDATATYPE block)
{
	char board[n][n];
	memset( &board[0][0], '.', n*n*sizeof(char) );

	int colCount = block.mBlockCount;
	int colStart = block.mMiddle;

	int maxHeight = 1;
	for(int c=colStart; c<colCount; ++c)
		maxHeight += block.mBlocks[c]-1;

	// Initialise each column on our board
	for(int c=0; c<colStart; ++c )
		for(int i=0; i<maxHeight;++i)
			board[c][i] = '.';

	for(int c=colStart; c<colCount; ++c)
	{
		int blockCount = block.mBlocks[c];

		for(int i=0; i<maxHeight;++i)
		{
			if ( c == block.mMiddle )
			{
				// NB Sets up middle which is fixed
				for(int i=0; i<maxHeight;++i)
				{
					if ( i<blockCount )
						board[c][i] = 'X';
					else
						board[c][i] = '.';
				}
			}
			else
				if ( i<maxHeight-blockCount )
					board[c][i] = '.';
				else
					board[c][i] = 'X';
		}
	}

	

	int solutionCount = 0;
	bool running = true;

	int incCol = 0;
	int firstIncCol = 0;
	while(running)
	{
		if ( checkBoard(&board[0][0], colCount, block.mMiddle, maxHeight) )
		{
			++solutionCount;
		}

		// Increment solution
		bool carry = false;
		int incCol = 0;
		do
		{
			if (incCol == block.mMiddle) ++incCol;
			carry = next_permutation(&board[incCol][0], &board[incCol][maxHeight]);
			++incCol;

			if ( !carry && incCol >= colCount )
				running = false;
		}
		while( !carry && running );
	}
	// cout << "Left score=" << solutionCount << endl;

	return solutionCount;
}

// Special case code for when height of middle block is fixed.
void specialCaseMiddleIsFixed()
{
	for(BLOCKDATALISTTYPE::iterator it = vColumns.begin(); it != vColumns.end(); ++it )
	{
		// Pull out the blocks we haven't already calculated a left score for
		if (it->mLeftScore != 0)
			continue;

		// Middle is one or first right is one
		if ( it->mBlocks[it->mMiddle] != 1 /* && it->mBlocks[it->mMiddle+1] != 1 */) // Just do middle==1 for now
			continue;

		int leftScore = getLeftScore(*it);

		// Cache the score everywhere else it is needed
		for(BLOCKDATALISTTYPE::iterator it2 = it; it2 != vColumns.end(); ++it2 )
		{
			if ( it2->mLeftScore != 0 )
				continue;

			// Two different cases - middle is one, or middle > 1.
			// Need to compare an extra col from rhs if middle>1 - should tidy this messy
			// code now it works.
			if ( it->mBlocks[it->mMiddle] == 1 )
			{
				if ( it2->mBlocks[it2->mMiddle] == it->mBlocks[it->mMiddle] &&
				     it2->mMiddle == it->mMiddle &&
				     memcmp(&it->mBlocks[0], &it2->mBlocks[0], (it->mMiddle+0)*sizeof(int)) == 0 )
				{
					it2->mLeftScore = leftScore;
				}
			}
			else
			{
				if ( it2->mBlocks[it2->mMiddle] == it->mBlocks[it->mMiddle] &&
				     it2->mMiddle == it->mMiddle &&
				     memcmp(&it->mBlocks[0], &it2->mBlocks[0], (it->mMiddle+1)*sizeof(int)) == 0 )
				{
					it2->mLeftScore = leftScore;
				}
			}
		}
	}

	// Quick hack - same again for Right score. Should merge the two if this works.
	for(BLOCKDATALISTTYPE::iterator it = vColumns.begin(); it != vColumns.end(); ++it )
	{
		// Pull out the blocks we haven't already calculated a left score for
		if (it->mRightScore != 0)
			continue;

		// Middle is one or first right is one
		if ( it->mBlocks[it->mMiddle] != 1 /* && it->mBlocks[it->mMiddle+1] != 1 */) // Just do middle==1 for now
			continue;

		int rightScore = getRightScore(*it);

		// Cache the score everywhere else it is needed
		for(BLOCKDATALISTTYPE::iterator it2 = it; it2 != vColumns.end(); ++it2 )
		{
			if ( it2->mRightScore != 0 )
				continue;

			// Two different cases - middle is one, or middle > 1.
			// Need to compare an extra col from rhs if middle>1 - should tidy this messy
			// code now it works.
			if ( it->mBlocks[it->mMiddle] == 1 )
			{
				if ( it2->mBlocks[it2->mMiddle] == it->mBlocks[it->mMiddle] &&
				     it2->mMiddle == it->mMiddle && it2->mBlockCount == it->mBlockCount &&
				     memcmp(&it->mBlocks[it->mMiddle], &it2->mBlocks[it->mMiddle], (it->mBlockCount - it->mMiddle+1)*sizeof(int)) == 0 )
				{
					it2->mRightScore = rightScore;
				}
			}
			else
			{
				// TODO - fix this part
				if ( it2->mBlocks[it2->mMiddle] == it->mBlocks[it->mMiddle] &&
				     it2->mMiddle == it->mMiddle &&
				     memcmp(&it->mBlocks[0], &it2->mBlocks[0], (it->mMiddle+1)*sizeof(int)) == 0 )
				{
					it2->mRightScore = rightScore;
				}
			}
		}
	}
}
#endif

int countSculptures(int a, int b, int c)
{
	// cout << "countSculptures(" << a << ", " << b << ", " << c << ") called" << endl;
	int sculptureCount = 0;
	// NB always a <= c
	assert(a <= c);

	// Easy special cases:
	// Just a central pillar
	if ( a == 0 && c == 0 )
	{
		return 1;
	}

	// Another special case - I worked it out on paper
	if ( a == 1 && c == 1 )
	{
		// sum of 1 .. b
		int bs = 0;
		for( int i=1; i<=b; ++i)
			bs += i;
		return bs;
	}

	BLOCKDATALISTTYPE vLeftBlocks;
	BLOCKDATALISTTYPE vRightBlocks;

	enumerateBlockColumns(a, vLeftBlocks);

	// Handle a == c as a special case to save us having to
	// enumerate the blocks twice and remove the duplicates. 
	if ( a != c )
	{	
		enumerateBlockColumns(c, vRightBlocks);
	}
	else
	{
		// Saves us some enumeration
		vRightBlocks = vLeftBlocks;
	}
	
	// cout << "lsize=" << vLeftBlocks.size() << " rsize=" << vRightBlocks.size() << endl;
	
	// Loop through each left block's weights looking for a matching right block, or blocks
	for( BLOCKDATALISTTYPE::iterator itL = vLeftBlocks.begin();
		itL != vLeftBlocks.end(); ++itL )
	{
		for( BLOCKDATALISTTYPE::iterator itR = vRightBlocks.begin();
			itR != vRightBlocks.end(); ++itR )
		{
			if (itL->mWeight == itR->mWeight )
			{
				sculptureCount += findAllPolys(itL, itR, b);
			}
		}	
	}

	return sculptureCount;
}

void sidesAndCentre()
{
	int sculptureCount = 0;
	
	// a and b are left side and right side
	// Enumerate all possible balancings
	// TODO - this loop is a possible candidate for parallel_for, although
	// it is very uneven in terms of amount of CPU needed for each b
	for(int b=1;b<=n;++b)
	{
		for(int a=0;a+b<=n;++a)
		{
			int c = n - b - a;

			// This case will all be mirrors of where c > a,
			// so don't need to process any more
			if ( a > c )
				continue;

			// We can quickly eliminate the cases that aren't possible.
			int mina = a; // ie blocks stacked vertically
			int maxa = 0; // ie blocks are in a horizontal row
			for(int i=0; i<a; ++i)
				maxa += i+1;

			int minc = c;
			int maxc = 0;
			for(int i=0; i<c; ++i)
				maxc += i+1;

			// If max on one side is less than min on the other, then it
			// will never be possible to make a balanced sculpture
			// For n=18 this reduces the count of left/middle/right combinations
			// down from 90 to 35
			if ( maxa < minc )
				continue;

			int sculptureCountForShape = countSculptures(a, b, c);
			cout << "(" << a << ", " << b << ", " << c << ") = " << sculptureCountForShape << endl;
			sculptureCount += sculptureCountForShape;
		}
	}

	cout << sculptureCount << " possible sculptures so far" << endl;

	// Now we have all the block heights in vColumns.
	// We need to remove any that are the reverse of another in the list
	// TODO - can't say I'm 100% this'll work for all cases. Looks OK, but suspect it if
	// problems later.
	for(BLOCKDATALISTTYPE::iterator it = vColumns.begin(); it != vColumns.end(); ++it )
	{
		int revBlocks[n];
		memset(revBlocks, 0, sizeof(revBlocks));

		int j = it->mBlockCount-1;
		for( int i=0; i<it->mBlockCount; ++i )
			revBlocks[j--] = it->mBlocks[i];
	
		// See if the reverse of this set of blocks appears anywhere else in the list
		for(BLOCKDATALISTTYPE::iterator it2 = it+1; it2 != vColumns.end(); )
		{
			bool found = true;
			for( int i=0; i<it2->mBlockCount; ++i )
			{
				if (revBlocks[i] != it2->mBlocks[i])
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				it2 = vColumns.erase(it2);
				//cout << "Duplicate Removed" << endl;
			}
			else
				++it2;
		}
	}

	// Next cool optimisation - runs of 1's can be replaced by a single 1

	for(BLOCKDATALISTTYPE::iterator it = vColumns.begin(); it != vColumns.end(); ++it )
	{
		for( int i=0; i<it->mBlockCount; ++i )
		{
			if ( i == it->mMiddle || i == it->mMiddle-1  )
				continue;

			if ( it->mBlocks[i] == 1 && it->mBlocks[i+1] == 1 )
			{
				
				if (i < it->mMiddle)
					--it->mMiddle;
				
				for(int j=i;j<it->mBlockCount;++j)
				{
					it->mBlocks[j] = it->mBlocks[j+1];
				}
				--i;
				--it->mBlockCount;
			}
		}
	}


	// TODO - If I want to handle lefts and rights separately and cache results, then
	// parallel processing won't work any more.
	// Try it out here for now.
	//specialCaseMiddleIsFixed();
	
	// Print out what we've got
	for(BLOCKDATALISTTYPE::iterator it = vColumns.begin(); it != vColumns.end(); ++it )
	{
		int maxHeight = 1;
		for( int i=0; i<it->mBlockCount; ++i )
		{
			maxHeight += it->mBlocks[i]-1;

			if (i == it->mMiddle )
			{
				cout << "(" << it->mBlocks[i] << ") ";
			}
			else
			{
				cout << it->mBlocks[i] << " ";
			}
		}
		cout << " l=" << it->mLeftScore << " r=" << it->mRightScore;
		
		cout << " mh=" << maxHeight << endl;
	}

	cout << "For n=" << n << " there are " << vColumns.size() << " sets of rows to process" << endl;
#define SINGLE_THREAD
#ifdef SINGLE_THREAD
	// Now do tryAllPossibles for each vColumns.
	// This could be the parallelised part?
	for(BLOCKDATALISTTYPE::iterator it = vColumns.begin(); it != vColumns.end(); ++it )
	{
		sculptureCount += NEWESTtryAllColPossibilities(*it);
	}
#else
	combinable<int> localSum;
	parallel_for(0, static_cast<int>(vColumns.size()), [&](int i) {
		localSum.local() += NEWESTtryAllColPossibilities(vColumns[i]);
	} );
	sculptureCount += localSum.combine(plus<int>());
#endif

	cout << "Final Solution Count = " << sculptureCount << endl;
}

void showBoard(char board[n][n], int colCount)
{
	for(int i=n; i>=0; --i)
	{
		for(int j=0; j<n;++j)
		{
			if ( board[j][i] == -1 )
				cout << ".";
			else if ( board[j][i] == n+1 )
				cout << "-";
			else
				cout << "X";
		}
		cout << endl;
	}
	cout << "================================" << endl;
}


typedef struct SOLTYPE {
	char mBoard[n][n];
};
int operator ==(const SOLTYPE &l,const SOLTYPE &r)
{
	return memcmp(&l.mBoard[0][0], &r.mBoard[0][0], n*n*sizeof(char)) == 0;
}


void testfn()
{
	int myints[]={0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1};
	int permCount = 0;
	while (next_permutation(&myints[0], &myints[17]) )
	{
		++permCount;
	}
	cout << "pc=" << permCount << endl;
}

bool testfn2()
{
	int expected[] = {0,0,0,0,0,0,18,44,111,324,964,3036,9657,31853,106153,360505,0,0,0};
	cout << "Expected score for n=" << n << " is " << expected[n] << endl;
	return true;
}

#if 0
void testfn3()
{
	BLOCKDATATYPE b;
	b.mBlockCount = 5;
	b.mMiddle = 3;
	b.mLeftScore = 0;
	memset(&b.mBlocks[0],0,sizeof(b.mBlocks));
	b.mBlocks[0] = 2;
	b.mBlocks[1] = 1;
	b.mBlocks[2] = 2;
	b.mBlocks[3] = 1; // middle
	b.mBlocks[4] = 3;
	cout << "lc=" << getLeftScore(b) << endl;
}

void testfn4()
{
	BLOCKDATATYPE b;
	b.mBlockCount = 4;
	b.mMiddle = 1;
	b.mLeftScore = 0;
	b.mRightScore = 0;
	memset(&b.mBlocks[0],0,sizeof(b.mBlocks));
	b.mBlocks[0] = 1;
	b.mBlocks[1] = 1;
	b.mBlocks[2] = 2;
	b.mBlocks[3] = 4;
	b.mBlocks[4] = 0;
	cout << "rc=" << getRightScore(b) << endl;
}
#endif


typedef vector< pair<char,char> > POLYTYPE;

struct compareSolution {
  bool operator() (const POLYTYPE& lhs, const POLYTYPE& rhs) const
  {

	  return lhs<rhs;
  }
};

typedef set<POLYTYPE, compareSolution> SOLUTIONLISTTYPE;

// Find a way to not have these global
char gBoard[n*2][n+1];
char gBlocksLeft[n*2];
int gPolyCount = 0;
SOLUTIONLISTTYPE gSolutions;
bool gSymmetryCheckNeeded;
int gPolySize;

void showBoard2()
{
	for(int i=n; i>=0; --i)
	{
		for(int j=0; j<n*2;++j)
		{
			if ( i == 0 && j == n)
				cout << "o";
			else
			{
				if ( gBoard[j][i] == -1 )
					cout << ".";
				else if ( gBoard[j][i] == n+1 )
					cout << "-";
				else
					cout << "X";
			}
		}
		cout << endl;
	}

	for(int i=0;i<n*2;++i)
		cout << static_cast<int>(gBlocksLeft[i]) << "";
	cout << endl;
	
}

bool isSolution(POLYTYPE &pp)
{
	// Need to sort the poly, otherwise we'll store duplicate solutions that
	// really are the same but
	// were formed by a different step of the algorithm 
	sort(pp.begin(), pp.end());

	// If poly might be symmetrical, then we need to see if we've already seen
	// its mirror version, too.
	if ( gSymmetryCheckNeeded )
	{
		// 2*n-x swaps x coords around y==n axis
		POLYTYPE mirrorPoly;
		for(int i=0;i<gPolySize;++i)
			mirrorPoly.push_back( make_pair(2*n-pp[i].first, pp[i].second) );
		sort(mirrorPoly.begin(), mirrorPoly.end());

		// if we find the mirror poly, then it isn't a solution
		if ( gSolutions.find(mirrorPoly) != gSolutions.end() )
			return false;
	
	}

	pair<SOLUTIONLISTTYPE::iterator, bool> status = gSolutions.insert(pp);
	return status.second;
}

bool isCanonical(POLYTYPE pp)
{
	sort(pp.begin(), pp.end());

	// 2*n-x swaps x coords around y==n axis
	POLYTYPE mirrorPoly;
	for(POLYTYPE::iterator it = pp.begin(); it != pp.end(); ++it)
		mirrorPoly.push_back( make_pair(2*n - it->first, it->second) );
	sort(mirrorPoly.begin(), mirrorPoly.end());

	if ( mirrorPoly == pp ) // Symmetrical
		return true;
	return mirrorPoly > pp;
}

// Passing in depth, (x,y), parent
void searchBlock(int depth, int x, int y, POLYTYPE pp)
{
	/*cout << "searchBlock(" << depth << ", " << x << ", " << y << endl;*/

	

	// Is pp canonical? If not, then we can stop immediately.
	// Then we won't need to check for symmetry and should half search time
	// if ( isCanonical(pp) == false )
	//	return;
	// Doesn't work, though. Maybe only if doing proper rooted poly search?
	// Or is it because I'm incorporating balance weights?

	// Do the move
	gBoard[x][y] = depth;

	// Update remaining columns count
	--gBlocksLeft[x];

	// Add move to next depth's parent poly
	pp.push_back( make_pair(x,y) );
	
	/*pp.mPolyX[depth] = x;
	pp.mPolyY[depth] = y;*/

	// See if we've finished.
	if (depth == gPolySize-1 )
	{		
		if ( isSolution(pp) )
		{
			++gPolyCount;
			//showBoard2();
		}			
	}
	else
	{
		// For each square in parentPoly, make all possible moves from there
		set<NEIGHBOURTYPE> nextMoves;
		for( int p=0; p<=depth;++p )
		{
			/*int px = pp.mPolyX[p];
			int py = pp.mPolyY[p];*/
			int px = pp[p].first;
			int py = pp[p].second;
			for(int dXY=0;dXY<4;++dXY)
			{
				int x2 = px+dirX[dXY];
				int y2 = py+dirY[dXY];
				if (gBoard[x2][y2] == -1 && gBlocksLeft[x2] > 0)
				{
					NEIGHBOURTYPE nm;
					nm.first = x2; nm.second = y2;
					nextMoves.insert(nm);
				}
			}
		}
		for(set<NEIGHBOURTYPE>::iterator it = nextMoves.begin();
			it != nextMoves.end(); ++it)
		{		
				searchBlock(depth+1, it->first, it->second, pp);	
		}
	}

	// Put remaining columns count back to how it was when we started
	++gBlocksLeft[x];

	// Undo the move we did at the start
	gBoard[x][y] = -1;
}

int EvenNewerTAP(BLOCKDATATYPE b)
{
	// My new bit:
	/*cout << "tryAllPossibilities(): ";
	for(int i=0;i<b.mBlockCount;++i)
		cout << b.mBlocks[i] << " ";
	cout << endl;*/
	
	// Set up
	for(int j=0; j<n+1; ++j)
	{
		for(int i=0;i<n*2;++i)
		{
			if ( j==0 )
				gBoard[i][j] = n+1;
			else
				gBoard[i][j] = -1;
		}
	}

	// Poly might not be size n if we've chopped out some repeated 1's
	gPolySize = 0;

	// Record the blocks we have left for each column
	memset(gBlocksLeft, 0, sizeof(gBlocksLeft));
	for(int i=0;i<b.mBlockCount;++i)
	{
		gPolySize += b.mBlocks[i];
		gBlocksLeft[i+n-b.mMiddle] = b.mBlocks[i];
	}

	gPolyCount = 0;
	gSolutions.clear();

	POLYTYPE parentPoly;
	
	__int64 elapsedTime;
	elapsedTime = time_call([&] {
		// Kick off the recursion
		searchBlock(0, n, 1, parentPoly);
	});

	cout << "tryAllPossibilities(): ";
	for(int i=0;i<b.mBlockCount;++i)
		cout << b.mBlocks[i] << " ";
	cout << "Search took " << elapsedTime << " ms for " << gPolyCount << " solutions";
	cout << endl;

	return gPolyCount;
}

int NEWESTtryAllColPossibilities(BLOCKDATATYPE b)
{

	// We save a little time by not checking for symmetry in cases
	// where we know there can't be any
	// TODO - be careful with this!
	gSymmetryCheckNeeded = b.mBlockCount-b.mMiddle-1 == b.mMiddle;
	if ( gSymmetryCheckNeeded )
	{
		// List has same number of terms about the middle, so compare them
		// to see if terms are the same
		int i=b.mMiddle;
		int j=b.mMiddle;
		while(i>=0)
		{
			if (b.mBlocks[i] != b.mBlocks[j])
			{
				gSymmetryCheckNeeded = false;
				break;
			}
			--i; ++j;
		}
	}
	
	// Have we cached any info previously?
	// Currently only for middle==1
	if ( b.mLeftScore != 0 && b.mRightScore != 0)
	{
		if ( gSymmetryCheckNeeded )
		{
			// sum of 1 .. left score
			int bs = 0;
			for( int i=1; i<=b.mLeftScore; ++i)
				bs += i;
			return bs;
		}
		else
			return b.mLeftScore * b.mRightScore;
	}

	return EvenNewerTAP(b);
}

void testfn7()
{
	char board[18][18];
	BLOCKDATATYPE b;
	b.mBlockCount = 5;
	b.mMiddle = 2;
	b.mLeftScore = 0;
	b.mRightScore = 0;
	memset(&b.mBlocks[0],0,sizeof(b.mBlocks));
	b.mBlocks[0] = 1;
	b.mBlocks[1] = 3;
	b.mBlocks[2] = 4;
	b.mBlocks[3] = 3;
	b.mBlocks[4] = 1;
	b.mBlocks[5] = 0;
	//cout << "sc=" << NEWESTtryAllColPossibilities(b) << endl;

	b.mBlockCount = 4;
	b.mMiddle = 2;
	b.mLeftScore = 0;
	b.mRightScore = 0;
	memset(&b.mBlocks[0],0,sizeof(b.mBlocks));
	b.mBlocks[0] = 1;
	b.mBlocks[1] = 3;
	b.mBlocks[2] = 4;
	b.mBlocks[3] = 3;
	b.mBlocks[4] = 0;
	b.mBlocks[5] = 0;
	cout << "sc=" << NEWESTtryAllColPossibilities(b) << endl;

}



int main()
{
	__int64 elapsedTime;
	
	
	testfn7();
	return 0;

	elapsedTime = time_call([&] {
		sidesAndCentre();
	});
	cout << "Finished in " << elapsedTime << " ms" << endl;
	
	cout << endl << endl;
	
	//countSculptures(5,1,12);

	//int testCols[] = {2,1,1,1,1,10,2,0,0,0,0,0,0,0,0,0,0,0};
	//tryAllColPossibilities(testCols, 4);

	//testfn();
	testfn2();
	return 0;

}