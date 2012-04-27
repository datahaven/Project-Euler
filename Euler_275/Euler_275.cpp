// Project Euler 275 - Balanced Sculptures
// http://projecteuler.net/index.php?section=problems&id=275
// Adrian Dale 19/09/2010

// NB: This works but takes just over two hours to come up with an answer

/*
Based on the D. Hugh Redelmeier Paper "Counting Polyominos: Yet Another Attack"
http://andres.jaimes.net/files/2008/06/counting-polyominoes-yet-another-attack.pdf

I struggled to follow his algorithm description, or any of the even more vague
descriptions of it elsewhere on the web but was able to follow the far
superior instructions contained in Paul Church's thesis "Snakes In The Plane"
http://uwspace.uwaterloo.ca/bitstream/10012/3517/1/thesis-print-jan21.pdf

I modified the algorithm very slightly, removing the starting condition "all cells directly
to the left of the origin" to be marked as BLOCKED. (ie all cells where x<0 for y==0).
This causes the algorithm to produce two of each non-symmetrical polyomino. This is
necessary because the algorithm grows all polyominos from the origin, so the original
algorithm would create what we call "balanced sculptures", but in the wrong rotation.
The L shaped piece for n=6 has this problem, for example.

For each polyomino we find, we check if it is balanced, and if so, count it as a solution
if it is either symmetrical, or if it is the canonical version of the polyomino.
*/

#include <iostream>
#include <vector>
#include <set>
#include <Windows.h> // For GetTickCount
#include <algorithm>
using namespace std;

int n=18;

const int nmax = 36;

typedef pair<char,char> CRDTYPE;
typedef vector< CRDTYPE > POLYTYPE;
typedef set<CRDTYPE> UTLISTTYPE;

char gField[nmax][nmax];

__int64 gSolutionCount = 0;

// Up, right, down, left
const int dirX[] = {0,1,0,-1};
const int dirY[] = {1,0,-1,0};

void showPoly(POLYTYPE &p)
{
	char board[nmax][nmax];
	memset(board,'.', sizeof(board));
	for(POLYTYPE::iterator it = p.begin(); it != p.end(); ++it)
		board[it->first][it->second] = '#';

	for(int i=n*2; i>=0; --i)
	{
		for(int j=0; j<n*2;++j)
		{
			if ( i == n && j == n)
				cout << "o"; // origin
			else
				cout << board[j][i];
		}
		cout << endl;
	}
	cout << endl;
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

// Our initial starting conditions
void setup()
{	
	// Clear the field
	for(int j=0;j<nmax;++j)	
		for(int k=0;k<nmax;++k)
			gField[j][k] = '.';
			
	// Mark anything below the origin as B for Blocked
	// so that polyomino cells can't go there
	for(int j=0;j<nmax;++j)
		if ( j<n)
		{
			for(int k=0;k<nmax;++k)
				gField[k][j] = 'B';
		}

	gSolutionCount = 0;
}

// This checks for symmetry in the y-axis.
// Returns true if pp is the canonical
// version of the polyomino. 
bool isCanonical(POLYTYPE pp)
{
	// Sort the polyomino points otherwise we can't compare
	// properly against the mirror as points don't come in any
	// particular order. Using less than at the end doesn't work
	// properly if we omit the sorting steps.
	sort(pp.begin(), pp.end());

	// 2*n-x swaps x coords around y==n axis
	POLYTYPE mirrorPoly;
	for(POLYTYPE::iterator it = pp.begin(); it != pp.end(); ++it)
		mirrorPoly.push_back( make_pair(2*n - it->first, it->second) );
	sort(mirrorPoly.begin(), mirrorPoly.end());

	if ( mirrorPoly == pp ) // Symmetrical
		return true;
	return mirrorPoly < pp;
}

// Is it a valid balanced sculpture?
bool isSolution(POLYTYPE &pp)
{
	int ls = 0;
	int rs = 0;
	for(POLYTYPE::iterator it = pp.begin(); it != pp.end(); ++it)
	{
		// Add up the scores for the balance check
		if ( it->first < n ) {
			ls += n - it->first;
		}
		else if ( it->first > n ) {
			rs += it->first - n;
		}
	}
	// Balance check
	if ( ls != rs)
		return false;
	return isCanonical(pp);
}

// enumerateSculptures is a recursive function that produces all
// balanced sculptures. It is based on Paul Church's pseudo-code
// and comments are taken directly from Algorithm 2 in his paper
// "Snakes In The Plane".

/*
Algorithm 2 redelmeier( lattice, untriedList, depth )
if depth == number of cells in completed polyomino then
output completed polyomino
else
	for i = 1 to length of untriedList do
	c = untriedList[i]
	set c to OCCUPIED in lattice
	newUntriedList = EMPTY
	for each FREE cell d adjacent to cell c do
		set d to UNTRIED
		add d to newUntriedList
	end for
	childUntriedList = untriedList[i + 1 . . . end] [ newUntriedList
	redelmeier( lattice, childUntriedList, depth + 1 )
	set c to REACHABLE in lattice
	for each cell d in newUntriedList do
		set d to FREE
	end for
end for
for each cell c in untriedList do
set c to UNTRIED
end for
end if
*/

// My only real changes were to call isSolution to check the 
// final polyomino is balanced and to add the pp parameter so that
// we don't need to scan the lattice at the end to check
// whether or not the polyomino is balanced or canonical.
//
// I neglected to include any of the clever optimisations of the
// untried list and converting the recursive function to a stack
// based function. I'd hope they'd take a nice chunk out of our
// two hour plus running time, but have spent enough time on this
// project already without messing about even more.
//
// I also didn't check whether the untried list really needs to be
// a set, or if a vector would have sufficed.
void enumerateSculptures(POLYTYPE pp, UTLISTTYPE ut, int d)
{
	// if depth == number of cells in completed polyomino then
	if ( d == n )
	{
		// output completed polyomino if it is balanced
		if (isSolution(pp))
		{
			gSolutionCount++;
		//	showPoly(pp);
		}
		return;
	}
	// else
	// for i = 1 to length of untriedList do
	for( UTLISTTYPE::iterator it = ut.begin(); it != ut.end(); ++it )
	{
		// c = untriedList[i]
		CRDTYPE c = *it;
		// set c to OCCUPIED in lattice
		gField[c.first][c.second] = 'O';
		pp.push_back(c); // track our poly as it grows
		// newUntriedList = EMPTY
		UTLISTTYPE nut;
		// for each FREE cell d adjacent to cell c do
		for(int dXY=0;dXY<4;++dXY)
		{
			CRDTYPE d = make_pair(c.first+dirX[dXY], c.second+dirY[dXY]);
			if ( gField[d.first][d.second] == '.')
			{
				// set d to UNTRIED
				gField[d.first][d.second] = 'U';
				// add d to newUntriedList
				nut.insert(d);
			}
		}
		// childUntriedList = untriedList[i + 1 . . . end] UNION newUntriedList
		UTLISTTYPE cunt;
		UTLISTTYPE::iterator nit = it;
		if ( nit != ut.end() )
		{
			++nit;
			for( UTLISTTYPE::iterator clit = nit; clit != ut.end(); ++clit )
				cunt.insert(*clit);
		}
		for( UTLISTTYPE::iterator clit = nut.begin(); clit != nut.end(); ++clit )
			cunt.insert(*clit);
		// redelmeier( lattice, childUntriedList, depth + 1 )
		enumerateSculptures(pp, cunt, d+1);
		// set c to REACHABLE in lattice
		gField[c.first][c.second] = 'R';
		pp.pop_back(); // remove the just added cell from our poly record
		// for each cell d in newUntriedList do
		for( UTLISTTYPE::iterator clit = nut.begin(); clit != nut.end(); ++clit )
		{
			//set d to FREE
			gField[clit->first][clit->second] = '.';
		}
	}
	// for each cell c in untriedList do
	for( UTLISTTYPE::iterator clit = ut.begin(); clit != ut.end(); ++clit )
	{
		// set c to UNTRIED
		gField[clit->first][clit->second] = 'U';
	}
}

void GenerateForN()
{
	setup();
	// Fn starts with field empty.
	// Parent poly is empty and the untried set
	// contains the origin (n,n) 
	POLYTYPE p;
	CRDTYPE c;
	c.first = n;
	c.second = n;
	set<CRDTYPE> ut;
	ut.insert(c);

	__int64 elapsedTime = time_call([&] {
		enumerateSculptures(p, ut,0);
	});
	
	cout << "n=" << n << " : " << gSolutionCount << " solutions found in " << elapsedTime << " ms" << endl;
}

int main()
{
	GenerateForN();
	return true;
}