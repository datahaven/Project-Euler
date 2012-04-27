// Project Euler 374
// http://projecteuler.net/index.php?section=problems&id=374
// Adrian Dale
// 04/03/2012
/*
Congratulations, the answer you gave to problem 374 is correct.

You are the 113th person to have solved this problem.

Return to Problems page or go to thread 374 in the forum.

334420941
*/

#include <iostream>
#include <sstream>
using namespace std;

typedef unsigned long long ull;

#include <windows.h> // For QPC timer

// Timer code from Stephan T. Lavavej's Nurikabe solver
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

// From http://comeoncodeon.wordpress.com/tag/factorial/
ull factMOD(ull n, ull MOD)
{
    long long res = 1;
    while (n > 0)
    {
        for (ull i=2, m=n%MOD; i<=m; i++)
            res = (res * i) % MOD;
        if ((n/=MOD)%2 > 0)
            res = MOD - res;
    }
    return res;
}

ull ProductCache[1800000];
ull modnum = 982451653ull;

void test01()
{
	cout << "test01" << endl;
	// Populate ProductCache
	ull startval = 100ull;
	ProductCache[0] = startval;
	ull prodval = startval;
	for(ull i=1; i<startval; ++i)
	{
		ProductCache[i] = (ProductCache[i-1] * --prodval) % modnum;
		//cout << ProductCache[i] << endl;
	}

	ull partcount = 0;
	ull base = 89;
	ull n = base+1;
	//ull mp=factMOD(100, modnum);
	ull mplower=factMOD(base, modnum);
	while( n <= 100 )
	{
		//cout << factMOD(n, modnum) << endl;
		int index = 100-n-1;
		//cout << "index=" << index << " val=" << ProductCache[index] << endl;
		ull pc=1;
		if (index >= 0)
		{
			pc = ProductCache[index];
		}

		ull mp = (mplower * pc) % modnum;
		cout << "n=" << n << " mp=" << mp << endl;

		mplower = (mplower * n) % modnum;
		++n;
	}
}

void test02()
{
	cout << "test02" << endl;
	ull sum = 936735527ull;
	ull n = 99999998250179ull;
	ull nmax = 100000000000000ull;

	// Populate ProductCache
	ull base = 12392314ull;
	ull startval = 14142136ull;
	ProductCache[0] = startval;
	ull prodval = startval;
	for(ull i=base; i<startval; ++i)
	{
		ull ix = i - base + 1;
		ProductCache[ix] = (ProductCache[ix-1] * --prodval) % modnum;
		//cout << ProductCache[i] << endl;
	}

	for(ull i=1749810; i<=1749822; ++i)
	{
		cout << "pc[" << i << "]=" << ProductCache[i] << endl;
	}
	cout << "Product Cache filled" << endl;

	
	ull partcount = 0;
	ull partlen = 14142136ull;
	
	ull mplower=factMOD(base-1, modnum);
	while( n <= nmax+1 )
	{
		//cout << factMOD(n, modnum) << endl;
		int index = nmax-n-1+1 /*- 14142136ull*/;
		//cout << "index=" << index << " val=" << ProductCache[index] << endl;
		ull pc=1;
		if (index >= 0)
		{
			pc = ProductCache[index];
		}

		ull mp = (mplower * pc) % modnum;
		
		ull mppl = (mp*(partlen-2)) % modnum;

		if (n != 99999998250179ull) // A bit of voodoo!
			sum = (sum + mppl)%modnum;

		if (nmax+1-n < 10 || n < 99999998250189ull )
		{
			cout << "n=" << n << " mp=" << mp << " sum=" << sum << endl;
		}
		mplower = (mplower * (base+partcount)) % modnum;
		++n;
		++partcount;
	}

	cout << "Final sum=" << sum << endl;
}

void test04()
{
	ull sum = 10;
	ull n = 4;
	ull partlen = 2;
	
	ull nmax = 100000000000000ull;

	ull aprev = 5;
	ull nfac = 6;
	ull oldsum = 0;
	while(n<=nmax)
	{
		//cout << "n=" << n << " partlen=" << partlen << " sum=" << sum << endl;
		oldsum = sum;
		ull anext = (aprev * (partlen+2))%modnum;
		anext = (anext + nfac) % modnum;

		sum = (sum + ((anext*partlen)%modnum) )%modnum;

		n = n + partlen + 2;
		aprev = anext;
		ull lastrow = (( ((partlen * nfac / 2)) * (partlen+3))) % modnum;
		//cout << "lastrow=" << lastrow << endl;
		sum = (sum + lastrow) % modnum;
		partlen = partlen+1;
		nfac = (nfac * (partlen+1)) % modnum;
	}
	cout << "Step One: new n=" << n-partlen-1 << " partlen=" << partlen
		<< " nfac=" << nfac << " n=" << n << endl;
	cout << "oldsum=" << oldsum << endl;
}

int main()
{
	const long long start = counter();
	test02();
	const long long finish = counter();
	cout << "Time Taken: " << format_time(start, finish) << endl;

	return 0;
}