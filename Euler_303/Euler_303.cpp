// Project Euler 303
// Multiples With Small Digits
// Adrian Dale 26/09/2010
//
// Takes around 6 minutes to run on AMD 3.2GHz CPU

#include <iostream>
#include <Windows.h> // For GetTickCount
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

LLT tenPow[50];

// isValid returns true if d*n 
// is a valid 0,1,2's number.
inline bool isValid(LLT d, LLT n)
{
	LLT dn = d*n;
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

// The naive slow version - for test/research purposes
// Returns f(n)/n
// Too slow to handle 999, 9999, etc but handles most other cases
// reasonably well
LLT slowVersion(LLT n)
{
	LLT i=1;
	bool found = false;
	while(!found)
	{
		found = isValid(i,n);
		if (found)
			return i;
		++i;
	}

	return 0;
}

// Sets up a handy short cut array of values of 
// 10^x, since C++ does not have a power operator
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

LLT Base3Count(LLT in)
{
	LLT b3 = 0;
	
	for( LLT i=0;i<15000000000;++i)
	{
		B3Inc(b3);

		LLT n = b3 % in;
		LLT d = b3 / in;

		if ( n == 0 )
		{
			// cout << "Found b3(" << in << ")=" << b3 
			//      << " d=" << d << " dc=" << digitCount(b3) << endl;
			return d;
		}
	}
	// Loop size was increased until this line no longer appears in
	// output. It happens with the 9999 case which can be done by
	// hand anyway.
	cout << "ERROR - " << in << endl;
	return 0;
}

LLT run(LLT n)
{
	LLT sum1 = 0;
	
	for( LLT i=1; i<=n; ++i)
		sum1 += Base3Count(i);
	
	cout << "Answer=" << sum1 << endl;
	
	return 0;
}

int main()
{
	setup();

	__int64 elapsedTime = time_call([&] {
		run(10000);
	});
	cout << "Took " << elapsedTime << " ms" << endl;

	return 0;
}