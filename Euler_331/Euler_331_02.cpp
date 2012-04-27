// Project Euler 331 - Cross Flips
// Adrian Dale
// 28/05/2011
//
// A handy little tool someone's made:
// http://www.eelvex.net/games/pe-331-game/
// This was very useful info, too:
// http://www.jaapsch.net/puzzles/lomath.htm
// This discusses our exact problem:
// (I'd worked some of it out already - eg about even n)
// http://cmup.fc.up.pt/cmup/inigma/fun/jogos/jogo_das_luzes/jogo_das_luzes.pdf

#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include "next_combination.h"
using namespace std;

// Enough for an 8x8 board (64 bit)
typedef unsigned long long BoardPosType;

vector< vector<int> > gBoard;

vector< vector<int> > gMatrix;

// Adds row j onto row i
void rr(vector<vector<int> > &mat, int i, int j)
{
	for( int z=0; z<mat[0].size(); ++z)
		mat[i][z] = (mat[i][z] + mat[j][z]) % 2;
}

// Swaps row i and j
void swp(vector<vector<int> > &mat, int i, int j)
{
	vector<int> tmp = mat[i];
	mat[i] = mat[j];
	mat[j] = tmp;
}

void piv( vector<vector<int> > &mat, int i, int j)
{
	for( int u=0; u<mat.size(); ++u )
	{
		// subtract A[u,j] * row i from row u
		if ( mat[u][j] == 1 && u != i)
			for( int z=0; z<mat[0].size(); ++z)
				mat[u][z] = (mat[u][z] + mat[i][z]) % 2;
	}

	swp(mat,i,j);
}

// Simplified row reduction, modulo 2, for n=5
void rowreduce5( vector<vector<int> > &mat)
{
	piv(mat,20,0);
	piv(mat,21,1);
	piv(mat,22,2);
	piv(mat,23,3);
	piv(mat,24,4);
	piv(mat,24,5);
	piv(mat,24,6);
	piv(mat,7,7);
	piv(mat,24,8);

	piv(mat,24,10);
	piv(mat,24,11);
	piv(mat,24,12);
	piv(mat,24,13);

	piv(mat,24,15);
	piv(mat,24,16);
	piv(mat,24,17);
	piv(mat,24,18);

	// NB - solution didn't work if we swapped the zero rows to the bottom
	//swp(mat,9,10);
	//swp(mat,10,11);
	//swp(mat,11,12);
	//swp(mat,12,13);

	//swp(mat,13,15);
	//swp(mat,14,16);
	//swp(mat,15,17);
	//swp(mat,16,18);
}





// Simplified row reduction, modulo 2
void rowreduce( vector<vector<int> > &mat)
{
	// This part produces the lower reduced form
	int i = 0;
	int j = 0;
	while (i < mat.size() && j < mat[0].size())
	{
		// Find pivot in column j, starting in row i:
		int maxi = i;
		for( int k=i+1; k<mat.size(); ++k )
		{
			if ( mat[k][j] == 1 )
			{
				maxi = k;
				break;
			}
		}
		if ( mat[maxi][j] != 0 && maxi != i)
		{
			//swap rows i and maxi, but do not change the value of i
			//cout << "i=" << i << " maxi=" << maxi << endl;
			vector<int> temprow = mat[i];
			mat[i] = mat[maxi];
			mat[maxi] = temprow;
			// Now A[i,j] will contain the old value of A[maxi,j].

			for( int u=i+1; u<mat.size(); ++u )
			{
				// subtract A[u,j] * row i from row u
				if ( mat[u][j] == 1 )
					for( int z=0; z<mat[0].size(); ++z)
						mat[u][z] = (mat[u][z] + mat[i][z]) % 2;
			}
			++i;
		}
		++j;
	}

	// Now need to reduce the top half of the matrix
	i = mat.size()-1;
	j = i;
	while (i >= 0 && j >= 0)
	{
		// Find pivot pos
		int piv = 0;
		for( int k=0; k<mat[0].size(); ++k)
		{
			if ( mat[i][k]==1 )
			{
				piv = k;
				break;
			}
		}

		if (piv != 0)
		{
			// Perform substitution on all rows above
			for( int u=i-1; u>=0; --u )
			{
				// subtract A[u,j] * row i from row u
				if ( mat[u][piv] == 1 )
					for( int z=0; z<mat[0].size(); ++z)
						mat[u][z] = (mat[u][z] + mat[i][z]) % 2;
			}
		}
		--i;
		--j;
	}

}

// Simplified row reduction, modulo 2
void rowreduce7( vector<vector<int> > &mat)
{
	// This part produces the lower reduced form
	int i = 0;
	int j = 0;
	while (i < mat.size() && j < mat[0].size())
	{
		// Find pivot in column j, starting in row i:
		int maxi = i;
		for( int k=i+1; k<mat.size(); ++k )
		{
			if ( mat[k][j] == 1 )
			{
				maxi = k;
				break;
			}
		}
		if ( mat[maxi][j] != 0 && maxi != i)
		{
			//swap rows i and maxi, but do not change the value of i
			//cout << "i=" << i << " maxi=" << maxi << endl;
			vector<int> temprow = mat[i];
			mat[i] = mat[maxi];
			mat[maxi] = temprow;
			// Now A[i,j] will contain the old value of A[maxi,j].

			for( int u=i+1; u<mat.size(); ++u )
			{
				// subtract A[u,j] * row i from row u
				if ( mat[u][j] == 1 )
					for( int z=0; z<mat[0].size(); ++z)
						mat[u][z] = (mat[u][z] + mat[i][z]) % 2;
			}
			++i;
		}
		++j;
	}

	// Now need to reduce the top half of the matrix
	i = mat.size()-1;
	j = i;
	while (i >= 0 && j >= 0)
	{
		// Find pivot pos
		int piv = 0;
		for( int k=0; k<mat[0].size(); ++k)
		{
			if ( mat[i][k]==1 )
			{
				piv = k;
				break;
			}
		}

		if (piv != 0)
		{
			// Perform substitution on all rows above
			for( int u=i-1; u>=0; --u )
			{
				// subtract A[u,j] * row i from row u
				if ( mat[u][piv] == 1 )
					for( int z=0; z<mat[0].size(); ++z)
						mat[u][z] = (mat[u][z] + mat[i][z]) % 2;
			}
		}
		--i;
		--j;
	}


}

void buildmatrix(int n)
{
#if 0
	gMatrix.clear();
	//       no. of cols         no. of rows
	gMatrix.resize( n*n, vector<int>( n*n , 0 ) );

	// Builds the matrix of which square (i) is flipped
	// when (j) is flipped
	for( int i=0; i<n*n; ++i )
	{
		for( int j=0; j<n*n; ++j )
		{
			if ( i%n == j%n)
				gMatrix[i][j] = 1;
			if ( i/n == j/n)
				gMatrix[i][j] = 1;
		}
	}
#endif
	// As above but with the identity matrix on the RHS
	gMatrix.clear();
	//       no. of rows         no. of cols
	gMatrix.resize( n*n, vector<int>( 2*n*n , 0 ) );

	// Builds the matrix of which square (i) is flipped
	// when (j) is flipped
	for( int i=0; i<n*n; ++i )
	{
		for( int j=0; j<n*n; ++j )
		{
			if ( i%n == j%n)
				gMatrix[i][j] = 1;
			if ( i/n == j/n)
				gMatrix[i][j] = 1;
		}
		for( int j=0; j<n*n; ++j )
		{
			if ( j == i)
				gMatrix[i][(n*n)+j] = 1;
			
		}
	}


	//rowreduce( gMatrix );
	rowreduce7( gMatrix );

	for( int i=0; i<n*n; ++i )
	{
		for( int j=0; j<2*n*n; ++j )
		{
			if (j < (n*n))
				//cout << " ";
				cout << gMatrix[i][j];
		}
		cout << endl;
	}

}

void testreduce()
{
	gMatrix.clear();
	//       no. of rows         no. of cols
	gMatrix.resize( 5, vector<int>( 10 , 0 ) );

// 01101 10000
// 11100 01000
// 11011 00100
// 00111 00010
// 10110 00001
	gMatrix[0][1]=1;gMatrix[0][2]=1;gMatrix[0][4]=1; gMatrix[0][5]=1;
	gMatrix[1][0]=1;gMatrix[1][1]=1;gMatrix[1][2]=1; gMatrix[1][6]=1;
	gMatrix[2][0]=1;gMatrix[2][1]=1;gMatrix[2][3]=1;gMatrix[2][4]=1; gMatrix[2][7]=1;
	gMatrix[3][2]=1;gMatrix[3][3]=1;gMatrix[3][4]=1; gMatrix[3][8]=1;
	gMatrix[4][0]=1;gMatrix[4][2]=1;gMatrix[4][3]=1; gMatrix[4][9]=1;

	for( int i=0; i<gMatrix.size(); ++i )
	{
		for( int j=0; j<gMatrix[0].size(); ++j )
		{
			cout << gMatrix[i][j];
		}
		cout << endl;
	}

	rowreduce(gMatrix);
	cout << endl;

	for( int i=0; i<gMatrix.size(); ++i )
	{
		for( int j=0; j<gMatrix[0].size(); ++j )
		{
			cout << gMatrix[i][j];
		}
		cout << endl;
	}


}


inline BoardPosType setnthbit( BoardPosType bp, int bit )
{
	// cout << "setnthbit=" << bit << endl;
	return bp | (0x1ULL << bit);
}

inline BoardPosType clearnthbit( BoardPosType bp, int bit )
{
	return bp & ~(0x1ULL << bit);
}

// Returns a board position bitfield with the requested move made on it
BoardPosType makeMove( BoardPosType bp, int move, int n )
{
	int row = move / n;
	int col = move % n;
	//cout << "move=" << move << " in r,c=" << row << ", " << col << endl;
	for( int i=0; i<n; ++i)
	{
		// Flip the necessary bit in the affected row and column
		int colbit = (i*n)+col;
		if ( (bp & (1ULL<<colbit)) != 0 )
			bp = clearnthbit(bp, colbit);
		else
			bp = setnthbit(bp, colbit);

		int rowbit = (row*n)+i;
		if ( (bp & (1ULL<<rowbit)) != 0 )
			bp = clearnthbit(bp, rowbit);
		else
			bp = setnthbit(bp, rowbit);
	}
	// Un-flip the bit that is in the intersection of the row and column,
	// otherwise it gets flipped twice.
	int intbit = (row*n)+col;
	if ( (bp & (1ULL<<intbit)) != 0 )
		bp = clearnthbit(bp, intbit);
	else
		bp = setnthbit(bp, intbit);
	return bp;
}

void showBoard( BoardPosType b, int n )
{
	int i=0;
	do
	{
		if ( (b & (1ULL<<i)) != 0 )
			cout << "1";
		else
			cout << "0";
		++i;
		if ( i%n==0 )
			cout << endl;
	} while (i < n*n );
}

// Sets up a board of size n, so I can see what it looks like
BoardPosType startPos(int n)
{
	gBoard.clear();
	for( int i=0; i<n; ++i )
	{
		vector<int> row(n, 0);
		gBoard.push_back( row );
	}
	
	// Set up board according to rule
	for( int i=0; i<gBoard.size(); ++i)
	{
		for (int j=0; j<gBoard[0].size(); ++j)
		{
			double flip = sqrt((double)i*i+j*j);
			if ( flip >= n-1.0 && flip < n)
				gBoard[i][j] = 1;
		}
	}

	// Build a bitfield to represent the board
	BoardPosType bp = 0;
	for( int i=0; i<n; ++i)
	{
		for (int j=0; j<n; ++j)
		{
			if ( gBoard[i][j] == 1 )
				bp |= setnthbit(bp, i*n+j);
		}
	}
	//cout << "Board=" << bp << endl;
	//showBoard(bp, n);
	//cout << endl;
	//for( int i=gBoard.size()-1; i>=0; --i)
	//{
	//	for (int j=0; j<gBoard[0].size(); ++j)
	//		cout << gBoard[i][j];
	//	cout << endl;
	//}
	return bp;
}

// The different values for T() that we'll need
void test02()
{
	for( int i=3; i<=31; ++i)
	{
		double boardsize = pow(2.0, (double)i) - (double)i;
		cout << i << " " << boardsize << endl;
	}

}

bool boardSet(long long i, long long j, long long n)
{
	//double flip = sqrt((double)i*i+j*j);
	//if ( flip >= (double)(n-1LL) && flip < (double)n)
	//	return true;
	//return false;

	long long flip2 = i*i+j*j;
	if ( flip2 >= (n-1)*(n-1) && flip2 < (n*n) )
		return true;
	return false;
}

bool isSolvable(unsigned long long n)
{

	bool result = true;

	unsigned long long ones_count = 0;
	for( unsigned long long i=0; i<n; ++i )
	{
		if (boardSet(n-1,i,n)==true)
			++ones_count;
		else
			break;
		
	}
	cout << "oc=" << ones_count << endl;
	if (ones_count % 2 == 0)
		result = false;

	return result;

#if 0
	for( int i=0; i<n; ++i )
	{
		for( int j=0; j<n; ++j)
		{
			if (boardSet(i,j,n))
				cout << "1";
			else
				cout << "0";
		}
		cout << endl;
	}
	return true;
#endif
}

// The different values for T() that we'll need
void test03()
{
	// +3 for when i=3
	for( int i=4; i<=30; i+=2)
	{
		unsigned long long boardsize = (1ULL << i) - i;
		
			cout << i << " " << boardsize << " "
				<< (boardsize % 2 == 0 ? "even " : "odd ") << endl;
	}

}


// Pretends to be the inverse matrix for even numbered
// solutions - saves holding it all in memory when we know
// it has a regular pattern
inline int getevenmat(int i, int j, int n)
{
	if ((i%n == j%n) || (i/n == j/n) )
		return 1;
	return 0;
}

// Pretends to be the inverse matrix for odd numbered
// solutions - saves holding it all in memory when we know
// it has a regular pattern.
// IBBBBBC
// 0A0000A
// 00A000A
// ...
// 00000AA

inline int getoddmat(int i, int j, int n)
{
	// Block I
	if ( i<n && j<n )
		return i==j ? 1 : 0;
	// Block B
	if ( i<n && j<((n*n)-n))
		return j%n == n-1 ? 1 : 0;
	// Block A - diagonal
	if ( i>0 && j>0 && i<((n*n)-n) && j<((n*n)-n) && (j/n==i/n)  )
	{
		if (i==j && i%n != n-1)
			return 1;
		if (j%n == n-1 && i%n != n-1)
			return 1;
		return 0;
	}
	// Block A - vertical
	if ( i<((n*n)-n) && j >= ((n*n)-n) && i >=n )
	{
		if ( i%n == j%n && i%n != n-1)
			return 1;
		if ( j==(n*n)-1 && i%n != n-1 )
			return 1;
		return 0;
	}
	
	// Block C - flip 1s and 0s of A pattern
	if ( i<n && j >= ((n*n)-n) )
	{
		if ( i%n == j%n && i%n != n-1)
			return 0;
		if ( j==(n*n)-1 && i%n != n-1 )
			return 0;
		return 1;
	}

	// 0 Block
	return 0;
}

void test_getoddmat(int n)
{
	for( int i=0; i<n*n; ++i )
	{
		for( int j=0; j<n*n; ++j )
		{
			cout << getoddmat(i,j,n);
		}
		cout << endl;
	}
}

unsigned long long oddsolution(int n)
{
	unsigned long long result = 0L;

	// TODO - We'll need to get rid of this to handle big n.
	// Contains initial board
	vector<int> init(n*n, 0);
	// Set up board according to rule
	for( int i=0; i<n; ++i)
	{
		for (int j=0; j<n; ++j)
		{
			double flip = sqrt((double)i*i+j*j);
			if ( flip >= n-1.0 && flip < n)
				init[i*n+j] = 1;
		}
	}

	for( int i=0; i<n*n; ++i )
	{
		// for j=1 to n - but here n will be 1
		unsigned long long soli = 0;
		for( int k=0; k<n*n; ++k )
		{
			soli = (soli + getoddmat(i,k,n)*init[k]) % 2;
		}
		if ( soli==1 )
		{
			++result;
			cout << i << " ";
		}
	}
	cout << endl;
	
	return result;

}

unsigned long long evensolution(int n)
{
	// Will contain solution
	
	unsigned long long result = 0L;

	// TODO - We'll need to get rid of this to handle big n.
	// Contains initial board
	vector<int> init(n*n, 0);
	// Set up board according to rule
	for( int i=0; i<n; ++i)
	{
		for (int j=0; j<n; ++j)
		{
			double flip = sqrt((double)i*i+j*j);
			if ( flip >= n-1.0 && flip < n)
				init[i*n+j] = 1;
		}
	}

	// TODO - for big n, this double loop algorithm isn't
	// going to be fast enough.
	// Bigger fish to fry for now - the odd numbered n.
	for( int i=0; i<n*n; ++i )
	{
		// for j=1 to n - but here n will be 1
		unsigned long long soli = 0;
		for( int k=0; k<n*n; ++k )
		{
			soli = (soli + getevenmat(i,k,n)*init[k]) % 2;
		}
		if ( soli==1 )
		{
			++result;
			cout << i << " ";
		}
	}
	cout << endl;
	
	return result;
}

int main()
{
	
	int n = 9;
	buildmatrix(n);
	cout << "oddsolution(" << n << ")=" << oddsolution(n) << endl;

	BoardPosType bp = startPos(9);
	showBoard(bp, 9);

	//test_getoddmat(7);

	//testreduce();
	//n=10;
	//cout << "evensolution(" << n << ")=" << evensolution(n) << endl;
	test03();
	return 0;
}