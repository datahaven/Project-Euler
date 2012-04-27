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
#include <cmath>
//#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <sstream>
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
dynamic_bitset<> even;
dynamic_bitset<> row;

//bitset<58> odd;
//bitset<58> even;
//bitset<58> row;

typedef unsigned long long ull;
typedef long long ll;

// Returns 1 if disk at i,j starts off black
// on an n*n board
int isBoardSet(long long i, long long j, long long n)
{
	long long flip2 = i*i+j*j;
	if ( flip2 >= (n-1)*(n-1) && flip2 < (n*n) )
		return 1;
	return 0;
}

// NB fn only tested up to i==18
// circleColStart returns a long to say which value of
// j the ith row starts in
ull circleColStart(ull i, ull n)
{
	//if ( i==n-1 )
	//	return 0ULL;
	ull j21 = (n*n)-(i*i);
	ull j22 = ((n-1)*(n-1))-(i*i);
	
	ull j = (unsigned long long)sqrt((double)j22);
	// Allowing for floating point rounding errors when j22 is big.
	// Not sure if this is needed
	if ( j > 5 )
		j -=5;
	else
		j=0;

	for(; j<n; ++j)
	{
		if (isBoardSet(i,j,n)==1)
		{
			return j;
		}
	}

	return 0ULL;
}

// Pretends to be the inverse matrix for even numbered
// solutions - saves holding it all in memory when we know
// it has a regular pattern
inline unsigned long long getevenmat(unsigned long long i, unsigned long long j, unsigned long long n)
{
	if ((i%n == j%n) || (i/n == j/n) )
		return 1ULL;
	return 0ULL;
}


unsigned long long singlesquaresolution(ull row, ull col, unsigned long long n)
{
	// Will contain solution
	unsigned long long result = 0L;
	
	ull i = row * n + col;

		unsigned long long soli = 0;
#if 0
		for( unsigned long long k=0; k<n*n; ++k )
		{
			//soli = (soli + getevenmat(i,k,n)*init[k]) % 2;
			soli += getevenmat(i,k,n)*isBoardSet(k/n,k%n,n);
		}
#endif

		// This block saves us from doing the above loop by
		// stepping directly from each value of k
		// to the next.
		// Speeds the run time up but still not enough!
		unsigned long long kstart=0;
		if ( i/n != 0 )
			kstart = i%n;
	
		unsigned long long k=kstart;
		unsigned long long iblock = 0;
		unsigned long long jblock = i/n;
		while( k<n*n)
		{
			//cout << k << " ";
			soli += isBoardSet(k/n,k%n,n);

			iblock = k/n;
			if (iblock == jblock)
			{
				if (k%n == n-1)
					k += 1+kstart;
				else
					++k;
			}
			else
				k+=n;

			unsigned long long newblock = k/n;
			if ( newblock != iblock )
			{
				if ( newblock == jblock )
					k=newblock*n;
				else
					k=newblock*n + i%n;
			}
		}

		// Can take the % operation out of the inner loop.
		// Doesn't save much.
		if ( soli % 2==1 ) 
		{
			++result;
			//cout << i << " ";
		}

		++i;
	
	//cout << endl;
	//}
	return result;
}

ull singlerowsolution(ull i, ull n)
{
	ull srs = 0;
	for( int j=0; j<n; ++j)
		srs += singlesquaresolution(i,j,n);
	return srs;
}

ull singlerowsolution_below_diagonal(ull i, ull n)
{
	ull srs = 0;
	for( int j=i+1; j<n; ++j)
		srs += singlesquaresolution(i,j,n);
	return srs;
}

ull blocksinrow(ull i, ull n)
{
	ull blocks=0;
	ull lb2 = ((n-1)*(n-1)) - ( i*i );
	ull ub2 = n*n - i*i;
	
	ull lb = (ull)sqrt((double)lb2);
	ull ub = (ull)sqrt((double)ub2);

	if ( lb < 5 )
		lb=0;
	else
		lb-=5;
	if (ub+5 < n )
		ub += 5;
	else
		ub=n-1;

	for( ;lb < n; ++lb)
		if (isBoardSet(lb, i,n )==1)
			break;

	for( ;ub>=0; --ub )
		if (isBoardSet(ub,i,n)==1)
			break;

	blocks = ub-lb+1;
	
	return blocks;
}

void circleColStartAndEnd(ull i, ull &rs, ull &re, ull n)
{
	ull blocks=0;
	ull lb2 = ((n-1)*(n-1)) - ( i*i );
	ull ub2 = n*n - i*i;
	
	ull lb = (ull)sqrt((double)lb2);
	ull ub = (ull)sqrt((double)ub2);

	if ( lb < 5 )
		lb=0;
	else
		lb-=5;
	if (ub+5 < n )
		ub += 5;
	else
		ub=n-1;

	for( ;lb < n; ++lb)
		if (isBoardSet(lb, i,n )==1)
			break;

	for( ;ub>=0; --ub )
		if (isBoardSet(ub,i,n)==1)
			break;

	rs = lb;
	re = ub;
	
}

ull evensolution(ull n)
{
	ull result = 0ULL;

	// Store in odd whether row[i] has odd (==T) or even no. of ones set
	
	odd.resize(n, false);
	even.resize(n, true);
	for(ull i=0; i<n; ++i)
		if ( blocksinrow(i,n) % 2 == 1 )
		{
			odd.set(i);
			even.reset(i);
		}
		
	for(ull i=0; i<n; ++i)
	{
		row.resize(n,false);
		row.reset();
		ull rs;
		ull re;
		circleColStartAndEnd(i,rs,re,n);
		for(ull b=rs;b<=re;++b)
			row.set(b);
		//cout << "i=" << i << " rcb=" << row.count() << endl;
		if (odd[i])
		{
			row ^= even;
		}
		else
		{
			row ^= odd;
		}
		ull rc = row.count();
		//cout << "i=" << i << " rc=" << rc << endl;
		result += rc;
	}

	
	return result;
}

// As for v4 but only for unique rows
// Not currently giving correct answers but looks like it still
// won't be fast enough, anyway.
ull evensolution_v6(ull n)
{
	ull result = 0ULL;
	ull diagres = 0ULL;
	for(ull i=0; i<n; )
	{
		ull bir = blocksinrow(i,n);
		ull rowres = 0ULL;
		for( ull j=i; j<n; ++j)
		{
			ull sq = bir;
			sq += isBoardSet(i,j,n);
			sq += blocksinrow(j,n);
			if ( sq % 2 == 1 )
			{
				++rowres;
				if ( j==i )
					++diagres;
				//cout << "1";
			}
			//else
				//cout << "0";
		}
		//cout << endl;
		ull currentstart = circleColStart(i,n);
		ull nextstart = currentstart;
		ull previ = i;
		while( nextstart == currentstart && i<n )
		{
			++i;
			nextstart = circleColStart(i, n);
		}
		result += (i-previ)*rowres;
		//if (i==n-1)
		//	break;
	}
	return result*2-diagres;
}

// This is the same as v4, only using symmetry to half the
// solution space searched.
// Hence run time is halved.
ull evensolution_v5(ull n)
{
	ull result = 0ULL;
	ull diagres = 0ULL;
	for(ull i=0; i<n; ++i)
	{
		ull bir = blocksinrow(i,n);

		for( ull j=i; j<n; ++j)
		{
			ull sq = bir;
			sq += isBoardSet(i,j,n);
			sq += blocksinrow(j,n);
			if ( sq % 2 == 1 )
			{
				++result;
				if ( j==i )
					++diagres;
				//cout << "1";
			}
			//else
				//cout << "0";
		}
		//cout << endl;
	}
	return result*2-diagres;
}

// This works OK but it is order n^2 so it gets impractical
// around i=16 (or n=65520)
ull evensolution_v4(ull n)
{
	ull result = 0ULL;
	for(ull i=0; i<n; ++i)
	{
		ull bir = blocksinrow(i,n);

		for( ull j=0; j<n; ++j)
		{
			ull sq = bir;
			sq += isBoardSet(i,j,n);
			sq += blocksinrow(j,n);
			if ( sq % 2 == 1 )
			{
				++result;
				//cout << "1";
			}
			//else
				//cout << "0";
		}
		//cout << endl;
	}
	return result;
}

unsigned long long evensolution_v3(unsigned long long n)
{

	ull result = 0;

	ull i = 0;
	for(int j=n-2; j>=1; --j )
	{
		ull row_result = singlerowsolution_below_diagonal(i,n);
		//cout << "row_result("<<i<<")=" << row_result << endl;
		ull next_row = circleColStart(j, n);
		//cout << "next_row=" << next_row << endl;
		result += row_result * (next_row-i);
		//if ( next_row == i )
		//	break;
		i = next_row;
	}

	//cout << "half-res=" << result << endl;
	// Double the result - for symmetry
	result *= 2;
#if 0
	// Calculate diagonal
	//ull diag = (ull)((double)n / sqrt(2.0));
	//if ( n < 10 )
	//	diag=0;
	//else
	//	diag-=10;
	ull diag=0;
	ull diagres = 0;
	for(;diag<n;++diag)
	{
		diagres += singlesquaresolution(diag,diag,n);
	}
	//cout << "diagres=" << diagres << endl;
	result += diagres;
#endif

#if 0
	for(ull i=0; i<n; ++i)
	{
		ull srs = 0;
		for(int j=0; j<n; ++j)
			srs += singlesquaresolution(i,j,n);
		result += srs;
		cout << "row " << i << " srs=" << srs << endl;
	}
#endif

	// show slow solution
	for(ull i=0; i<n; ++i)
	{
		for(ull j=0; j<n; ++j)
		{
			ull soli = singlesquaresolution(i,j,n);
			if ( soli == 1 )
				cout << "1";
			else
				cout << "0";
		}
		cout << " ";
		for(ull j=0; j<n; ++j)
		{
			ull soli = isBoardSet(i,j,n);
			if ( soli == 1 )
				cout << "1";
			else
				cout << "0";
		}

		cout << endl;
	}
#if 0
	// Show circle
	cout << endl;
	for(ull i=0; i<n; ++i)
	{
		for(ull j=0; j<n; ++j)
		{
			ull soli = isBoardSet(i,j,n);
			if ( soli == 1 )
				cout << "1";
			else
				cout << "0";
		}
		cout << endl;
	}
#endif
	return result;
}


unsigned long long evensolution_v2(unsigned long long n)
{
	// Will contain solution
	unsigned long long result = 0L;

	// Find first row solution, and where it starts
	unsigned long long solstart = 0;
	for( unsigned long long i=0; i<n; ++i )
	{
		// for j=1 to n - but here n will be 1
		unsigned long long soli = 0;
		for( unsigned long long k=0; k<n*n; ++k )
		{
			//soli = (soli + getevenmat(i,k,n)*init[k]) % 2;
			soli += getevenmat(i,k,n)*isBoardSet(k/n,k%n,n);
		}
		// Can take the % operation out of the inner loop.
		// Doesn't save much.
		if ( soli % 2==1 ) 
		{
			solstart = i;
			goto firstrowstartfound;
		}
	}
	//cout << endl;
firstrowstartfound:
	//cout << "solstart=" << solstart << endl;

	for( unsigned long long i=solstart; i<n*n;  )
	{
		// for j=1 to n - but here n will be 1
		unsigned long long soli = 0;
		for( unsigned long long k=0; k<n*n; ++k )
		{
			//soli = (soli + getevenmat(i,k,n)*init[k]) % 2;
			soli += getevenmat(i,k,n)*isBoardSet(k/n,k%n,n);
		}
		// Can take the % operation out of the inner loop.
		// Doesn't save much.
		if ( soli % 2==1 ) 
		{
			++result;
			cout << i << " ";
			
		}
#if 0
		unsigned long long row = i/n;
		unsigned long long col = i%n;
		if (row < solstart)
		{
			if (col < solstart)
			{
				i+=solstart-col;
			}
			else
				++i;
		}
		else
		{
			++i;
		}
#endif
		++i;
	}
	cout << endl;
	
	return result;
}

unsigned long long evensolution_v1(unsigned long long n)
{
	// Will contain solution
	unsigned long long result = 0L;

	// TODO - for big n, this double loop algorithm isn't
	// going to be fast enough.
	
	for( unsigned long long i=0; i<n*n; ++i )
	{
		// for j=1 to n - but here n will be 1
		unsigned long long soli = 0;
		for( unsigned long long k=0; k<n*n; ++k )
		{
			//soli = (soli + getevenmat(i,k,n)*init[k]) % 2;
			soli += getevenmat(i,k,n)*isBoardSet(k/n,k%n,n);
		}
		if ( soli%2==1 )
		{
			++result;
			//cout << i << " ";
		}
	}
	//cout << endl;
	
	return result;
}

// The different values for T() that we'll need
unsigned long long solveAllEven()
{
	// Include T(5)==3, no other odd n are solveable
	unsigned long long result = 3; 
	
	for( int n=4; n<=30 /* 30 */; n+=2)
	{
		unsigned long long boardsize = (1ULL << n) - n;
		cout << "n=" << n << " T(" << boardsize << ")= ";
		unsigned long long solution_n = evensolution(boardsize);
		cout << solution_n << endl;
		result += solution_n;
	}
	return result;
}

void test01()
{
	for( int i=14; i<15; i+=2 )
	{
		cout << "n=" << i << " sol=" << evensolution(i) << endl;
		cout << endl;
	}
/*
n=4 sol=6
n=6 sol=17
n=8 sol=23
n=10 sol=29
n=12 sol=31
n=14 sol=72
n=16 sol=95
n=18 sol=134
n=20 sol=161
n=22 sol=187
n=24 sol=180
n=26 sol=261
n=28 sol=250
n=30 sol=375
n=32 sol=447
n=34 sol=467
n=36 sol=339
n=38 sol=490
*/
}



void colltest(ull n)
{
	cout << "Testing: " << n << endl;

	for(ull i=0; i<n; ++i)
	{
		ull rs;
		ull re;
		circleColStartAndEnd(i,rs,re,n);
		cout << "i=" << i << " " << rs << " " << re << endl;
	}
}

void colltestall()
{
	for( int n=4; n<=30 /* 30 */; n+=2)
	{
		unsigned long long boardsize = (1ULL << n) - n;
		cout << "n=" << n << " T(" << boardsize << ")= ";
		colltest(boardsize);
	}
}



int main()
{
	//cout << "es(10)=" << evensolution(10) << endl;
	//cout << "es(1000)=" << evensolution(1000) << endl;
	//test01();

	//for(int i=0; i<16; ++i)
	//	innertest(i,4);

	
	//evensolution(40);
	//test01();
	//colltest(14);
	const long long start = counter();
	cout << "Answer=" << solveAllEven() << endl;
	const long long finish = counter();
	cout << "Time taken: " << format_time(start, finish) << endl;
	return 0;
}