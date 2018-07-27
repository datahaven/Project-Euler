// Project Euler 129 - Repunit divisibility
// Adrian Dale
// 27/07/2018

// Congratulations, the answer you gave to problem 129 is correct.
// You are the 5087th person to have solved this problem.
// This problem had a difficulty rating of 45 % .The highest difficulty rating you have solved so far is 100 % .

// This page was really helpful:
// https://mathlesstraveled.com/2011/11/17/fun-with-repunit-divisors-more-solutions/

#include <iostream>
using namespace std;

typedef unsigned long long ULL;

ULL A(ULL n)
{
	
	ULL repunit = 1;
	for (ULL i = 1; true; ++i)
	{
		if (repunit % n == 0)
		{
			//cout << "A(" << n << ") == " << i << endl;
			return i;
		}
		repunit = repunit % n;
		repunit = 10 * repunit + 1;
	}
	return 0;
}

void Solve()
{
	ULL bestSoFar = 0;
	for (ULL i = 1000000; i < 100000000000; i+=1)
	{
		if (i % 2 == 0 || i % 5 == 0)
			continue;

		ULL Ai = A(i);
		if (Ai != 0)
		{
			if (Ai > bestSoFar)
			{
				cout << "A(" << i << ") == " << Ai << endl;
				bestSoFar = Ai;
			}
		}
		if (Ai > 1000000)
		{
			cout << "Result: " << i << endl;
			return;
		}
	}
}

int main()
{
	Solve();
	
	return 0;
}