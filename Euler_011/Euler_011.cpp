// Project_011.cpp 
// Adrian Dale 10/01/2009
//
// What is the greatest product of four adjacent numbers 
// in any direction (up, down, left, right, or diagonally) in the 20×20 grid?
//
// Answer: 70600674
// (Got this wrong first time. I got the last diagonal back to front, so it was 
// checking the same direction as the one it had already checked. Doh!)

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;


int main(int argc, char* argv[])
{
	vector<vector<int>> grid;
	grid.resize(20);
	for( vector<vector<int>>::iterator it = grid.begin(); it != grid.end(); ++it )
		it->resize(20);

	// Read the file - note hack style no error checking and assuming input file is valid
	ifstream puzFile( "C:\\Documents and Settings\\Adrian\\My Documents\\Projects\\Puzzles\\ProjectEuler\\Project_011\\InputFile.txt" );
	for( int y=0; y<20; ++y )
		for( int x=0; x<20; ++x )
			puzFile >> grid[x][y];
	puzFile.close();

	int greatestProduct = 0;

	// Check horizontally
	for( int y=0; y<20; ++y )
		for( int x=0; x<17; ++x )
		{
			int product = grid[x][y] * grid[x+1][y] * grid[x+2][y] * grid[x+3][y];
			if ( product > greatestProduct )
				greatestProduct = product;
		}

	cout << "greatestProduct (so far) =" << greatestProduct << endl;

	// Check vertically - could combine with above by swapping x and y params over,
	// but doesn't save much at this scale compared to the loss of readibilty.
	for( int y=0; y<17; ++y )
		for( int x=0; x<20; ++x )
		{
			int product = grid[x][y] * grid[x][y+1] * grid[x][y+2] * grid[x][y+3];
			if ( product > greatestProduct )
				greatestProduct = product;
		}

	cout << "greatestProduct (so far) =" << greatestProduct << endl;

	// Check diagonally
	for( int y=0; y<17; ++y )
		for( int x=0; x<17; ++x )
		{
			int product = grid[x][y] * grid[x+1][y+1] * grid[x+2][y+2] * grid[x+3][y+3];
			if ( product > greatestProduct )
				greatestProduct = product;
		}

	cout << "greatestProduct (so far) =" << greatestProduct << endl;

	// Check diagonally - other way up
	for( int y=3; y<20; ++y )
		for( int x=0; x<17; ++x )
		{
			int product = grid[x][y] * grid[x+1][y-1] * grid[x+2][y-2] * grid[x+3][y-3];
			if ( product > greatestProduct )
				greatestProduct = product;
		}

	cout << "greatestProduct=" << greatestProduct << endl;

	return 0;
}