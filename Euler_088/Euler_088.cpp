// Project Euler 88
// Minimal Product-Sum Sets
// Adrian Dale 01/08/2012

/*
Congratulations, the answer you gave to problem 88 is correct.

You are the 3105th person to have solved this problem.

You have earned 1 new award:

    Centurion: Solve one hundred consecutive problems
*/

// Answer = 7587457

// This is the messy version of the code, but with the unused bits removed

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

// Useful info: http://www-users.mat.umk.pl/~anow/ps-dvi/si-krl-a.pdf

const int KMAX = 12000;
int bestkValues[KMAX+2];

bool incDigitsII(int maxDigits, int maxDigitVal, int *digits)
{
	const int maxprod = 24000;

	int incIndex = maxDigits-1;
	
	digits[incIndex]++;
	
	// See if we can carry straight away
	int product = 1;
	for(int i=0; i<maxDigits; ++i)
	{
		product *= digits[i];
	}

	bool carry = (digits[incIndex] > maxDigitVal) || product > maxprod;

	while(carry)
	{
		incIndex--;
		if (incIndex >= 0)
		{
			digits[incIndex]++;
			carry = digits[incIndex] > maxDigitVal;
		}
		else
			return false; // overflow

		for(int j=incIndex+1; j<maxDigits; ++j)
		{
			digits[j] = digits[j-1];
		}

		product = 1;
		for(int i=0; i<maxDigits; ++i)
		{
			product *= digits[i];
		}
		carry |= (product > maxprod);
	}
	
	return true;
}

void test03()
{
	int digits[15];

	int digitCount = 2;
	
	int callcount = 0;

	while(digitCount < 16)
	{
		for(int i=0; i<15; ++i)
			digits[i] = 1;

		do
		{
			int sum=0;
			int product = 1;
			for(int i=0; i<digitCount; ++i)
			{
				sum += digits[i];
				product *= digits[i];
			}
			int k = digitCount - sum + product;
			if (k <= KMAX && k >= 2)
			{
				if (bestkValues[k] > product)
					bestkValues[k] = product;
			}

			++callcount;
		} while( incDigitsII(digitCount, 24000, digits) );

		++digitCount;
	}

	//cout << "callcount=" << callcount << endl;
}

int main()
{
	for(int k=0; k<KMAX; ++k)
	{
		bestkValues[k] = 25000;
	}

	test03();

	// Needs to be sum of unique bestkValues

	vector<int> kVector(bestkValues+2, bestkValues+KMAX+1);
	sort(kVector.begin(), kVector.end());
	auto it = unique(kVector.begin(), kVector.end());
	kVector.resize( it - kVector.begin() );

	int sum = accumulate(kVector.begin(),kVector.end(),0);
	cout << "Answer=" << sum << endl;
}