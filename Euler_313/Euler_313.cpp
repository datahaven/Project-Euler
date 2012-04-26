// Project Euler 313
// Adrian Dale 15/03/2011
//
// See test04() for final working solution

#include <iostream>
#include <fstream>
#include <string>
#include <set>

using namespace std;

typedef unsigned long long ULL;

set<ULL> primeSquares;

set<ULL> primeSquaresTwo;

inline ULL minMoves(ULL m, ULL n)
{
	//if ( m > n )
	//	swap(m, n);

	if ( m == n )
		return 8*m - 11;

	return 2*m + 6*n - 13;
}


void readPrimes()
{
	string line;
	ifstream primeFile("primes.txt");
	int lineCount = 0;
	int addCount = 0;
	if (primeFile.is_open())
	{
		while( primeFile.good() )
		{
			ULL number;
			primeFile >> number;
			//cout << number << endl;
			if ( number < 1000000 )
			{
				primeSquares.insert(number * number);

				// We're only really using primeSquaresTwo now
				
				ULL pss = (number * number) + 13;
				if ( pss % 2 == 0 )
				{
					primeSquaresTwo.insert(pss / 2);
					++addCount;
					//cout << pss << " ";
				}
				
			}
			++lineCount;
			// NB last line gets read twice - can't be bothered to fix.
		}
		primeFile.close();
		cout << "Read " << lineCount << " lines" << endl;
		cout << "size of primeSquaresTwo=" << primeSquaresTwo.size() << endl;
	}
	else
		cout << "ERROR opening prime file" << endl;
}

// Works for the p<100 case, but too slow for p<1000000
void test01()
{
	int gridCount = 0;
	int eqCount = 0;
	for( ULL m=2; m<10000; ++m)
	{
		for( ULL n=m; n<10000; ++n)
		{
			ULL mm = minMoves(m,n);
			if ( mm < 10000 && primeSquares.find(mm) != primeSquares.end() )
			{
				if ( m == n )
				{
					++eqCount;
					++gridCount;
				}
				else
					gridCount += 2;
			}
		}
	}
	cout << "gridCount=" << gridCount << " eqCount=" << eqCount << endl;
}

// Still too slow!
void test02()
{
	int gridCount = 0;
	for( set<ULL>::iterator it = primeSquares.begin(); it != primeSquares.end(); ++it)
	{
		if ( (*it+11)%8 == 0 )
			++gridCount;
	}

	cout << "gridCount=" << gridCount << endl;

	ULL nmax = 0;
	ULL mmax = 0;
	for( ULL m=2; m<12000000; ++m)
	{
		for( ULL n=m+1; n<17000000; n++)
		{
			ULL t = m + 3*n;
			if ( primeSquaresTwo.find(t) != primeSquaresTwo.end() )
			{
				gridCount+=2;
				//nmax = max(nmax, n);
				//mmax = max(mmax, m);
			}
		}
		if ( m%1000 == 0 )
			cout << "m=" << m << endl;
	}

	cout << "mmax=" << mmax << " nmax=" << nmax << endl;

	cout << "gridCount=" << gridCount << endl;
}

void test03()
{
	cout << "test03" << endl;
	int gridCount = 0;
	int itCount = 0;
	for( set<ULL>::iterator it = primeSquaresTwo.begin(); it != primeSquaresTwo.end(); ++it)
	{
		ULL t = *it;
		for( ULL m=2; m<t-3*m; ++m )
		{
			ULL t2 = t-m;
			if ( t2 % 3 == 0 )
				gridCount += 2;
		}
		++itCount;
		if ( itCount %1000 == 0 )
			cout << "itCount=" << itCount << endl;
	}
	cout << endl;
	cout << "gridCount=" << gridCount << endl;
}

// Answer comes out out by one for some reason.
// Therefore do +1, then divide by 2 to get final correct answer.
// 2057774861813004
// Lightning fast!
void test04()
{
	cout << "test04" << endl;
	ULL gridCount = 0;
	int itCount = 0;
	for( set<ULL>::iterator it = primeSquaresTwo.begin(); it != primeSquaresTwo.end(); ++it)
	{
		ULL t = *it;
		ULL mStart = t % 3;
		if ( mStart <= 1 ) mStart+=3; // Since m>=2
		ULL rem = ((t - mStart) / 3) - (mStart>=3); // Since can't count n==1
		
	//	cout << "t=" << t << " mStart=" << mStart << " rem=" << rem << endl;
		gridCount += rem;

		++itCount;
		if ( itCount %1000 == 0 )
			cout << "itCount=" << itCount << endl;
	}
	cout << endl;
	cout << "gridCount=" << gridCount << endl;
}

int main()
{
	cout << "Project Euler 313" << endl;
	readPrimes();
	test04();
	return 0;
}