// Project Euler 393 - Ant Migration
// Adrian Dale 09/09/2012
// 
// Answer=112398351350823112 in 24.4 seconds
//
// Congratulations, the answer you gave to problem 393 is correct.
//
// You are the 64th person to have solved this problem.
#include <iostream>
#include <vector>
#include <map>
#include <ctime>
using namespace std;

typedef unsigned long long RowNumType;

typedef unsigned long long RowClassType;

/*const */int N = 10;

// Having this map from numbers to the bit representation
// of each piece allows us to step through them one by one
const int PC = 12; // Count of no. of pieces
int PieceMap[PC];

// Stores all possible rows that can come beneath RowNumType
map<RowNumType, vector<RowNumType> > PossibleRows;

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

	for(int i=0; i<PC; ++i)
		cout << "Piece " << i << " => " << PieceMap[i] << endl;
}

bool isValidPiece(int p)
{
	for(int i=0; i<PC; ++i)
		if (PieceMap[i] == p)
			return true;
	return false;
}

long long ValidRowCount = 0;

void printRow(RowNumType r)
{
	for(int i=0; i<N; ++i)
	{
		int piece = (r >> (i*4)) & 0xf;
		//if ( (piece & 3) == 0 ) cout << ".1.";
		//else if ( ((piece >> 2) & 3) ==  0 ) cout << ".0.";
		//else cout << "...";
		cout << piece;
		cout << " ";
	}
	cout << r << endl;
	
}

// classifyRow takes the row and returns a single int no. which
// has pairs of bits in order 
// 00 == blank
// 01 == In
// 10 == Out
RowClassType classifyRow( RowNumType row, bool forTop = true )
{
	RowClassType out = 0ull;
	for(int i=0; i<N; ++i)
	{
		RowClassType outType = 0ull;
		if ( (row & 3) == 0 ) outType = 1ull;
		if ( ((row >> 2) & 3) == 0 ) outType = 2ull;
		out |= (outType << (i*2));
		row >>= 4;	
	}
	return out;
}

void generateRow(vector<RowNumType> &Results, RowNumType RowAbove, RowNumType RowSoFar, int RowPos, int RowNo)
{
	if (RowPos == N)
	{
		Results.push_back(RowSoFar);
		++ValidRowCount;
		return;
	}
	// generateRow for each possible next brick in row
	for(int i=0; i<PC; ++i)
	{
		int piece = PieceMap[i];
		int pieceAbove = (RowAbove >> (4ull*RowPos)) & 0x0full;
		int pieceBefore = 2; // A valid left hand border piece
		if (RowPos > 0) // or find actual left hand border piece
			pieceBefore = (RowSoFar >> ((RowPos-1ull)*4ull)) & 0x0full;

		/*
		// assertions
		if (isValidPiece(pieceBefore)==false || isValidPiece(pieceAbove)==false)
		{
			// Used this to catch a bug when RowNumType was going above 32 bits
			cout << "ERROR! " << pieceBefore << " " << pieceAbove << " " << RowSoFar << " " << RowPos << endl;
		}
		*/

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
		// This cast really does need to be there, otherwise piece is an int
		// and gets shifted off the end of the int.
		// TODO - would be interesting to see if you get this behaviour with char to int
		nextRow |= (static_cast<RowNumType>(piece) << (RowPos*4));

		generateRow(Results, RowAbove, nextRow, RowPos+1, RowNo);
	}
}

// Dynamic Programming version of generateRow that uses previously stored results
void generateRowDP(vector<RowNumType> &Results, RowNumType RowAbove, RowNumType RowSoFar, int RowPos, int RowNo)
{
	auto existingRowIt = PossibleRows.find(RowAbove);
	if (existingRowIt != PossibleRows.end() && RowNo < N-1 && RowPos==0)
	{
		// We've been asked about this parent row before
		// and we're not on the last row, which is a special case that needs checking against the bottom edge
		Results = existingRowIt->second;
		return;
	}

	if (RowPos == N)
	{
		Results.push_back(RowSoFar);
		++ValidRowCount;
		return;
	}
	// generateRow for each possible next brick in row
	for(int i=0; i<PC; ++i)
	{
		int piece = PieceMap[i];
		int pieceAbove = (RowAbove >> (4ull*RowPos)) & 0x0full;
		int pieceBefore = 2; // A valid left hand border piece
		if (RowPos > 0) // or find actual left hand border piece
			pieceBefore = (RowSoFar >> ((RowPos-1ull)*4ull)) & 0x0full;

		/*
		// assertions
		if (isValidPiece(pieceBefore)==false || isValidPiece(pieceAbove)==false)
		{
			// Used this to catch a bug when RowNumType was going above 32 bits
			cout << "ERROR! " << pieceBefore << " " << pieceAbove << " " << RowSoFar << " " << RowPos << endl;
		}
		*/

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
		// This cast really does need to be there, otherwise piece is an int
		// and gets shifted off the end of the int.
		// TODO - would be interesting to see if you get this behaviour with char to int
		nextRow |= (static_cast<RowNumType>(piece) << (RowPos*4));

		generateRowDP(Results, RowAbove, nextRow, RowPos+1, RowNo);
	}
}

// Test code to see how many possible rows there are, regardless of what goes above
// or below. Only constraint is that sides are flat and pieces fit eachother
void generateAllRows(vector<RowNumType> &Results, RowNumType RowSoFar, int RowPos)
{
	if (RowPos == N)
	{
		Results.push_back(RowSoFar);
		++ValidRowCount;
		return;
	}
	// generateRow for each possible next brick in row
	for(int i=0; i<PC; ++i)
	{
		int piece = PieceMap[i];
		
		int pieceBefore = 2; // A valid left hand border piece
		if (RowPos > 0) // or find actual left hand border piece
			pieceBefore = (RowSoFar >> ((RowPos-1ull)*4ull)) & 0x0full;

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

		RowNumType nextRow = RowSoFar;
		// This cast really does need to be there, otherwise piece is an int
		// and gets shifted off the end of the int.
		// TODO - would be interesting to see if you get this behaviour with char to int
		nextRow |= (static_cast<RowNumType>(piece) << (RowPos*4));

		generateAllRows(Results, nextRow, RowPos+1);
	}
}

// This function works for N=2-6, but takes a LONG time
// for N=8-10, which is pretty much what I'd expected.
// I need a way to make this a Dynamic Programming problem
unsigned long long solCount = 0;
void solve(int RowNo, RowNumType RowAbove)
{
	if (RowNo == N)
	{
		++solCount;
		return;
	}

	// For each possible new row given RowAbove
	vector<RowNumType> results;
	generateRow(results, RowAbove, 0, 0, RowNo);
	for(auto it=results.begin(); it!= results.end(); ++it)
	{
		solve(RowNo+1, *it);
	}
}



bool insertResults(RowNumType parentRow, vector<RowNumType> &Results)
{
	if (PossibleRows.find(parentRow) == PossibleRows.end())
	{
		PossibleRows.insert( pair<RowNumType, vector<RowNumType> >(parentRow, Results));
		return false;
	}
	
	return true; // Result already in our map
}

// Works but still too slow!
unsigned long long solveDP(int RowNo, RowNumType RowAbove)
{
	// See if we've seen a call to solveDP with these params before
	map<RowNumType, unsigned long long> *prevResMap = Memo[RowNo];

	if (prevResMap->find(RowAbove) != prevResMap->end())
		return prevResMap->find(RowAbove)->second;

	// cout << RowNo << " -> " << RowAbove << endl;
	unsigned long long resultsSum = 0;
	if (RowNo == N)
	{
		resultsSum = 1;
	}
	else
	{

		// For each possible new row given RowAbove
		vector<RowNumType> results;
		generateRowDP(results, RowAbove, 0, 0, RowNo);
		if (PossibleRows.find(RowAbove) == PossibleRows.end() && RowNo < N-1)
		{
			insertResults(RowAbove, results);
		}

		for(auto it=results.begin(); it!= results.end(); ++it)
		{
			resultsSum += solveDP(RowNo+1, *it);
		}
	}

	// Add this result to our Memo so we don't have to calculate it again
	prevResMap->insert( pair<RowNumType,unsigned long long>(RowAbove, resultsSum) );
	return resultsSum;
}

// Stack overflow when N==10
void test01_Recurse(RowNumType parentRow)
{
	if (PossibleRows.find(parentRow) != PossibleRows.end())
	{
		return;
	}

	vector<RowNumType> RowResults;
	generateRow(RowResults, parentRow, 0ull, 0, 0);
	insertResults(parentRow, RowResults);

	for( auto it=RowResults.begin(); it!=RowResults.end(); ++it)
	{
		if (PossibleRows.find(*it) == PossibleRows.end())
		{
			test01_Recurse(*it);
		}
	}
}

void test01()
{
	clock_t start = clock();

	RowNumType topRow=0;
	// Create a valid topRow
	for(unsigned long long i=0; i<N; ++i)
		topRow |= (1ull << (i*4ull));
	cout << "TopRow=" << topRow << endl;

	ValidRowCount = 0;
	vector<RowNumType> RowResults;
	generateRow(RowResults, topRow, 0ull, 0, 0);
	cout << "ValidRowCount=" << ValidRowCount << endl;

	PossibleRows.clear();
	test01_Recurse(topRow);

	for(auto it=PossibleRows.begin(); it != PossibleRows.end(); ++it)
	{
		//cout << it->first << " has row count = " << (it->second).size() << endl;
	}

	cout << "There are " << PossibleRows.size() << " entries in map" << endl;
	clock_t finish = clock();
	cout << "Run time=" << ((float)finish/CLOCKS_PER_SEC) << endl;
}

void test02()
{
	
		clock_t start = clock();

		RowNumType topRow=0;
		// Create a valid topRow
		for(int i=0; i<N; ++i)
			topRow |= (1ull << (i*4));
		solCount = 0;
		//PossibleRows.clear();

		// Set up an empty Memo
		for(int i=0; i<N+1; ++i)
		{
			map<RowNumType, unsigned long long> *pEmptyResultMap = new map<RowNumType, unsigned long long> ;
			Memo.push_back( pEmptyResultMap );
		}

		unsigned long long resSum = solveDP(0, topRow);
		clock_t finish = clock() - start;
		cout << "n=" << N << " resSum=" << resSum << " time=" << ((float)finish/CLOCKS_PER_SEC) << endl;

		// A bit of memory management wouldn't hurt!
}

void test03()
{
	for(N=4; N<=4; N+=2)
	{
		vector<RowNumType> RowResults;
		generateAllRows(RowResults, 0ull, 0);
		for(auto it=RowResults.begin(); it!=RowResults.end(); ++it)
		{
			// printRow(*it);
			cout << "Row " << *it <<  " class = " << classifyRow(*it, true) << endl;
		}
		cout << "For N=" << N << " there are " << RowResults.size() << " rows" << endl;
	}
	/*
For N=2 there are 12 rows
For N=4 there are 216 rows
For N=6 there are 4104 rows
For N=8 there are 78408 rows
For N=10 there are 1498824 rows
*/
}

int main()
{
	generatePieces();
	
	test02();

	return 0;
}
