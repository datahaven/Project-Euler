// Project_015.cpp
// Adrian Dale
//
// Find the number of possible paths from top left to
// bottom right of a nxn grid.
//
// I love the nice simple recursive solution but it
// is way too slow for 20x20 which is what we've been
// asked to find.
// Even with the nice use of symmetry to half the search
// space.
//
// I've left it running while I think about the next step.
// What I'm considering is using binomial coefficients to
// work out how many routes there are from each point on the
// main diagonal of the grid. 
// Needs a diagram to explain!
//
// This worked anyway ...
#include <iostream>

using namespace std;

int gridSize = 2;
__int64 SolCount = 0;

void FindNext( int x, int y )
{
	if ( x == gridSize && y == gridSize )
	{
		SolCount++;
		return;
	}

	if ( x < gridSize)
		FindNext(x+1,y);
	if ( y < gridSize)
		FindNext(x,y+1);
}

__int64 Fact(int n)
{
	__int64 ans = 1;
	for( int i=1; i<=n; ++i)
		ans *= i;
	return ans;
}

__int64 FindBinCoeft(int k)
{
	// Our NCR code where n is gridSize
	return Fact(gridSize-1) / (Fact(k)*Fact(gridSize-k-1));
}

 

int main( int argc, char **argv )
{
    int retstatus = 0;
	// TEST CODE
	// The BF Solution
	//FindNext(0,1);
	//cout << "For g=" << gridSize << " answer=" << 2 * SolCount << endl;

	for( int j=2; j<=20; ++j)
	{
		gridSize = j;
 
		// Counting solutions from the main diagonal
		unsigned __int64 FinalSolCount = 0;
		for( int i=0; i<gridSize; ++i )
		{
			int diagX = gridSize-1-i;
			int diagY = i;
			//cout << diagX << ", " << diagY << endl;
			SolCount = 0;
			FindNext(diagX, diagY);
			__int64 BinCoeft = FindBinCoeft(i);
			cout << BinCoeft * SolCount << endl;
			FinalSolCount += SolCount * BinCoeft;
		}

		cout << "For g=" << gridSize << " answer=" << FinalSolCount << endl;
	}

	return( retstatus );

}
