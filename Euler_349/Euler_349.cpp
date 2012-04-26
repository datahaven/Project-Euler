// Euler 349 - Langton's Ant
// by Adrian Dale
// 22/09/2011
//
// 115384615384614952
// You are the 345th person to have solved this problem.
//
// Calculates first 10500 steps of the ant's move, after which we expect it
// to have started to form a repeating cyclic highway pattern.
// We know the highway grows by 12 squares per 104 ant moves, so we divide to
// find out how many lots of 104 moves we'd do between 10500 and our target of
// 10^18, then we do the remaining steps again and see how many extra squares
// got added to the original amount.
// NB The "remaining steps" part of the calculation could be removed by
//    carefully choosing the number of steps the first simulation runs for,
//    but then the code would only work for our 10^18 value
//
// Helped significantly knowing that the cycle size was 104, starting around
// 10000, thanks to wikipedia
#include <iostream>
using namespace std;

// Only need a fairly small sized grid - determined approx size on previous runs
// of the code, noting min and max extents of the ant's movements.
// This code and grid bounds checking are removed from the final code to improve
// performance
#define XSIZE 128
#define YSIZE 128

#define WHITE 0
#define BLACK 1
typedef unsigned long long ull;

char grid[XSIZE][YSIZE];
int antx = XSIZE/2;
int anty = YSIZE/2;
int antd = 0;

// Do a single ant move, as per our instructions
void moveAnt()
{
	// Flip grid and update direction
	if (grid[antx][anty] == WHITE)
	{
		grid[antx][anty] = BLACK;
		antd = (antd+1)%4;
	}
	else
	{
		grid[antx][anty] = WHITE;
		antd = (antd+3)%4;
	}
	// Move forward
	switch(antd)
	{
	case 0:
		--anty;
		break;
	case 1:
		++antx;
		break;
	case 2:
		++anty;
		break;
	case 3:
		--antx;
		break;
	};
}

// Count up number of black squares on grid
ull countGrid()
{
	ull count = 0;
	for(int y=0; y<YSIZE; ++y)
		for(int x=0; x<XSIZE; ++x)
			count += grid[x][y];
		
	return count;
}

ull solve(ull target)
{
	memset(&grid[0][0], WHITE, XSIZE*YSIZE);
	
	for(int i=0; i<10500; ++i)
		moveAnt();
	
	ull baseCount = countGrid();
	ull remainingSteps = target-10500;
	ull cyclesLeft = remainingSteps / 104;
	ull remainingInCycle = remainingSteps - cyclesLeft*104;
	ull countSoFar = baseCount + cyclesLeft*12;

	for(int i=0; i<remainingInCycle; ++i)
		moveAnt();

	ull newBaseCount = countGrid();

	countSoFar += newBaseCount-baseCount;
	
	return countSoFar;
}

int main(int argc, char **argv)
{
	cout << "Euler 349 - Langton's Ant" << endl;
	cout << "Answer=" << solve(1000000000000000000) << endl;

	return 0;
}