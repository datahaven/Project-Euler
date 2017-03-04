// Project Euler 491
// https://projecteuler.net/problem=491
// Double pandigital number divisible by 11
//
// Adrian Dale
// 04/03/2017
//
// Congratulations, the answer you gave to problem 491 is correct.
//
// You are the 1208th person to have solved this problem.
//
#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>
using namespace std;

typedef unsigned long long ULL;

void solve()
{
	// This is horribly inefficient code but it runs in a couple of seconds,
	// so it'll do for now. It could be cleaned out a lot more.
	const int maxDigits = 20;
	vector<int> digitBits(maxDigits, 0);
	fill(digitBits.begin() + maxDigits / 2, digitBits.end(), 1);

	ULL result = 0ull;
	
	set<string> digits;

	// Use every permutation of 00000000001111111111 to select all the distinct
	// combinations of digits 0-9 from the set of digits 00112233445566778899
	// This method returns duplicate combinations so use a set to filter out duplicates.
	// Also, only store digits where the sum of digits in the set minus the sum of the
	// digits not in the set is a multiple of 11.
	// This uses the rule that a number is divisible by 11 if its alternating sum is divisible by 11
	do
	{
		string ds = "";
		string dso = "";
		int evenSum = 0;
		for (int i = 0; i < maxDigits; i++)
		{		
			if (digitBits[i] == 1)
			{
				ds += static_cast<char>('A' + i / 2);
				evenSum += i / 2;
			}
			else
			{
				dso += static_cast<char>('A' + i / 2);
			}
		}
		if ((90 - 2*evenSum)%11 == 0)
			digits.insert(ds+"-"+dso);
	} while (next_permutation(digitBits.begin(), digitBits.end()));

	// The digits can go in any order in our alternating sum, so long as they remain in
	// odd/even digit positions, as chosen above. Also, the number can't start with a zero.
	for (const auto &ds : digits)
	{
		// Work out how many variations of this set we can have
		// Horrible code alert! You can do this with basic maths, which is much faster.
		// As if the parsing strings into/out of sets wasn't bad enough!
		vector<int> plusDigits;
		vector<int> minusDigits;
		bool dashFound = false;
		for (const auto c : ds)
		{
			if (c == '-')
			{
				dashFound = true;
				continue;
			}
			if (dashFound == false)
			{
				plusDigits.push_back(c);
			}
			else
			{
				minusDigits.push_back(c);
			}
		}
		ULL permCountPlus = 0;
		ULL permCountMinus = 0;
		do
		{
			if (plusDigits[0] != 'A')
			{
				permCountPlus++;
			}
		} while (next_permutation(plusDigits.begin(), plusDigits.end()));

		do
		{
			permCountMinus++;
		} while (next_permutation(minusDigits.begin(), minusDigits.end()));
		result += permCountPlus * permCountMinus;
	}
	cout << "Answer=" << result << endl;
}

int main()
{
	solve();
	
	return 0;
}