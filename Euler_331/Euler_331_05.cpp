// Project Euler 331 - Cross Flips
// Adrian Dale
// 28/05/2011
//
// Congratulations, the answer you gave to problem 331 is correct.
//
// You are the 54th person to have solved this problem.
//
/*
n=4 T(12)= 31 t=870.711 microseconds
n=6 T(58)= 1193 t=1.07018 milliseconds
n=8 T(248)= 22385 t=1.20169 milliseconds
n=10 T(1014)= 372547 t=1.28969 milliseconds
n=12 T(4084)= 6275536 t=2.58427 milliseconds
n=14 T(16370)= 100990283 t=6.51102 milliseconds
n=16 T(65520)= 1630913781 t=21.2398 milliseconds
n=18 T(262126)= 26146650973 t=79.8004 milliseconds
n=20 T(1048556)= 418057047705 t=314.057 milliseconds
n=22 T(4194282)= 6683197830265 t=1.25302 seconds
n=24 T(16777192)= 106934422355761 t=5.00218 seconds
n=26 T(67108838)= 1710850979291959 t=20.0799 seconds
n=28 T(268435428)= 27373815729676843 t=82.5229 seconds
n=30 T(1073741794)= 437979504875414284 t=323.19 seconds
Answer=467178235146843549
Time taken: 432.496 seconds
*/
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

// This works up to n=20, which takes just over two minutes.
// Each increment of n seems to take 10-12x the previous one
ull evensolution(ull n)
{
	ull result = 0ULL;

	//row.resize(n,false);

	// Store in odd whether row[i] has odd (==T) or even no. of ones set
	
	odd.resize(n, false);
	odd.reset();
	even.resize(n, true);
	even.set();
	for(ull i=0; i<n; ++i)
	{
		if ( blocksinrow(i,n) % 2 == 1 )
		{
			odd.set(i);
			even.reset(i);
		}
	}
	//cout << odd << endl;
	size_t oddcount = odd.count(); // TODO - this must be quicker done inside our loop?
	size_t evencount = n-oddcount;
	//cout << "oddcount=" << oddcount << " evencount=" << evencount << endl;
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

		//row.reset();
		prs = rs; pre = re;

		ull rc;
		if (odd[i])
			rc = evencount;
		else
			rc = oddcount;
		// TODO - get rid of even array
		for(ull b=rs;b<=re;++b)
		{
			bool rowbit;
		
			if (odd[i])
			{
				rowbit = even[b];
				if (rowbit)
					--rc;
				else
					++rc;
			}
			else
			{
				rowbit = odd[b];
				if (rowbit)
					--rc;
				else
					++rc;
			}

			
		}

		
		//cout << "i=" << i << " rc=" << rc << endl;
		result += rc;
		prev_rc = rc;
	}

	
	return result;
}

// This works up to n=20, which takes just over two minutes.
// Each increment of n seems to take 10-12x the previous one
ull evensolution_v7(ull n)
{
	ull result = 0ULL;

	row.resize(n,false);

	// Store in odd whether row[i] has odd (==T) or even no. of ones set
	
	odd.resize(n, false);
	odd.reset();
	even.resize(n, true);
	even.set();
	for(ull i=0; i<n; ++i)
	{
		if ( blocksinrow(i,n) % 2 == 1 )
		{
			odd.set(i);
			even.reset(i);
		}
	}
	//cout << odd << endl;

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

		row.reset();
		prs = rs; pre = re;
		for(ull b=rs;b<=re;++b)
			row.set(b);
		//cout << "i=" << i << " rcb=" << row.count() << endl;
		if (odd[i])
			row ^= even;
		else
			row ^= odd;

		ull rc = row.count();
		//cout << "i=" << i << " rc=" << rc << endl;
		result += rc;
		prev_rc = rc;
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


// The different values for T() that we'll need
unsigned long long solveAllEven()
{
	// Include T(5)==3, no other odd n are solveable
	unsigned long long result = 3; 
	
	for( int n=4; n<=30 /* 30 */; n+=2)
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

void test01()
{
	for( int i=12; i<40; i+=2 )
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







int main()
{
	//cout << "es(10)=" << evensolution(10) << endl;
	//cout << "es(1000)=" << evensolution(1000) << endl;
	//test01();

	//for(int i=0; i<16; ++i)
	//	innertest(i,4);

	
	//cout << "es(14)=" << evensolution(14) << endl;
	//test01();
	

	
	const long long start = counter();
	cout << "Answer=" << solveAllEven() << endl;
	const long long finish = counter();
	cout << "Time taken: " << format_time(start, finish) << endl;
	return 0;
}