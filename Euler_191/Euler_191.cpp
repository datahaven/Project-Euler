// Project Euler 191
// School Attendance Records
// Adrian Dale 29/09/2010
//
// Takes 20 seconds to run.
// Looking at other solutions, I missed some interesting answers, much shorter than mine.
// Especially the recursive depth first search one that uses a memo table.
// Can't believe I missed all that with all the clues and all the DP I've done recently!
// State machine solutions are interesting, though!
// As is pen and paper solution by deriving a direct solution (somehow!)
#include <iostream>
#include <vector>
#include <Windows.h>
using namespace std;

typedef long long LLT;

LLT tenPow[50];

struct ADT
{
	LLT dn;
	int lc; // Late Count
	int lac; // Left Absent Count
	int rac; // Right Absent Count
};

vector<ADT> Segments;

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

bool check(LLT dn)
{
	if ( dn == 0 )
	{
		ADT z;
		z.lc = 0;
		z.rac = 0;
		z.lac = 0;
		z.dn = 0;
		Segments.push_back(z);
		return true;
	}
	
	LLT ddn = dn;

	int lc = 0;
	int ac = 0;
	int dc = 0;
	int d29 = 0;
	int d30 = 0;
	ADT r;
	r.dn = dn;
	r.lac = 0; r.rac = 0;
	while(dn)
	{
		switch( dn % 10 )
		{
		case 0: // On time
			ac = 0;
			break;
		case 1: // Absent
			if (++ac == 3 )
				return false;
			if ( dc == 0 )
				r.rac = 1;
			if ( dc == 1 && r.rac == 1 )
				r.rac = 2;
			break;
		case 2: // Late
			ac = 0;
			if (++lc > 1 )
				return false;
			break;
		default:
			cout << "ERROR: " << dn << endl;
		}

		if ( dc == 8 )
			d29 = dn % 10;
		if ( dc == 9 )
			d30 = dn % 10;

		dn /= 10;
		++dc;
	}

	r.lc = lc;

	if ( d30 == 1 && d29 == 1 )
		r.lac = 2;
	else if ( d30 == 1 && d29 != 1 )
		r.lac = 1;
	
	Segments.push_back(r);

	return true;
}

void run(void)
{
	setup();
	
	// Generate results for a set of ten days
	LLT t = 0;
	for( int i=0; i<59049; ++i ) // 3^10
	{
		check(t);
		B3Inc(t);
	}

	// Combine the results, checking all the edge cases
	LLT count = 0;
	LLT sc = 0;
	for( vector<ADT>::iterator it1 = Segments.begin(); it1 != Segments.end(); ++it1 )
		for( vector<ADT>::iterator it2 = Segments.begin(); it2 != Segments.end(); ++it2 )
		{
			if ( it1->lc + it2->lc > 1 )
				continue;
			if ( it1->rac + it2->lac >= 3 )
				continue;
			for( vector<ADT>::iterator it3 = Segments.begin(); it3 != Segments.end(); ++it3 )
			{
				if ( it1->lc + it2->lc + it3->lc > 1 )
					continue;
				if ( /*it1->rac + it2->lac >= 3 || */ it2->rac + it3->lac >= 3 )
					continue;
				
				if ( it1->dn == it2->dn && it2->dn == it3->dn )
					 continue;
				++count;
				
			}
		}

	for( vector<ADT>::iterator it1 = Segments.begin(); it1 != Segments.end(); ++it1 )
	{
		if ( it1->lc != 0 )
			continue;
		if ( it1->lac + it1->rac >= 3 )
			continue;
		++sc;
	}

	cout << "Answer=" << count + sc << endl;
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

int main()
{
	__int64 runtime = time_call( []{run();} );
	
	cout << "Took " << runtime << " ms" << endl;
	return 0;
}