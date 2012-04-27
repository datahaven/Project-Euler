// Project Euler 303
// Multiples With Small Digits
// Adrian Dale 26/09/2010
#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h> // For GetTickCount
#include <queue>
#include <functional>
#include <algorithm>
#include <set>
#include <assert.h>
using namespace std;

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

typedef unsigned long long  LLT;

LLT tenPow[100];

inline bool isValidOLD(LLT d, LLT n, LLT mod = 0)
{
	LLT dn = d*n;
	if ( mod != 0 )
		dn %= mod;

	bool solOK = true;
	while(dn)
	{
		if ( dn % 10 > 2 )
		{
			solOK = false;
			break;
		}
		dn /= 10;
	}
	return solOK;
}

// Returns the number of decimal digits in d
// ie d(0)==1, d(123)==3
inline LLT digitCount(LLT d)
{
	LLT res = 0;
	do
		++res;
	while(d /= 10);
	return res;
}

// isValid returns true if d*n %mod (if mod specified)
// is a valid 0,1,2's number.
// This version needs to work for big integer results
LLT dcmax = 0;
inline bool isValid(LLT d, LLT n, LLT mod = 0)
{
	LLT dn = d*n;
	if ( dn < d || dn < n )
	{
		cout << "OVERFLOW!!!" << endl;
		return true; // Tends to stop the algorithm
	}

	if ( mod != 0 )
		dn %= mod;

	LLT dc = digitCount(dn);
	if ( dc > dcmax )
	{
		dcmax = dc;
	//	cout << "new dcmax=" << dcmax << endl;
	}
	bool solOK = true;
	while(dn)
	{
		if ( dn % 10 > 2 )
		{
			solOK = false;
			break;
		}
		dn /= 10;
	}
	return solOK;
}

// The naive slow version - for test purposes
// Returns f(n)/n
// Goes wrong at 999 - overflow?
LLT test01(LLT n)
{
	LLT i=1;
	bool found = false;
	while(!found)
	{
		found = isValid(i,n);
		if (found)
			return i;
		++i;

		// DEBUG
		if ( i==1 )
			cout << "test01 OVERFLOW" << endl;
		if ( i > 1000000000 )
		{
			cout << "BIG NUMBER - STOPPING EARLY" << endl;
			return 0;
		}
	}

	return 0;
}


// Set version - which has the advantage over the priority queue version of 
// actually working. PQ one fails as you can put the same number in the queue
// multiple times, which you don't want.
LLT test03(LLT n)
{
	set<LLT> q;
	
	LLT lsbn = n % 10;
	// Cache this as we use it frequently
	LLT dcn = digitCount(n);

	for(LLT i=1;i<=10;++i)
	{
		if ( lsbn*i % 10 < 3 )
			q.insert(i);
	}
	
	int count = 0;
	while( !q.empty() )
	{
		cout << endl;
		// Pop the top digit off the queue
		LLT d = *q.begin();
		q.erase(q.begin());

		if (isValid(d,n))
			return d;
		else
		{
			// Put the one digit longer versions of d on the queue for trial in turn
			LLT dc = digitCount(d);
			for(LLT i=1;i<=10;++i)
			{
				LLT nd = tenPow[dc]*i + d;

				if ( digitCount(nd) > dc+1 )
					nd |= 0x8000000000000000;

				// Only push digits for trial if they would produce valid lsb's
				assert((min(dc,dcn)-1) < sizeof(tenPow)/sizeof(LLT) );
				
				
				bool valid=false;
				if (dcn > dc)
					valid = isValid( nd,n, tenPow[dc] );
				else
					valid = isValid( nd, n, tenPow[dcn]);
				if (valid)
				{
					cout << nd << " ";
					q.insert(nd);
				}
			}
		}
	}

	return 0;
}

// Set version - attempting to fix case when digits 02 are needed
LLT test04(LLT n)
{
	set<pair<LLT,LLT> > q;
	
	LLT lsbn = n % 10;
	// Cache this as we use it frequently
	LLT dcn = digitCount(n);

	for(LLT i=1;i<=10;++i)
	{
		if ( lsbn*i % 10 < 3 )
			q.insert(make_pair(i,i==10 ? 1 : 0));
	}
	
	int count = 0;
	while( !q.empty() )
	{
		// Pop the top digit pair off the queue
		pair<LLT,LLT> d = *q.begin();
		q.erase(q.begin());

	//	cout << endl << d.first << ":" << d.second << " ";

		// Only test number if it is complete
		if ( d.second != 1 && isValid(d.first,n))
			return d.first;
		else
		{
			// Put the one digit longer versions of d on the queue for trial in turn
			LLT dc = digitCount(d.first);
			for(LLT i=1;i<=10;++i)
			{
				LLT nd;
				if ( d.second == 1 )
				{
					nd = tenPow[dc-1]*i + (d.first - tenPow[dc-1]);
			//		cout << "X:" << nd << " ";
				
				}
				else
				{
					nd = tenPow[dc]*i + d.first;
			//		cout << "Y:" << nd << " ";
				}
			
				if ( digitCount(nd) > dc )
				{
					LLT nnd = nd;// % tenPow[dc];
				//	cout << nnd << "* ";
					q.insert(make_pair(nnd, 1));
				}
				else
				{
					bool valid=false;
					if (dcn > dc)
						valid = isValid( nd,n, tenPow[dc] );
					else
						valid = isValid( nd, n, tenPow[dcn]);
					if (valid)
					{
					//	cout << nd << " ";
						q.insert(make_pair(nd, 0));
					}
				}
			}
		}
	}

	return 0;
}

int run01()
{
	LLT sum1 = 0;
	LLT sum2 = 0;
	bool errorFound = false;
	for( LLT i=1; i<=10000; ++i)
	{
		switch(i) {
		case 999:
		case 1998:
		case 2997:
		case 3996:
		case 4995:
		case 5994:
		case 6993:
		case 7992:
		case 8991:
		case 9899:
		case 9990:
		case 9999:
			cout << "SKIPPING DODGY CASE" << endl;
			continue;
		default:
			;
		};

		LLT res1 = 0;
		res1 = test01(i);
		sum1 += res1;
		LLT res2 = 0;
	//	res2 = test04(i);
		sum2 += res2;

		if ( res1 == res2 )
		{
			cout << i << " " << res1 << " - OK" << endl;
		}
		else
		{
			errorFound = true;
			cout << i << " " << res1 << " " << res2 << " - ERROR" << endl;
		}
	}
	cout << "sum=" << sum1 << endl;
	if ( errorFound )
		cout << "WARNING: ERRORS FOUND" << endl;

	return 0;
}

void setup()
{
	tenPow[0] = 1;
	for(int i=1; i<sizeof(tenPow)/sizeof(LLT); ++i)
		tenPow[i] = tenPow[i-1] * 10;
}

// Increments b3 (a regular long long) as though it were in Base 3,
// therefore ensuring we can step in turn through each number
// containing only 0,1,2's
void B3Inc(LLT &b3)
{
	int incpos = 0;
	bool carry = false;
	do
	{
		carry = false;
		LLT digit = (b3 / tenPow[incpos]) % 10;
		b3 -= tenPow[incpos] * digit;
		digit++;
		
		if ( digit > 2 )
		{
			digit = 0;
			carry = true;
			++incpos;
		}
		else
			b3 += tenPow[incpos] * digit;
	} while(carry);
}

LLT Base3CountTest(LLT in)
{
	LLT b3 = 0;
	
	for( LLT i=0;i<15000000000;++i)
	{
		B3Inc(b3);

		LLT n = b3 % in;
		LLT d = b3 / in;

		if ( n == 0 )
		{
			// cout << "Found b3(" << in << ")=" << b3 << " d=" << d << " dc=" << digitCount(b3) << endl;
			return d;
		}
	}
	cout << "ERROR - ERROR - ERROR - ERROR " << in << endl;
	return 0;
}

LLT test05(LLT n)
{
	LLT sum1 = 0;
	
	bool errorFound = false;
	for( LLT i=1; i<=n; ++i)
	{
		LLT res1 = 0;
		res1 = Base3CountTest(i);
		sum1 += res1;
	}
	cout << "sum=" << sum1 << endl;
	if ( errorFound )
		cout << "WARNING: ERRORS FOUND" << endl;

	return 0;
}

int main()
{
	setup();

	__int64 elapsedTime = time_call([&] {
	//	run01();
		test05(10000);
	});
	cout << "Took " << elapsedTime << " ms" << endl;

	//cout << "t1(99)=" << test01(999) << endl;
	//cout << "t4(999)=" << test04(999) << endl;
	return 0;
}