// Project Euler 275
// Balanced Sculptures
//
// Adrian Dale 11/09/2010
#include <iostream>
#include <vector>
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


// Function Prototypes
int tryAllColPossibilities(BLOCKDATATYPE block);
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
	specialCaseMiddleIsFixed();
	
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

#ifdef SINGLE_THREAD
	// Now do tryAllPossibles for each vColumns.
	// This could be the parallelised part?
	for(BLOCKDATALISTTYPE::iterator it = vColumns.begin(); it != vColumns.end(); ++it )
	{
		sculptureCount += tryAllColPossibilities(it->mBlocks, it->mMiddle, false);
	}
#else
	combinable<int> localSum;
	parallel_for(0, static_cast<int>(vColumns.size()), [&](int i) {
		localSum.local() += tryAllColPossibilities(vColumns[i]);
	} );
	sculptureCount += localSum.combine(plus<int>());
#endif

	cout << "Final Solution Count = " << sculptureCount << endl;
}

// Recursive fn to track which squares can be reached
void markBoard(char *board, int colCount, int x, int y)
{

	if ( *(board + n*x + y) == '.' || *(board + n*x + y) == 'V')
		return;

	*(board + n*x + y) = 'V';

	if ( x > 0 )
		markBoard(board, colCount, x-1, y);
	if ( x < colCount-1 )
		markBoard(board, colCount, x+1, y);
	if ( y > 0 )
		markBoard(board, colCount, x, y-1);
	if ( y < n-1 )
		markBoard(board, colCount, x, y+1);
}

bool checkBoard(char *board, int colCount, int middle, int maxHeight)
{
	// Sanity check before we go recursive - is the middle connected?
	// Possibly not worth doing this.
	if ( *(board + n*middle+ 1) == '.' && *(board + n*(middle-1)) == '.' && *(board + n*(middle+1)) == '.')
		return false;

	// Look for obvious shortcuts - eg if any square has no neighbours return false
	for(int i=0; i<colCount; ++i)
	{
		for(int j=0; j<maxHeight;++j)
		{
			if ( *(board +n*i + j) == 'X' )
			{
				if ( i>0 && *(board + n*(i-1) + j) == 'X')
					break;
				if ( i<colCount-1 && *(board + n*(i+1)+j) == 'X')
					break;
				if ( j>0 && *(board + n*i + j-1) == 'X')
					break;
				if ( j<maxHeight-1 && *(board + n*i+j+1) == 'X')
					break;
				return false;
			}
		}
	}

	int startX = middle;
	int startY = 0;

	markBoard(board, colCount, startX, startY);

	// Scan board and see if there are any squares we didn't visit
	bool result = true;
	for(int i=0; i<colCount; ++i)
	{
		for(int j=0; j<maxHeight;++j)
		{
			if (*(board +n*i + j) == 'X')
				result = false;
			// Put board back the way we found it
			if (*(board +n*i + j) == 'V')
				*(board +n*i + j) = 'X';
		}
	}

	return result;
}

bool NEWcheckBoard(char *board, int colCount, int middle, int maxHeight)
{
	// Look for obvious shortcuts - eg if any square has no neighbours return false
	// TODO - test if this is actually a shortcut, given that I now have the AND check.
	for(int i=0; i<colCount; ++i)
	{
		for(int j=0; j<maxHeight;++j)
		{
			if ( *(board +n*i + j) == 'X' )
			{
				if ( i>0 && *(board + n*(i-1) + j) == 'X')
					break;
				if ( i<colCount-1 && *(board + n*(i+1)+j) == 'X')
					break;
				if ( j>0 && *(board + n*i + j-1) == 'X')
					break;
				if ( j<maxHeight-1 && *(board + n*i+j+1) == 'X')
					break;
				return false;
			}
		}
	}

	int startX = middle;
	int startY = 0;

	markBoard(board, colCount, startX, startY);

	// Scan board and see if there are any squares we didn't visit
	bool result = true;
	for(int i=0; i<colCount; ++i)
	{
		for(int j=0; j<maxHeight;++j)
		{
			if (*(board +n*i + j) == 'X')
			{
				result = false;
				i = colCount;
				break;
			}
		}
	}

	return result;
}

void showBoard(char board[n][n], int colCount)
{
	for(int i=0; i<n/*colCount*/; ++i)
	{
		for(int j=0; j<n;++j)
		{
			cout << board[i][j];
		}
		cout << endl;
	}
	cout << "================================" << endl;
}

int getChecksum(char board[n][n])
{
	// Get the board's checksum
	int checksum = 1;
	for(int i=0; i<n; ++i)
	{
		for(int j=0; j<n;++j)
		{
			if (board[i][j] == 'X')
			{
				checksum *= (i+1)*n + (j + 1);
				//cout << checksum << " ";
			}
			else
				checksum += (i+1)*n + (j + 1);
		}
	}
	return checksum;
}

// getChecksums is not currently used. Keep it as it might be re-instated,
// given that I belive the current implementation does work and should
// use a lot less memory than storing whole board positions.
void getChecksums(char board[n][n], int middle, int &sum, int &revsum)
{
	// Get the board's checksum
	sum = getChecksum(board);

	// Get the checksum of the board's mirror image
	// TODO - should probably be able to do this without
	// actually copying the board. Do it by altering the checksum
	// routine?
	char mirrorBoard[n][n];
	memset(&mirrorBoard[0][0], '.', sizeof(char)*n*n);
	int i = middle;
	int j = middle;
	while(i >= 0)
	{
		for(int k=0; k<n;++k)
		{
			mirrorBoard[i][k] = board[j][k];
			mirrorBoard[j][k] = board[i][k];
		}
		--i; ++j;
	}

	/*cout << "=================="<<endl;
	showBoard(board, n);
	cout << "mmmmmmmmmmmmmmmm" << endl;
	showBoard(mirrorBoard, n);
	cout << "=================="<<endl;*/

	revsum = getChecksum(mirrorBoard);
}

typedef struct SOLTYPE {
	char mBoard[n][n];
};
int operator ==(const SOLTYPE &l,const SOLTYPE &r)
{
	return memcmp(&l.mBoard[0][0], &r.mBoard[0][0], n*n*sizeof(char)) == 0;
}

inline bool incrementColumn(char board[n][n], int maxHeight, BLOCKDATATYPE block, int colCount, int incCol)
{
	// To save us some incrementing, anytime all the Xs on one row
	// are above the ones on the row below, shuffle them all down at once
	//int lowestXPos = -1;
	//for( int i=0; i<maxHeight; ++i)
	//{
	//	if ( board[incCol][i] == 'X' )
	//	{
	//		lowestXPos = i;
	//		break;
	//	}
	//}
	//int highestXPos = -1;
	//for( int i=maxHeight-1; i>=0; --i)
	//{
	//	if ( board[incCol+1][i] == 'X' )
	//	{
	//		highestXPos = i;
	//		break;
	//	}
	//}
	//if ( lowestXPos > highestXPos )
	//{
	//	//cout << "shuffling: " << lowestXPos << " " << highestXPos << " mh=" << maxHeight << endl;
	//	for(int i=highestXPos; i<maxHeight; ++i)
	//	{
	//		if ( i+lowestXPos-highestXPos < maxHeight )
	//			board[incCol][i] = board[incCol][i+lowestXPos-highestXPos];
	//		else
	//			board[incCol][i] = '.';
	//	}
	//	return true; // No need to increment now
	//}

	bool carry;
	if (incCol == block.mMiddle )
		carry = next_permutation(&board[incCol][1], &board[incCol][maxHeight]);
	else
		carry = next_permutation(&board[incCol][0], &board[incCol][maxHeight]);

	return carry;
}

inline bool incrementBoard(char board[n][n], int maxHeight, BLOCKDATATYPE block, int colCount)
{
	// incrementColumn(board, maxHeight, block, colCount, 0);

	bool running = true;
	
	
	//showBoard(board, colCount);

	bool carry = false;
	int incCol = 0;
	do
	{
		carry = incrementColumn(board, maxHeight, block, colCount, incCol);
		/*if (incCol == block.mMiddle )
			carry = next_permutation(&board[incCol][1], &board[incCol][maxHeight]);
		else
			carry = next_permutation(&board[incCol][0], &board[incCol][maxHeight]);*/
		++incCol;
		if ( !carry && incCol >= colCount )
			return false;
	}
	while( !carry && running );


	// OK, but there's got to be better ways...
	//// See what biggest gap in last incremented row is
	//incCol--;
	//if ( block.mBlocks[incCol] >= 2 && maxHeight > 3)
	//{
	//	int gap = 0;
	//	int bestgap = 0;
	//	bool inGap = false;
	//	for(int i=0; i<maxHeight; ++i )
	//	{
	//		if ( inGap )
	//		{
	//			if (board[incCol][i] == 'X')
	//			{
	//				inGap = false;
	//				if ( gap > bestgap )
	//					bestgap = gap;
	//			}
	//			else
	//			{
	//				++gap;
	//			}
	//		}
	//		else /* not in gap */
	//		{
	//			if (board[incCol][i] == 'X')
	//			{
	//				inGap = true;
	//				gap = 0;
	//			}	
	//		}
	//	}
	//}

	return running;
}

int tryAllColPossibilities(BLOCKDATATYPE block)
{
	char board[n][n];

	
	memset( &board[0][0], '.', n*n*sizeof(char) );

	int colCount = block.mBlockCount;
	
	int maxHeight = 1;
	for(int c=0; c<colCount; ++c)
		maxHeight += block.mBlocks[c]-1;

	// We save a little time by not checking for symmetry in cases
	// where we know there can't be any
	bool symmetryCheckNeeded = colCount-block.mMiddle-1 == block.mMiddle;
	if ( symmetryCheckNeeded )
	{
		// List has same number of terms about the middle, so compare them
		// to see if terms are the same
		int i=block.mMiddle;
		int j=block.mMiddle;
		while(i>=0)
		{
			if (block.mBlocks[i] != block.mBlocks[j])
			{
				symmetryCheckNeeded = false;
				break;
			}
			--i; ++j;
		}
	}
	
	// Have we cached any info previously?
	// Currently only for middle==1
	if ( block.mLeftScore != 0 && block.mRightScore != 0)
	{
		if ( symmetryCheckNeeded )
		{
			// sum of 1 .. left score
			int bs = 0;
			for( int i=1; i<=block.mLeftScore; ++i)
				bs += i;
			return bs;
		}
		else
			return block.mLeftScore * block.mRightScore;
	}

	// Initialise each column on our board
	for(int c=0; c<colCount; ++c)
	{
		int blockCount = block.mBlocks[c];

		if ( c == block.mMiddle )
		{
			// NB Sets up middle, which is different as bottom square
			// is fixed. (or could ignore and have this come out when we
			// check solution validity?)
			// for other rows loop from 0 and use n-blockCount
			board[c][0] = 'X';
			for(int i=1; i<maxHeight;++i)
			{
				if ( i<maxHeight-blockCount+1 )
					board[c][i] = '.';
				else
					board[c][i] = 'X';
			}
		}
		else
		{
			// for other rows loop from 0 and use n-blockCount
			for(int i=0; i<maxHeight;++i)
			{
				if ( i<maxHeight-blockCount )
					board[c][i] = '.';
				else
					board[c][i] = 'X';
			}
		}
	}
	

	int solutionCount = 0;
	bool running = true;
	vector<SOLTYPE> boardList;
	int incCol = 0;
	int firstIncCol = 0;
	while(running)
	{
		// Above code, only storing a copy of entire board, instead of using checksum,
		// which appears to be a shade unreliable
		if ( checkBoard(&board[0][0], colCount, block.mMiddle, maxHeight) )
		{
			if (symmetryCheckNeeded == false)
			{
				++solutionCount;
			}
			else
			{
				SOLTYPE ourBoard;
				memcpy(ourBoard.mBoard, board, n*n*sizeof(char));

				// Mirror board
				SOLTYPE mirrorBoard;
				memset(mirrorBoard.mBoard, '.', sizeof(char)*n*n);
				int i = block.mMiddle;
				int j = block.mMiddle;
				while(i >= 0)
				{
					for(int k=0; k<maxHeight;++k)
					{
						mirrorBoard.mBoard[i][k] = board[j][k];
						mirrorBoard.mBoard[j][k] = board[i][k];
					}
					--i; ++j;
				}
				// Re-centre, if needed
				int firstPopulatedCol = -1;
				for(i=0;i<colCount && firstPopulatedCol == -1;++i)
				{
					for(j=0;j<maxHeight;++j)
					{
						if (mirrorBoard.mBoard[i][j] == 'X')
						{
							firstPopulatedCol = i;
							break;
						}
					}
				}
				assert(firstPopulatedCol != -1); // Something bad has happened!
				// Shuffle up the columns
				for(i=0;i<n-firstPopulatedCol;++i)
				{
					for(j=0;j<maxHeight;++j)
					{
						mirrorBoard.mBoard[i][j] = mirrorBoard.mBoard[firstPopulatedCol + i][j];
					}
				}
				// Blank out the final copied rows - do we ever need this?
				for(i=n-firstPopulatedCol;i<n;++i) 
				{
					for(j=0;j<maxHeight;++j)
					{
						mirrorBoard.mBoard[i][j] = '.';
					}
				}

				if ( find( boardList.begin(), boardList.end(), ourBoard ) == boardList.end() &&
					find( boardList.begin(), boardList.end(), mirrorBoard ) == boardList.end() )
				{
					boardList.push_back(ourBoard);
					++solutionCount;
				}
			}
		}


		// This works!
#if 0
		// Increment solution		
		bool carry = false;
		int incCol = 0;
		do
		{
			if (incCol == block.mMiddle )
				carry = next_permutation(&board[incCol][1], &board[incCol][maxHeight]);
			else
				carry = next_permutation(&board[incCol][0], &board[incCol][maxHeight]);

			++incCol;
			if ( !carry && incCol >= colCount )
				running = false;
		}
		while( !carry && running );
#endif
		
		running = incrementBoard(board, maxHeight, block, colCount);
	}

	cout << "Final sc=" << solutionCount << endl;

	return solutionCount;
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

void testfn5()
{
	char board[n][n];
	BLOCKDATATYPE b;
	b.mBlockCount = 4;
	b.mMiddle = 2;
	b.mLeftScore = 0;
	b.mRightScore = 0;
	memset(&b.mBlocks[0],0,sizeof(b.mBlocks));
	b.mBlocks[0] = 1;
	b.mBlocks[1] = 2;
	b.mBlocks[2] = 2;
	b.mBlocks[3] = 3;
	b.mBlocks[4] = 0;
	
	memset( &board[0][0], '.', n*n*sizeof(char) );

	int colCount = b.mBlockCount;
	
	int maxHeight = 1;
	for(int c=0; c<colCount; ++c)
		maxHeight += b.mBlocks[c]-1;

	for(int c=0; c<colCount; ++c)
	{
		int blockCount = b.mBlocks[c];

		if ( c == b.mMiddle )
		{
			// NB Sets up middle, which is different as bottom square
			// is fixed. (or could ignore and have this come out when we
			// check solution validity?)
			// for other rows loop from 0 and use n-blockCount
			board[c][0] = 'X';
			for(int i=1; i<maxHeight;++i)
			{
				if ( i<maxHeight-blockCount+1 )
					board[c][i] = '.';
				else
					board[c][i] = 'X';
			}
		}
		else
		{
			// for other rows loop from 0 and use n-blockCount
			for(int i=0; i<maxHeight;++i)
			{
				if ( i<maxHeight-blockCount )
					board[c][i] = '.';
				else
					board[c][i] = 'X';
			}
		}
	}

	showBoard(board, colCount);
	int incCount = 0;
	while (incrementBoard(board, maxHeight, b, colCount))
	{
		++incCount;
		//showBoard(board, colCount);
	}
	cout << "incCount=" << incCount << endl;
}

// TESTING below here

int bitcount (unsigned int n) {
   int count = 0;
   while (n) {
      count += n & 0x1u;
      n >>= 1;
   }
   return count;
}

int bits[16][0x10000];
int twoPow[16];

bool checkBitBoard(BLOCKDATATYPE b, int boardPerm[], int maxHeight)
{
	char board[n][n];

	// Convert the board to chars
	for( int i=0; i<b.mBlockCount; ++i)
	{
		long rowBits = bits[ b.mBlocks[i]-1 ][ boardPerm[i] ];
		for(int j=0;j<maxHeight;++j)
		{
			if (_bittest(&rowBits, j))
				board[i][j] = 'X';
			else
				board[i][j] = '.';
		}
	}
	return NEWcheckBoard(&board[0][0], b.mBlockCount, b.mMiddle, maxHeight);
}

void testfn7()
{
	char board[18][18];
	BLOCKDATATYPE b;
	b.mBlockCount = 4;
	b.mMiddle = 2;
	b.mLeftScore = 0;
	b.mRightScore = 0;
	memset(&b.mBlocks[0],0,sizeof(b.mBlocks));
	b.mBlocks[0] = 1;
	b.mBlocks[1] = 2;
	b.mBlocks[2] = 2;
	b.mBlocks[3] = 3;
	b.mBlocks[4] = 0;
	
	memset( &board[0][0], '.', 18*18*sizeof(char) );

	int colCount = b.mBlockCount;
	
	int maxHeight = 1;
	for(int c=0; c<colCount; ++c)
		maxHeight += b.mBlocks[c]-1;

	int boardPerm[18];
	memset(boardPerm, 0, sizeof(boardPerm));

	bool finished = false;
	int pc = 0;
	while (finished == false)
	{
		++pc;

		// Do we need to check this solution?
		bool checkSol = true;
		// - not if middle isn't in place
		long middle = bits[ b.mBlocks[b.mMiddle]-1 ][ boardPerm[b.mMiddle] ];
		if ( _bittest(&middle, 0) == 0 )
		{
			//cout << "Middle not connected" << endl;
			checkSol = false;
		}
		
		// - if any rows & to zero, then they're not connected
		for( int i=0; i<b.mBlockCount-1;++i )
		{
			int rowAnd = bits[b.mBlocks[i]-1][boardPerm[i]] & bits[b.mBlocks[i+1]-1][boardPerm[i+1]];
			if (rowAnd == 0)
			{
				//cout << "Rows don't AND" << endl;
				checkSol = false;
				break;
			}
		}

		if (checkSol)
		{
			if (checkBitBoard(b, boardPerm, maxHeight))
			{
				// Print the board
				for( int i=0; i<b.mBlockCount; ++i)
				{
					long rowBits = bits[ b.mBlocks[i]-1 ][ boardPerm[i] ];
					for(int j=0;j<16;++j)
					{
						if (_bittest(&rowBits, j))
							cout << "X";
						else
							cout << ".";
					}
					cout << endl;
				}
				cout << "===================" << endl;
			}
		}

		// Increment the board
		int incCol = 0;
		++boardPerm[incCol];
		while ( bits[ b.mBlocks[incCol]-1 ][ boardPerm[incCol] ] >= twoPow[maxHeight] )
		{
			boardPerm[incCol] = 0;
			++incCol;
			if ( incCol >= b.mBlockCount )
			{
				finished = true;
				break;
			}
			++boardPerm[incCol];
		}
	}
	cout << "pc=" << pc << endl;
	
}


void testfn6()
{
	// Sets up our lookup table
	const int nbits = 16;
	memset(&bits[0][0],0,sizeof(bits));
	
	for(int a = 0; a < 16; ++a)
	{
		int nextPerm = 0;
		for(unsigned int i=0;i<0x10000;++i)
		{
			int bc = bitcount(i);
			if ( bc == a + 1)
			{
				bits[a][nextPerm++] = i;
			}
		}
		bits[a][nextPerm] = 0;
	}

	twoPow[0] = 1;
	for(int i=1;i<16;++i)
		twoPow[i] = twoPow[i-1]*2;

	cout << "DONE" << endl;

	// Some test printing
	for(int a=0;a<16;++a)
	{
		int count = 0;
		int i=0;
		 while (bits[a][i++] != 0)
			count++;
		
		cout << "a=" << a+1 << " count=" << count << endl;
	}

	testfn7();
}

int main()
{
	__int64 elapsedTime;
	
	testfn6();

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