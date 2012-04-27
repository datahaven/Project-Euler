// Project Euler 331 - Cross Flips
// Adrian Dale
// 28/05/2011
//
// Congratulations, the answer you gave to problem 331 is correct.
//
// You are the 54th person to have solved this problem.
//
/*
n=4 T(12)= 31 t=727.956 microseconds
n=6 T(58)= 1193 t=704.978 microseconds
n=8 T(248)= 22385 t=845.778 microseconds
n=10 T(1014)= 372547 t=1.23053 milliseconds
n=12 T(4084)= 6275536 t=2.16578 milliseconds
n=14 T(16370)= 100990283 t=5.01356 milliseconds
n=16 T(65520)= 1630913781 t=16.6154 milliseconds
n=18 T(262126)= 26146650973 t=62.4869 milliseconds
n=20 T(1048556)= 418057047705 t=248.492 milliseconds
n=22 T(4194282)= 6683197830265 t=979.034 milliseconds
n=24 T(16777192)= 106934422355761 t=3.90398 seconds
n=26 T(67108838)= 1710850979291959 t=15.6364 seconds
n=28 T(268435428)= 27373815729676843 t=62.4967 seconds
n=30 T(1073741794)= 437979504875414284 t=251.551 seconds
Answer=467178235146843549
Time taken: 334.923 seconds
*/
// A handy little tool someone's made:
// http://www.eelvex.net/games/pe-331-game/
// This was very useful info, too:
// http://www.jaapsch.net/puzzles/lomath.htm
// This discusses our exact problem:
// http://cmup.fc.up.pt/cmup/inigma/fun/jogos/jogo_das_luzes/jogo_das_luzes.pdf

#include <iostream>
#include <cmath>
#include <boost/dynamic_bitset.hpp>

#include <sstream> // For timing functions
#include <windows.h>

using namespace std;
using namespace boost;

long long counter() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return li.QuadPart;
}

long long frequency() {
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
    return li.QuadPart;
}

string format_time(const long long start, const long long finish) {
    ostringstream oss;

    if ((finish - start) * 1000 < frequency()) {
        oss << (finish - start) * 1000000.0 / frequency() << " microseconds";
    } else if (finish - start < frequency()) {
        oss << (finish - start) * 1000.0 / frequency() << " milliseconds";
    } else {
        oss << (finish - start) * 1.0 / frequency() << " seconds";
    }

    return oss.str();
}

dynamic_bitset<> odd;

typedef unsigned long long ull;

// Cached values of n^2 and (n-1)^2
// used by circleColStartAndEnd and isBoardSet
ull ns;
ull n1s;

// Returns 1 if disk at i,j starts off black
// on an n*n board
inline int isBoardSet(ull i, ull j, ull n)
{
	long long flip2 = i*i+j*j;
	if ( flip2 >= n1s && flip2 < ns )
		return 1;
	return 0;
}

void circleColStartAndEnd(ull i, ull &lb, ull &ub, ull n)
{
	ull lb2 = n1s - i*i;
	ull ub2 = ns - i*i;
	
	lb = (ull)sqrt((double)lb2);
	ub = (ull)sqrt((double)ub2);

	// Fiddle factor to allow for sqrt double conversion inaccuracies
	if ( lb < 2 )
		lb=0;
	else
		lb-=2;
	if (ub+2 < n )
		ub += 2;
	else
		ub=n-1;

	for( ;lb < n; ++lb)
		if (isBoardSet(lb, i,n )==1)
			break;

	for( ;ub>=0; --ub )
		if (isBoardSet(ub,i,n)==1)
			break;
}

ull evensolution(ull n)
{
	ull result = 0ULL;

	// Set up the cached n^2 and (n-1)^2 values for circleColStartAndEnd
	ns = n*n;
	n1s = (n-1)*(n-1);

	// Store in odd whether row[i] has odd (==T) or even no. of ones set
	odd.resize(n, true);
	odd.set();
	
	size_t evencount = 0;
	for(ull i=0; i<n; ++i)
	{
		ull rs;
		ull re;
		circleColStartAndEnd(i,rs,re,n);
		if ( (re-rs+1) % 2 == 0 )
		{
			odd.reset(i);
			++evencount;
		}
	}
	
	size_t oddcount = n-evencount;
	
	ull prs = 0;
	ull pre = 0;
	ull prev_rc = 0;
	for(ull i=0; i<n; ++i)
	{
		ull rs;
		ull re;
		circleColStartAndEnd(i,rs,re,n);

		// So that we don't repeat the calculation if
		// the row is the same as the one above
		if ( rs == prs && re == pre )
		{
			result+=prev_rc;
			continue;
		}

		prs = rs; pre = re;

		ull rc;
		if (odd[i])
			rc = evencount;
		else
			rc = oddcount;
		// Only need to do the xor for bits that are set in
		// our row, as these are the only bits that can change
		// the count of bits set in the result of xoring
		// the entire row
		for(ull b=rs;b<=re;++b)
		{
			if (odd[i])
			{
				if (!odd[b])
					--rc;
				else
					++rc;
			}
			else
			{
				if (odd[b])
					--rc;
				else
					++rc;
			}			
		}

		result += rc;
		prev_rc = rc;
	}

	return result;
}

unsigned long long solveAllEven()
{
	// Include T(5)==3, no other odd n are solveable
	unsigned long long result = 3; 
	
	for( int n=4; n<=30; n+=2)
	{
		const long long start = counter();
		unsigned long long boardsize = (1ULL << n) - n;
		cout << "n=" << n << " T(" << boardsize << ")= ";
		unsigned long long solution_n = evensolution(boardsize);
		cout << solution_n;
		result += solution_n;
		const long long finish = counter();
		cout << " t=" << format_time(start, finish) << endl;
	}
	return result;
}

int main()
{
	const long long start = counter();

	cout << "Answer=" << solveAllEven() << endl;

	const long long finish = counter();

	cout << "Time taken: " << format_time(start, finish) << endl;
	return 0;
}