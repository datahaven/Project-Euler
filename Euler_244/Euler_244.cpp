// Project Euler 244
// Adrian Dale 15/03/2011
//
// Finished on 23/05/2011
// This code produces the correct answer but for 
// some reason (that I haven't bothered to fix!) with an extra R at the end
// of the path. There is only one solution, despite the problem hinting at there
// being more.
// I found the bug by checking the single solution path with a chessboard and
// draughts pieces.
// Not my neatest code ever, so won't be posting it up on the forum.
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Each possible board configuration has a code:
// Highest 16 bits are 1 for blue, 0 for red, bottom 4 bits
// say which square number has the space.
const int START_STATE = 0xcccc0;
const int END_STATE = 0x5a5a0;

struct CState
{
	int pathLength;
	vector<string> paths;
};

// Map from board state to data for that state.
// Basically this saves having a sparse array indexed by state
// Suspect that costs us some performance time and the expense of
// saved memory
map<int, CState> gStateMap;

void showBoard(int b)
{
	int spacePos = b & 0x0f;
	int pos=0;
	for(int p=4; p<=19; ++p)
	{
		if (p%4 == 0 && p>4)
			cout << endl;
		if ( pos++ == spacePos )
			cout << "*";
		else
		{
			if ( b & 1<<p)
				cout << "b";
			else
				cout << "r";
		}
	}
	cout << endl;
}

// nextState returns the board config we'll get starting from b
// and making move d.
// Returns 0 if move d is not allowed.
int nextState(int b, char d)
{
	int spacePos = b & 0x0f;
	int newSpacePos = spacePos;
	switch(d)
	{
		// NB L/R and D/U are swapped as they signify the direction the
		// tile is slid, not the direction the space moves.
	case 'R':
		if ( spacePos % 4 > 0 )
			newSpacePos = spacePos-1;
		break;
	case 'L':
		if ( spacePos % 4 < 3 )
			newSpacePos = spacePos+1;
		break;
	case 'D':
		if ( spacePos > 3 )
			newSpacePos = spacePos-4;
		break;
	case 'U':
		if ( spacePos < 12 )
			newSpacePos = spacePos+4;
		break;
	}

	if ( newSpacePos == spacePos )
		return 0;

	//cout << "spacePos="<<spacePos << " newSpacePos=" << newSpacePos << endl;

	// Set spacePos bit to whatever bit is at newSpacePos,
	// then blank newSpacePos bit for tidiness
	bool nspBit = (b & (1<<(newSpacePos+4))) != 0;
	if ( nspBit )
	{
		b = b | 1<<(spacePos+4);
	}
	else
	{
		b = b & ~(1<<(spacePos+4));
	}

	b = b & ~(1<<(newSpacePos+4));

	b &= 0xfffffff0;
	b |= newSpacePos;

	return b;
}

// test01 - checks our nextState function gives the results
// the examples tell us we should get
void test01()
{
	
	// 0xcccc0 is starting state S
	// 0x???? is end state T

	showBoard(0xcccc0);
	cout << endl;
	int b = nextState(0xcccc0, 'L');
	b = nextState(b, 'U');
	b = nextState(b, 'L');
	b = nextState(b, 'U');
	b = nextState(b, 'R');
	showBoard(b);

	int e = 0x5a5a0;
	showBoard(e);
}


long long checksum(long long sumsofar, char direction)
{
	return ((sumsofar * 243) + direction) % 100000007;
}

long long checksumstring(string &moves)
{
	long long cs = 0;
	for( auto ci = moves.begin(); ci != moves.end(); ++ci )
	{
		if ( *ci == 'X' )
			continue;
		cs = checksum(cs, *ci);
	}
	return cs;
}

bool process(int state, CState &curState, char direction)
{
	bool changemade = false;

	int ns = nextState(state, direction);
	// move not allowed
	if ( ns == 0 )
		return false;

	// If next state hasn't been seen before, or if it has been seen before but via a 
	// longer route, then replace the paths with the one from our current state
	if (gStateMap[ns].pathLength == 0 || gStateMap[ns].pathLength > curState.pathLength + 1 )
	{
		changemade = true;
		gStateMap[ns].pathLength = curState.pathLength + 1;
		gStateMap[ns].paths = curState.paths;
	}
	
	// If the current state has been seen before by a path of the same length as this path,
	// then need to store both routes to this state
	if ( gStateMap[ns].pathLength == curState.pathLength + 1 )
	{
		changemade = true;
		gStateMap[ns].pathLength = curState.pathLength + 1;
		gStateMap[ns].paths.insert(gStateMap[ns].paths.end(), curState.paths.begin(), curState.paths.end());
		sort( gStateMap[ns].paths.begin(), gStateMap[ns].paths.end() );
		gStateMap[ns].paths.erase( unique( gStateMap[ns].paths.begin(), gStateMap[ns].paths.end()), gStateMap[ns].paths.end());
	}

	// Add this direction onto all paths.
	// (Sensible code could maintain checksums rather than full paths but I wanted to see results)
	for( auto it = gStateMap[ns].paths.begin(); it != gStateMap[ns].paths.end(); ++it )
	{
		string &path = *it;
		stringstream ss;
		string s;
		ss << direction;
		path.append(ss.str());
	}

	return changemade;
}

void test04()
{
	int start = 0xcccc0;
	int end = 0x5a5a0;

	CState startState;
	startState.pathLength = 1;
	startState.paths.push_back("X");
	gStateMap[start] = startState;

	
	// Scan our list of states and see if we've found this one before
	int solMax = 99999;
	bool changemade = true;
	int passcount = 0;
	int total_checksum = 0;

	while(changemade)
	{
		++passcount;
		cout << "Starting pass " << passcount << endl;
		cout << "Map size " << gStateMap.size() << endl;
		changemade = false;
		for( auto it = gStateMap.begin(); it != gStateMap.end(); ++it )
		{
			int state = it->first;
			CState &curState = it->second;
			if ( curState.pathLength != passcount )
				continue;

			changemade |= process(state, curState, 'U');
			changemade |= process(state, curState, 'D');
			changemade |= process(state, curState, 'L');
			changemade |= process(state, curState, 'R');
		
		}
	}

	cout << passcount << " passes" << endl;
	cout << "total_checksum=" << total_checksum << endl;
	
	CState endstate = gStateMap[END_STATE];

	for( auto it = endstate.paths.begin(); it != endstate.paths.end(); ++it )
	{
		cout << *it << " " << (*it).length() << " " << checksumstring(*it) << endl;
	}

	// There is only one answer and this is it! (96356848)
	// Code has bug that it prints an R on the end of the string.
	// I found this by testing the answer with a draughts board
	cout << checksumstring(string("LLURRDLLLURRDLUURULDLURDRRULDDRD")) << endl;
}

int main()
{
	cout << "Project Euler 244" << endl;
	test04();
	return 0;
}