// Project Euler 112
// Bouncy Numbers
// Adrian Dale
// 06/06/2018

// Congratulations, the answer you gave to problem 112 is correct.
// You are the 20156th person to have solved this problem.
// This problem had a difficulty rating of 15%

// Simple brute force solution, runs in well under a second
#include <iostream>
using namespace std;

typedef unsigned long long ULL;

inline bool isBouncy(ULL n)
{
	if (n < 100)
		return false;
	bool isInc = false;
	bool isDec = false;
	int prevDigit = n % 10;
	while ((n /= 10) > 0 && !(isDec == true && isInc == true))
	{
		int digit = n % 10;
		if (digit > prevDigit)
		{
			isInc = true;
		}
		else if (digit < prevDigit)
		{
			isDec = true;
		}
		prevDigit = digit;
	}
	return isDec == true && isInc == true;
}

int main()
{
	ULL bouncyCount = 0;
	const ULL bouncyMax = 10000000000ull;
	for (ULL n = 1; n < bouncyMax; ++n)
	{
		if (isBouncy(n))
			++bouncyCount;
		if (100 * bouncyCount == 99 * n)
		{
			cout << "n=" << n << endl;
			break;
		}
	}
	return 0;
}
