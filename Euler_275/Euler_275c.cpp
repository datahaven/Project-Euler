// Project Euler 275 (third attempt - OMG!)
//
// Adrian Dale 16/09/2010
#include <iostream>
#include <set>
#include <string>
#include <Windows.h> // For GetTickCount()

using namespace std;

// Could be a const in the final build
const int n=11;

const int boardSize = 1 + 2*n;
const int middle = boardSize / 2;

int board[boardSize][boardSize];

// Up, right, down, left
int dirX[] = {0,1,0,-1};
int dirY[] = {-1,0,1,0};

typedef struct POLYTYPE {
	// Don't need size as this will always be depth when searching
	// char mPolySize;
	char mPolyX[n]; // x,y coords
	char mPolyY[n];
};

set<string> sSolutions;

typedef pair<char,char> NEIGHBOURTYPE;

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

void showBoard(int depth, bool showNumbers)
{

	//middle = 1; // TEST
	//for(int i=middle-n; i<1+middle; ++i )
	for(int i=boardSize-1;i>=middle-1; --i )
	{
		for(int j=middle-n+1;j<middle+n;++j)
		{
			// Make it easier to see symmetry
			if ( i==middle-1 && j==middle &&  board[j][i] != -1)
			{
					cout << "o";
					continue;
			}

			if ( showNumbers )
				cout << board[j][i];
			else
			{
				switch(board[j][i])
				{
				case -1:
					cout << ".";
					break;
				case n+1:
					cout << "-";
					break;
				default:
					cout << "X";
					break;
				}
			}
		}
		cout << endl;
	}
	cout << endl << endl;
}


int polyCount = 0;

// Not needed by testfn5b
bool isBalanced()
{
	
	int ls = 0;
	int rs = 0;
	for(int i=0;i<middle;++i)
	{
		for(int j=0;j<boardSize;++j)
		{
			if ( board[j][i] != -1 )
			{
				if ( j<middle )
					ls += middle - j;
				else if ( j>middle )
					rs += j - middle;
			}
		}
	}
	return ls == rs;
}

int moveType[15][4];
int moveSize[15];

int maxlh = 0; // max vals
int maxlc = 0;
int maxls = 0;

int maxvlh = 0; // max vals for valid sols
int maxvlc = 0;
int maxvls = 0;

// Passing in depth, (x,y), parent, l/r scores, l/r y heights, l/r block counts
void testfn5d(int depth, int x, int y, POLYTYPE pp, 
	int ls, int rs, int lh, int rh, int lc, int rc)
{
	/*cout << "testfn5d(" << depth << ", " << x << ", " << y << ", "
		 << ls << ", " << rs << ", " << lh << ", " << rh 
		<< ", " << lc << ", " << rc << ")" << endl;*/

	// TESTING
	maxlh = max(maxlh, lh);
	maxlc = max(maxlc, lc);
	maxls = max(maxls, ls);

	// Do the move
	board[x][y] = depth;

	// Update left and right scores, heights and counts to include new piece
	if (x < middle)	{
		ls += middle - x;
		lh += y;
		++lc;
	}
	else if (x > middle) {
		rs += x - middle;
		rh += y;
		++rc;
	}	
	
	// Add move to next depth's parent poly
	pp.mPolyX[depth] = x;
	pp.mPolyY[depth] = y;


	// TODO - at this point we can do a little simulation using 
	// scores, heights and blocks to see if we can prune
	bool pruning = false;
	
	// doesn't work - needs correct values n/2 is 5 for n=11
	// Doesn't prune much but might if we allow for x,y
	// max blocks on any side for valid sculpture is n/2
	// Biggest lc/rc difference?
	//if ( depth < n-1 && (lc > n/2 || rc > n/2 ))
	//	pruning = true;

	// Prune Tests

	// Find min and max x coords of poly
	int minx = n+1;
	int maxx = 0;
	for( int p=0; p<=depth;++p )
	{
		minx = min(minx, pp.mPolyX[p]);
		maxx = max(maxx, pp.mPolyX[p]);
	}
	int lsmin = ls; int lsmax = ls;
	int rsmin = rs; int rsmax = rs;

	// Simulate each next possible square
	for(int i=0;i<n-depth;++i)
	{
		--minx; ++maxx;
		if ( minx < middle ) // Always true?
			lsmax += middle - minx;
		
		if ( maxx > middle ) // Always true?
			rsmax += maxx - middle;
	}
	if ( depth < n-1 && (rsmax < lsmin || lsmax < rsmin ))
		pruning = true;

	
	// See if we've finished.
	if (depth == n-1 || pruning)
	{
		// Special case when apparently symmetrical
		// but not necessarily symmetrical.
		// In which case do a more careful check
		if ( lh == rh && ls == rs)
		{
			// TODO - should not need to do this (slow) check if lc!=rc
			// Needs to combine wiht lh <= rh comparison

			// NB lh == rh doesn't necessarily mean sym
			bool sym = true;
			for(int i=middle;i<boardSize && sym;++i)
			{
				int l = middle; int r=middle;
				do
				{
					--l; ++r;
					if ( (board[l][i] == -1 && board[r][i] != -1) ||
						 (board[l][i] != -1 && board[r][i] == -1) )
					{
						sym = false;
						if ( board[l][i] == -1 )
							--rh;
						else
							--lh;
						break;
					}
				} while (sym && l>0);
			}
			if (lh != rh && lc != rc) {
				lh = lc; rh = rc;
			}
		}

		if ( !pruning && ls == rs && lh <= rh )
		{
		
			// Check for duplicates here.
			// Will probably need something better than this string representation
			// if we don't want to be too slow.
			string poly;
			
			for(int i=middle;i<boardSize;++i)
				for(int j=0;j<boardSize;++j)
				{
					if (board[j][i] == -1)
						poly.append(".");
					else
					{
						poly.append("X");
					}
				}
			pair<set<string>::iterator, bool> status;
			status = sSolutions.insert(poly);
			//cout << poly << endl;
			//showBoard(0,false);
			if ( status.second )
			{
				++polyCount;
				//showBoard(0,false);

				// TESTING
				maxvlh = max(maxvlh, lh);
				maxvlc = max(maxvlc, lc);
				maxvls = max(maxvls, ls);
			}
			
		}
		
	}
	else
	{
		// For each square in parentPoly, make all possible moves from there
		set<NEIGHBOURTYPE> nextMoves;
		for( int p=0; p<=depth;++p )
		{
			int px = pp.mPolyX[p];
			int py = pp.mPolyY[p];

			for(int dXY=0;dXY<4;++dXY)
			{
				int x2 = px+dirX[dXY];
				int y2 = py+dirY[dXY];
				if (board[x2][y2] == -1)
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
			// Don't go to right for first move - cuts down some symmetrical searches
			// Too many, unfortunately!
			//if ( !(depth == 0 && it->first == middle-1 && it->second == middle))
			//{
				testfn5d(depth+1, it->first, it->second, pp,ls,rs,lh,rh,lc,rc);
			//}
		}
	}

	// Undo the move we did at the start
	board[x][y] = -1;
}

// Do some set up for testfn5a
void testfn5()
{
	// initialise board
	for(int i=0;i<boardSize;++i)
		for(int j=0;j<boardSize;++j)
			board[i][j] = -1;

	// Fill in the plinth and lower
	for(int i=0;i<middle;++i)
		for(int j=0;j<boardSize;++j)
			board[j][i] = n+1;

	// initialise move type with all possible
	// combinations of directions we could move in
	for(int i=0;i<15;++i)
	{
		int ms = 0;
		for(int j=0; j<4;++j)
		{
			if ( i+1 & 1<<j )
			{
				moveType[i][j] = j;
				++ms;
			}
			else
				moveType[i][j] = -1;
		}

		// moveSize is how many moves we'll make at
		// once - ie how much depth will increase by
		moveSize[i] = ms;
	}

	// moveSize is how many moves we'll make at
	// once - ie how much depth will increase by

	/*for(int i=0;i<15;++i)
	{
		cout << i << ": ";
		for(int j=0; j<4;++j)
		{
			
			if (moveType[i][j] == -1)
				cout << "-" << " ";
			else
				cout << moveType[i][j] << " ";
		}
		cout << endl;
	}*/

	

	// Kick us off
	
	polyCount = 0;
	POLYTYPE parentPoly;
	memset(&parentPoly, 0, sizeof(POLYTYPE));
	
	//showBoard(0,false);

	__int64 elapsedTime;
	elapsedTime = time_call([&] {
			testfn5d(0, middle, middle, parentPoly, 0,0, 0,0, 0,0);
		});

	
	cout << " maxlh=" << maxlh  <<  " maxls=" << maxls  << " maxlc=" << maxlc << endl;
	cout << " maxvlh=" << maxvlh  <<  " maxvls=" << maxvls  << " maxvlc=" << maxvlc << endl;

	cout << "n=" << n << " polyCount=" << polyCount << " time=" << elapsedTime << " ms" << endl;
}

int main()
{
	
		testfn5();
	
	return 0;
}