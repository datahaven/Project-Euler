// Project Euler 74
// http://projecteuler.net/index.php?section=problems&id=74
// Factorial Number Chains
//
// Adrian Dale 28/09/2010
//
// Answer is 402, produced in 343ms

#include <iostream>
#include <vector>
#include <Windows.h> // For GetTickCount
#include <set>
using namespace std;

// Stores factorial for each digit.
int F[10];

typedef vector<int> VIT;

VIT memo(10000000, 0);

// n!
// Works up to N==20 for long long version
// Works up to N==13 for unsigned/signed int version)
int fac(int N)
{
	int res = 1;
	for(int i=2;i<=N;++i)
		res *= i;
	return res;
}

// Get the sum of the factorials of the digits of i
// NB Doesn't work for i=0, but not needed for this problem.
int sumFacD(int i)
{
	int nsos = 0;
	while(i)
	{
		nsos += F[i % 10];
		i /= 10;
	}
	return nsos;
}

// Checks to see if i has the required property for the puzzle
// Try this the slow way first - simply calculate each i
// Works but is way too slow - I used it to check the results of
// the fast version when it wasn't working properly.
int checkFac_SLOW(int i)
{
	set<int> s;
	s.insert(i);
	pair<set<int>::iterator, bool> status;
	do
	{
		i = sumFacD(i);
		status = s.insert(i);
	} while (status.second == true);
	
	return s.size();
}

int checkFac(int i)
{
	int &m = memo[i];
	if ( m != 0 )
		return m;

	// Stores numbers seen so far in an easy to search form
	set<int> s;
	s.insert(i);
	// Stores numbers we've seen so far in order
	vector<int> v;
	v.push_back(i);
	pair<set<int>::iterator, bool> status;
	
	// Follow chain until we either reach its end,
	// or reach a position from where we already know
	// the answer
	int chainLen = 1;
	do
	{
		i = sumFacD(i);
		int nm = memo[i];
		if ( nm != 0 )
		{
			chainLen += nm;
			break;
		}
		status = s.insert(i);
		if (status.second)
		{
			++chainLen;
			v.push_back(i);
			
			//if ( chainLen > 60 )
			//{
				// This optimisation doesn't make any difference in the range we
				// are checking, as it never happens
				// Also, it would cause results to be incorrect if we were interested
				// in sequences longer than 60
			//	break;
			//}
		}
	} while (status.second == true);
	
	// Use vector to fill in the answers we found above
	int j = chainLen;
	for(unsigned int i=0; i<v.size(); ++i)
	{
		int &nnm = memo[v[i]];
		nnm = j--;
	}
	m = chainLen;
	return chainLen;
}

template <class Function>
__int64 time_call(Function&& f)
{
   __int64 begin = GetTickCount();
   f();
   return GetTickCount() - begin;
}

int run()
{
	for(int i=0;i<10;++i)
		F[i] = fac(i);

	int count = 0;
	for( int i=1; i<1000000; ++i )
		if (checkFac(i) == 60)
			++count;
	cout << "Answer=" << count << endl;
	return 0;
}

int main()
{
	__int64 itTime = time_call( [&]{
		run();
	});

	cout << "Took " << itTime << " ms" << endl;

	return 0;
}