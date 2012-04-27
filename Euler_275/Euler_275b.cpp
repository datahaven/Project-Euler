// Project Euler 275 (second attempt!)
//
// Adrian Dale 14/09/2010
#include <iostream>
#include <set>
#include <string>
#include <Windows.h> // For GetTickCount()

using namespace std;

// Could be a const in the final build
int n=4;

// Needs to be n*2 + 1
const int boardSize = 40;
int board[boardSize][boardSize];

// Up, right, down, left
int dirX[] = {0,1,0,-1};
int dirY[] = {-1,0,1,0};

set<string> sSolutions;

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
	int middle = boardSize / 2;
	//middle = 1; // TEST
	//for(int i=middle-n; i<1+middle; ++i )
	for(int i=middle;i>=middle-n; --i )
	{
		for(int j=middle-n;j<middle+n;++j)
		{
			// Make it easier to see symmetry
			if ( i==middle && j==middle &&  board[j][i] != -1)
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
				case 0:
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
	int middle = boardSize/2;
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

int callCount = 0;
void testfn5a(int depth, int x, int y, int move)
{
	// cout << "testfn5a(" << depth << ", " << x << ", " << y << ", " << move << ")" << endl;

	++callCount;
	
	// Do the move we've been given - we'll never be given a move
	// we can't do fully.
	for(int m=0;m<4;++m)
	{
		int dXY = moveType[move][m];
		if ( dXY != -1 )
		{
			int x2 = x+dirX[dXY];
			int y2 = y+dirY[dXY];
			board[x2][y2] = depth;
		}
	}

	// See if we've finished.
	if (depth == n-1)
	{
		//if ( isBalanced() )
		//{
		//	// TODO - check for duplicates here.
		//	//showBoard(0,false);
			++polyCount;
		//}
	}
	else
	{
		// For each populated neighbour, make all possible moves from there
		for(int dXY=0;dXY<4;++dXY)
		{
			int x2 = x+dirX[dXY];
			int y2 = y+dirY[dXY];
			if (board[x2][y2] != -1)
			{
				// Can we do move[m] from this position?
				for(int m=0;m<15;++m)
				{
					// Make sure we won't go too deep
					bool moveValid = moveSize[m] + depth < n;
					
					if ( moveValid )
					{
						// Make sure all move squares are available
						for(int i=0;i<4;++i)
						{
							int dXY2 = moveType[m][i];
							if ( dXY2 != -1 )
							{
								int x3 = x2+dirX[dXY2];
								int y3 = y2+dirY[dXY2];
								if ( board[x3][y3] != -1 )
									moveValid = false;
							}
						}
					}

					if (moveValid)
						testfn5a(depth + moveSize[m], x2, y2, m);
				}
			}
		}
	}

	// Undo the move we did at the start
	for(int m=0;m<4;++m)
	{
		int dXY = moveType[move][m];
		if ( dXY != -1 )
		{
			int x2 = x+dirX[dXY];
			int y2 = y+dirY[dXY];
			board[x2][y2] = -1;
		}
	}

	
}

void testfn5b(int depth, int x, int y, int move, int ls, int rs)
{
	//cout << "testfn5b(" << depth << ", " << x << ", " << y << ", " << move << ", " << ls << ", " << rs << ")" << endl;

	int middle = boardSize/2; // Any help having this global const?

	++callCount;
	
	// Do the move we've been given - we'll never be given a move
	// we can't do fully.
	for(int m=0;m<4;++m)
	{
		int dXY = moveType[move][m];
		if ( dXY != -1 )
		{
			int x2 = x+dirX[dXY];
			int y2 = y+dirY[dXY];
			board[x2][y2] = depth;

			// Update left and right scores to include new pieces
			if (x2 < middle)
				ls += middle - x2;
			else
				rs += x2 - middle;
		}
	}

	// Run a little "simulation" on ls and rs to see what the best
	// and worst cases could be. Use this to help us prune
	bool pruning = false;

	//int nlsl = ls; int nrsl = rs; // new_r/l_txl/r
	//int nlsr = ls; int nrsr = rs;

	//int txl = middle;// x;
	//int txr = middle;// x;
	//for(int i=0;i<n-depth-1;++i)
	//{
	//	// For each remaining piece to add to board
	//	// Best/worst left case
	//	--txl;
	//	if ( txl < middle )
	//		nlsl += middle - txl;
	//	else if ( txl > middle )
	//		nrsl += txl - middle;
	//	
	//	++txr;
	//	if ( txr < middle )
	//		nlsr += middle - txr;
	//	else if ( txr > middle )
	//		nrsr += txr - middle;
	//}
	//int lsmin = min(nlsl, nlsr);
	//int lsmax = max(nlsl, nlsr);
	//int rsmin = min(nrsl, nrsr);
	//int rsmax = max(nrsl, nrsr);
	//if ( depth < n-1 && (rsmax < lsmin || lsmax < rsmin ))
	//{
	//	cout << "PRUNING: " << lsmin << " " << lsmax << " : " << rsmin << " " << rsmax
	//		<< " d=" << depth << " (" << x << ", " << y << ")" << endl;
	//	showBoard(0,false);
	////	pruning = true;
	//	// TODO - This is currently a little TOO effective. Pruning things it shouldn't.
	//	// I think it needs to start the checks from existing leftmost/rightmost positions.
	//	// come back to this ...
	//}

	// TODO - some more pruning should be possible to remove sculptures where
	// number of squares on right > no on left, as we are guaranteed to duplicate these
	// Maybe only at final depth?
	// Also for duplicate checking we don't need to keep polys where the sum of the rhs
	// y coords is > sum of lhs ycoords.


	// See if we've finished.
	if (depth == n-1 || pruning)
	{
		if ( !pruning && ls == rs )
		{
			// Check for duplicates here.
			// Will probably need something better than this string representation
			// if we don't want to be too slow.
			string poly;
			
			for(int i=0;i<middle+1;++i)
				for(int j=0;j<boardSize;++j)
				{
					if (board[j][i] == -1)
						poly.append(".");
					else
						poly.append("X");					
				}
			pair<set<string>::iterator, bool> status;
			status = sSolutions.insert(poly);
			//cout << poly << endl;
			//showBoard(0,false);
			if ( status.second )
			{
				++polyCount;
				showBoard(0,false);
			}
			
		}
	}
	else
	{
		// For each populated neighbour, make all possible moves from there
		for(int dXY=0;dXY<4;++dXY)
		{
			int x2 = x+dirX[dXY];
			int y2 = y+dirY[dXY];
			if (board[x2][y2] != -1)
			{
				// Can we do move[m] from this position?
				for(int m=0;m<15;++m)
				{
					// Make sure we won't go too deep
					bool moveValid = moveSize[m] + depth < n;
					
					if ( moveValid )
					{
						// Make sure all move squares are available
						for(int i=0;i<4;++i)
						{
							int dXY2 = moveType[m][i];
							if ( dXY2 != -1 )
							{
								int x3 = x2+dirX[dXY2];
								int y3 = y2+dirY[dXY2];
								if ( board[x3][y3] != -1 )
									moveValid = false;
							}
						}
					}

					if (moveValid)
						testfn5b(depth + moveSize[m], x2, y2, m,ls,rs);
				}
			}
		}
	}

	// Undo the move we did at the start
	for(int m=0;m<4;++m)
	{
		int dXY = moveType[move][m];
		if ( dXY != -1 )
		{
			int x2 = x+dirX[dXY];
			int y2 = y+dirY[dXY];
			board[x2][y2] = -1;
		}
	}

	
}

// Passing in depth, (x,y), move, l/r scores, l/r y heights
void testfn5c(int depth, int x, int y, int move, int ls, int rs, int lh, int rh)
{
	/*cout << "testfn5b(" << depth << ", " << x << ", " << y << ", "
		<< move << ", " << ls << ", " << rs << ", " << lh << ", " << rh << ")" << endl;
*/

	// This function does NOT work. It doesn't produce all possible shapes when n=7
	// eg it misses:
	// x x
	// x x
	// xxx
	//  o 

	int middle = boardSize/2; // Any help having this global const?

	++callCount;
	
	// Do the move we've been given - we'll never be given a move
	// we can't do fully.
	for(int m=0;m<4;++m)
	{
		int dXY = moveType[move][m];
		if ( dXY != -1 )
		{
			int x2 = x+dirX[dXY];
			int y2 = y+dirY[dXY];
			board[x2][y2] = depth;

			// Update left and right scores to include new pieces
			if (x2 < middle)
				ls += middle - x2;
			else
				rs += x2 - middle;

			// Update left and right heights
			if (x2 < middle)
				lh += y2;
			else if ( x2 > middle )
				rh += y2;
		}
	}

	// Run a little "simulation" on ls and rs to see what the best
	// and worst cases could be. Use this to help us prune
	bool pruning = false;
	// NB Pruning code is in testfn5b but commented out as it doesn't work.
	
	// TODO - some more pruning should be possible to remove sculptures where
	// number of squares on right > no on left, as we are guaranteed to duplicate these
	// Maybe only at final depth?
	// Also for duplicate checking we don't need to keep polys where the sum of the rhs
	// y coords is > sum of lhs ycoords.


	// See if we've finished.
	if (depth == n-1 || pruning)
	{
		if ( !pruning && ls == rs && lh <= rh )
		{
			// Check for duplicates here.
			// Will probably need something better than this string representation
			// if we don't want to be too slow.
			string poly;
			
			for(int i=0;i<middle+1;++i)
				for(int j=0;j<boardSize;++j)
				{
					if (board[j][i] == -1)
						poly.append(".");
					else
						poly.append("X");					
				}
			pair<set<string>::iterator, bool> status;
			status = sSolutions.insert(poly);
			//cout << poly << endl;
			//showBoard(0,false);
			if ( status.second )
			{
				++polyCount;
				cout << "lh=" << lh << " rh=" << rh << endl;
				showBoard(0,false);
			}
		}
	}
	else
	{
		// For each populated neighbour, make all possible moves from there
		for(int dXY=0;dXY<4;++dXY)
		{
			int x2 = x+dirX[dXY];
			int y2 = y+dirY[dXY];
			if (board[x2][y2] != -1)
			{
				// Can we do move[m] from this position?
				for(int m=0;m<15;++m)
				{
					// Make sure we won't go too deep
					bool moveValid = moveSize[m] + depth < n;
					
					if ( moveValid )
					{
						// Make sure all move squares are available
						for(int i=0;i<4;++i)
						{
							int dXY2 = moveType[m][i];
							if ( dXY2 != -1 )
							{
								int x3 = x2+dirX[dXY2];
								int y3 = y2+dirY[dXY2];
								if ( board[x3][y3] != -1 )
									moveValid = false;
							}
						}
					}

					if (moveValid)
						testfn5c(depth + moveSize[m], x2, y2, m,ls,rs,lh,rh);
				}
			}
		}
	}

	// Undo the move we did at the start
	for(int m=0;m<4;++m)
	{
		int dXY = moveType[move][m];
		if ( dXY != -1 )
		{
			int x2 = x+dirX[dXY];
			int y2 = y+dirY[dXY];
			board[x2][y2] = -1;
		}
	}
}

// Do some set up for testfn5a
void testfn5()
{
	// initialise board
	for(int i=0;i<boardSize;++i)
		for(int j=0;j<boardSize;++j)
			board[i][j] = -1;

	// Fill in the plinth and lower
	for(int i=boardSize/2;i<boardSize;++i)
		for(int j=0;j<boardSize;++j)
			board[j][i] = 0;

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
	board[boardSize/2][boardSize/2] = 0;

	__int64 elapsedTime;
	elapsedTime = time_call([&] {
			testfn5c(1, boardSize/2, boardSize/2, 0, 0,0, 0,0);
		});

	//cout << "callCount=" << callCount << endl;
	cout << "n=" << n << " polyCount=" << polyCount << " time=" << elapsedTime << " ms" << endl;
}

int main()
{
	n = 8; // ie 7:  n in puzzle is 6
	//for(n=4;n<20;++n)
		testfn5();
	
	return 0;
}