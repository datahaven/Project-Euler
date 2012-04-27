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

const int n=10;

typedef struct BLOCKDATATYPE
{
	int mBlocks[n];
	int mWeight;
	int mBlockCount;
	int mMiddle;
};

typedef vector<BLOCKDATATYPE> BLOCKDATALISTTYPE;



int tryAllColPossibilities(int cols[], int middle, bool symmetryCheckNeeded);

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
		
		//for(int i=0;i<n;++i)
		//	cout << newBlock.mBlocks[i] << " ";
		//cout << "weight=" << newBlock.mWeight << endl;

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
	//cout << "fAP: ";

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
			//cout << itL->mBlocks[i] << " ";
		}
	}
	
	newBlock.mMiddle = colpos;
	newBlock.mBlocks[colpos++] = centrePillarHeight;
	//cout << " : " << centrePillarHeight << " : ";

	for(int i=0; i<n; ++i)
	{
		if ( itR->mBlocks[i] != 0 )
		{
			newBlock.mBlocks[colpos++] = itR->mBlocks[i];
			//cout << itR->mBlocks[i] << " ";
		}
	}
	//cout << endl;

	newBlock.mBlockCount = colpos;

	//cout << "fAP: ";
	//for(int i=0;i<colpos;++i)
	//	cout << columns[i] << " ";
	//cout << endl;

	// TODO - I'm not currently using symmetryCheckNeeded, but I think
	// it could still be worth adding back in.
	bool symmetryCheckNeeded = colpos-newBlock.mMiddle-1 == newBlock.mMiddle;

	vColumns.push_back(newBlock);

	return 0;
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
			}
			else
				++it2;
		}
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
		localSum.local() += tryAllColPossibilities(vColumns[i].mBlocks, vColumns[i].mMiddle, false);
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

bool checkBoard(char board[n][n], int colCount, int middle)
{
	// Sanity check before we go recursive - is the middle connected?
	// Possibly not worth doing this.
	if ( board[middle][1] == '.' && board[middle-1][0] == '.' && board[middle+1][0] == '.')
		return false;

	// Look for obvious shortcuts - eg if any square has no neighbours return false
	for(int i=0; i<colCount; ++i)
	{
		for(int j=0; j<n;++j)
		{
			if ( board[i][j] == 'X' )
			{
				if ( i>0 && board[i-1][j] == 'X')
					break;
				if ( i<colCount-1 && board[i+1][j] == 'X')
					break;
				if ( j>0 && board[i][j-1] == 'X')
					break;
				if ( j<n-1 && board[i][j+1] == 'X')
					break;
				return false;
			}
		}
	}

	int startX = middle;
	int startY = 0;

	markBoard(&board[0][0], colCount, startX, startY);

	// Scan board and see if there are any squares we didn't visit
	bool result = true;
	for(int i=0; i<colCount; ++i)
	{
		for(int j=0; j<n;++j)
		{
			if (board[i][j] == 'X')
				result = false;
			// Put board back the way we found it
			if (board[i][j] == 'V')
				board[i][j] = 'X';
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

// Global version - works, but messes up our ability to multi-thread.
// Turns out the current MT implementation didn't help much anyway for n=10.
// Weird!
//vector<SOLTYPE> boardList;  

int tryAllColPossibilities(int cols[], int middle, bool symmetryCheckNeeded)
{
	char board[n][n];
	memset( &board[0][0], '.', n*n*sizeof(char) );

	int colCount;
	for(colCount = 0;colCount<n;++colCount)
	{
		if (cols[colCount] == 0)
			break;
	}
	
	// Initialise each column on our board
	for(int c=0; c<colCount; ++c)
	{
		int blockCount = cols[c];

		if ( c == middle )
		{
			// NB Sets up middle, which is different as bottom square
			// is fixed. (or could ignore and have this come out when we
			// check solution validity?)
			// for other rows loop from 0 and use n-blockCount
			board[c][0] = 'X';
			for(int i=1; i<n;++i)
			{
				if ( i<n-blockCount+1 )
					board[c][i] = '.';
				else
					board[c][i] = 'X';
			}
		}
		else
		{
			// for other rows loop from 0 and use n-blockCount
			for(int i=0; i<n;++i)
			{
				if ( i<n-blockCount )
					board[c][i] = '.';
				else
					board[c][i] = 'X';
			}
		}
	}


	int solutionCount = 0;
	bool running = true;
	vector<int> checkSumList;
//	vector<SOLTYPE> localboardList; // local version
	vector<SOLTYPE> boardList;
	int incCol = 0;
	int firstIncCol = 0;
	while(running)
	{

		// See if all of our columns join up.
		// If so, then we have a solution
		//if ( checkBoard(board, colCount) )
		//{
		//	if ( symmetryCheckNeeded )
		//	{
		//		// Calculate a checksum to see if we've already got this solution, or
		//		// its mirror
		//		int checksum;
		//		int revsum;
		//		getChecksums(board, middle, checksum, revsum);
	
		//		//cout << "cs=" << checksum << " rcs=" << revsum << endl;
		//		//showBoard(board, colCount);

		//		if ( find( checkSumList.begin(), checkSumList.end(), checksum ) == checkSumList.end() &&
		//			find( checkSumList.begin(), checkSumList.end(), revsum ) == checkSumList.end() )
		//		{
		//			checkSumList.push_back(checksum);
		//			++solutionCount;
		//			//showBoard(board, colCount);
		//		}
		//	}
		//	else
		//	{
		//		++solutionCount;
		//		//showBoard(board, colCount);
		//	}
		//}

		// Above code, only storing a copy of entire board, instead of using checksum,
		// which appears to be a shade unreliable
		if ( checkBoard(board, colCount, middle) )
		{
			SOLTYPE ourBoard;
			memcpy(ourBoard.mBoard, board, n*n*sizeof(char));

			// Mirror board
			SOLTYPE mirrorBoard;
			memset(mirrorBoard.mBoard, '.', sizeof(char)*n*n);
			int i = middle;
			int j = middle;
			while(i >= 0)
			{
				for(int k=0; k<n;++k)
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
				for(j=0;j<n;++j)
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
				for(j=0;j<n;++j)
				{
					mirrorBoard.mBoard[i][j] = mirrorBoard.mBoard[firstPopulatedCol + i][j];
				}
			}
			// Blank out the final copied rows - do we ever need this?
			for(i=n-firstPopulatedCol;i<n;++i) 
			{
				for(j=0;j<n;++j)
				{
					mirrorBoard.mBoard[i][j] = '.';
				}
			}

			/*cout << "MIRROR PAIR:"<<endl;
			showBoard(ourBoard.mBoard, colCount);
			cout << "---------------------------" << endl;
			showBoard(mirrorBoard.mBoard, colCount);
			if ( ourBoard == mirrorBoard )
				cout << "symmetrical" <<endl;
			else
				cout << "non-symmetrical" <<endl;*/

			if ( find( boardList.begin(), boardList.end(), ourBoard ) == boardList.end() &&
				find( boardList.begin(), boardList.end(), mirrorBoard ) == boardList.end() )
			{
				boardList.push_back(ourBoard);
				//boardList.push_back(mirrorBoard);
				//showBoard(ourBoard.mBoard, colCount);
				++solutionCount;
			}
		}


		

		// Increment solution
		
		bool carry = false;
		int incCol = 0;
		do
		{
			if (incCol == middle )
				carry = next_permutation(&board[incCol][1], &board[incCol+1][0]);
			else
				carry = next_permutation(&board[incCol][0], &board[incCol+1][0]);

			++incCol;

			if ( !carry && incCol >= colCount )
				running = false;
		}
		while( !carry && running );
		
		
		
		//showBoard(board, colCount);
	}
	cout << "Final sc=" << solutionCount << endl;

#if 0
	int combCount = 0;
	do
	{
		++combCount;
		for(int i=0; i<n;++i)
		{	
			cout <<	board[middle][i];	
		}
		cout << endl;
	} while (next_permutation(&board[middle][0], &board[middle+1][0]));
	cout << "combCount=" << combCount << endl;
#endif
	return solutionCount;
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

int main()
{
	__int64 elapsedTime;
	
	elapsedTime = time_call([&] {
		sidesAndCentre();
	});
	cout << "Finished in " << elapsedTime << " ms" << endl;
	
	cout << endl << endl;
	
	//countSculptures(5,1,12);

	//int testCols[] = {2,1,1,1,1,10,2,0,0,0,0,0,0,0,0,0,0,0};
	//tryAllColPossibilities(testCols, 4);

	//testfn();

	return 0;

}