// Project Euler 393 - Ant Migration
// Adrian Dale 09/09/2012
// 
// Answer=112398351350823112 in 32 seconds
//
// Congratulations, the answer you gave to problem 393 is correct.
//
// You are the 64th person to have solved this problem.
//
// Possible Optimisations?
// There are only two possible top left corner pieces, so I could solve
// just for one piece, then double the final answer. This may half the
// final run time?
//
// Rather than memoing on the RowAbove, I could memo on the pattern of
// protrusions from the RowAbove. My previous tests showed that there are
// over a million possible rows when N=10, but a max of 3^10 possible
// protrusion patterns.
#include <iostream>
#include <vector>
#include <map>
#include <ctime>
using namespace std;

typedef unsigned long long RowNumType;

const int N = 10;

// Having this map from numbers to the bit representation
// of each piece allows us to step through them one by one
const int PC = 12; // Count of no. of pieces
int PieceMap[PC];

// Stores all possible rows that can come beneath RowNumType
// This is used like Memo below
map<RowNumType, vector<RowNumType> *> PossibleRows;

// Stores previously seen row results
vector< map<RowNumType, unsigned long long> *> Memo;

void generatePieces()
{
	int pn=0;
	for(int out=0; out<4; ++out)
	{
		for(int in=0; in<4; ++in)
		{
			if (out != in)
			{
				int pieceNo = (out << 2) | in;
				PieceMap[pn++] = pieceNo;
			}
		}
	}

	//for(int i=0; i<PC; ++i)
	//	cout << "Piece " << i << " => " << PieceMap[i] << endl;
}

// Dynamic Programming version of generateRow that keeps a cache of previously calculated results
// Generates all possible rows given the RowAbove
void generateRowDP(vector<RowNumType> **Results, RowNumType RowAbove, RowNumType RowSoFar, int RowPos, int RowNo)
{
	auto existingRowIt = PossibleRows.find(RowAbove);
	if (existingRowIt != PossibleRows.end() && RowNo < N-1 && RowPos==0)
	{
		// We've been asked about this parent row before
		// and we're not on the last row, which is a special case that needs checking against the bottom edge
		*Results = existingRowIt->second;
		return;
	}

	if (RowPos==0)
		*Results = new vector<RowNumType>();

	if (RowPos == N)
	{
		(*Results)->push_back(RowSoFar);
	}
	else
	{
		// generateRow for each possible next brick in row
		for(int i=0; i<PC; ++i)
		{
			int piece = PieceMap[i];
			int pieceAbove = (RowAbove >> (4ull*RowPos)) & 0x0full;
			int pieceBefore = 2; // A valid left hand border piece
			if (RowPos > 0) // or find actual left hand border piece
				pieceBefore = (RowSoFar >> ((RowPos-1ull)*4ull)) & 0x0full;

			// See if piece fits with pieceAbove and pieceBefore
			// - pieceAbove has out sticking down and piece doesn't have in
			if ((pieceAbove >> 2) == 2 && (piece & 3) != 0) continue;
			// - pieceAbove has in on lower side and piece doesn't have out on top
			if ((pieceAbove & 3) == 2 && (piece >> 2) != 0) continue;
			// - pieceAbove has blank lower surface and piece has out or in on top
			if ( ((pieceAbove >> 2) != 2 && (pieceAbove & 3) != 2) &&
				(((piece&3)==0) || (piece >> 2)==0) ) continue;
		
			// - pieceBefore has right sticking out and piece doesn't have in
			if ((pieceBefore>>2) == 1 && (piece & 3) != 3) continue;
			// - pieceBefore has right sticking in and piece doesn't have out
			if ((pieceBefore&3) == 1 && (piece >> 2) != 3) continue;
			// - pieceBefore has blank right and piece has out or in facing it
			if ( ((pieceBefore>>2) != 1 && (pieceBefore&3) != 1) &&
				(((piece&3)==3) || (piece>> 2)==3 )) continue;

			// If we're at right hand edge, need to check that, too
			if (RowPos == N-1)
			{
				// Skip if right hand edge of piece has an in or out
				if ( (piece>>2)==1 || (piece&3)==1) continue;
			}

			// If we're at botton row then piece must have flat bottom
			if (RowNo == N-1)
			{
				if ( (piece>>2)==2 || (piece&3)==2 ) continue;
			}

			RowNumType nextRow = RowSoFar;
			nextRow |= (static_cast<RowNumType>(piece) << (RowPos*4));

			generateRowDP(Results, RowAbove, nextRow, RowPos+1, RowNo);
		}
	}

	// Add results to our RowMemo - note that we can't Memo the results for the bottom row,
	// as the bottom edge affects the possibilities
	if (RowNo < N-1)
		PossibleRows.insert( pair<RowNumType, vector<RowNumType> *>(RowAbove, *Results) );
}

// Recursively solve row by row using Dynamic Programming
// ie storing previously seen results in a Memo
unsigned long long solveDP(int RowNo, RowNumType RowAbove)
{
	// See if we've seen a call to solveDP with these params before
	map<RowNumType, unsigned long long> *prevResMap = Memo[RowNo];

	if (prevResMap->find(RowAbove) != prevResMap->end())
		return prevResMap->find(RowAbove)->second;

	unsigned long long resultsSum = 0;
	if (RowNo == N)
		resultsSum = 1;
	else
	{
		// Sum results for each possible new row given RowAbove
		vector<RowNumType> *results = NULL;
		generateRowDP(&results, RowAbove, 0, 0, RowNo);
		
		for(auto it=results->begin(); it!= results->end(); ++it)
			resultsSum += solveDP(RowNo+1, *it);
	}

	// Add this result to our Memo so we don't have to calculate it again
	prevResMap->insert( pair<RowNumType,unsigned long long>(RowAbove, resultsSum) );
	return resultsSum;
}

int main()
{
	clock_t start = clock();

	generatePieces();
	
	// Create a valid topRow
	RowNumType topRow=0;
	for(int i=0; i<N; ++i)
		topRow |= (1ull << (i*4));
		
	// Set up an empty Memo
	for(int i=0; i<N+1; ++i)
	{
		map<RowNumType, unsigned long long> *pEmptyResultMap = new map<RowNumType, unsigned long long> ;
		Memo.push_back( pEmptyResultMap );
	}

	PossibleRows.clear();

	unsigned long long resSum = solveDP(0, topRow);

	clock_t finish = clock() - start;
	
	cout << "N=" << N << " result=" << resSum << " time=" << ((float)finish/CLOCKS_PER_SEC) << endl;

	// A bit of memory management to delete the Memo properly doesn't hurt!
	for(int i=0; i<N+1; ++i)
		delete Memo[i];
	// And all the PossibleRows
	for(auto it=PossibleRows.begin(); it!=PossibleRows.end(); ++it)
		delete it->second;
	
	return 0;
}
