// Project Euler 455
// http://projecteuler.net/problem=455
// Adrian Dale 21/01/2014
//
// Congratulations, the answer you gave to problem 455 is correct.
//
// You are the 138th person to have solved this problem.
// (Answer produced in 87 seconds)
// testXX functions are not used in the final solution, although I've left
// them in the code to show my working.
// I solved this mostly by experimenting until I could see a pattern - the test
// fns show some of my experiments. Mostly looking at where digits repeat in 
// cycles for higher and higher powers.
// There are much faster ways to solve this using
// additional maths properties.
// Eventually in test03 I noticed that solutions stayed the same in the lower
// digits as you added more digits and that the problem could be solved
// digit by digit.
#include <iostream>
#include <iomanip>
#include <set>
#include <algorithm>
#include <ctime>
using namespace std;

typedef signed long long LL;

// From wikipedia http://en.wikipedia.org/wiki/Modular_exponentiation
LL modular_pow(LL base, LL exponent, LL modulus)
{
	LL result = 1;
	while (exponent > 0)
	{
		//cout << "b = " << base << " e = " << exponent << " r = " << result << endl;
		if (exponent % 2 == 1)
		{
			result = (result * base) % modulus;
		}
		exponent = exponent >> 1;
		base = (base * base) % modulus;
	}
	return result;
}

void test01(LL n)
{
	set<LL> seen;
	bool firstRepeatFound = false;
	LL firstRepeat = 0;
	LL firstRepVal = 0;
	LL mp = n;
	for (LL i = 0; i < 1000000000; ++i)
	{
		//LL mp = modular_pow(n, i, 1000000000);
		mp = (mp * n) % 1000000000; // Faster than re-calculating power every time.
		if (i < 100)
		{
			//cout << n << "^" << i << " = " << setw(10) << setfill('0') << mp << endl; 
		}
		if (seen.find(mp) != seen.end())
		{
			if (firstRepeatFound == false)
			{
				firstRepeatFound = true;
				firstRepeat = i;
				firstRepVal = mp;
				cout << "firstrepetition at i=" << firstRepeat << " val=" << firstRepVal << endl;
				cout << "seen.size()=" << seen.size() << endl;
				break;
			}
			else if (mp == firstRepVal)
			{
				//cout << "Next rep at i=" << i << endl;
				break;
			}
			
		}
		else
			seen.insert(mp);
	}

	// For n=4, at 4^781255 we get the first number we've seen before.
	// (seen.size() == 781255, of course.)
	// after that at 4^(781255 + k * 781250) we repeat again for k=1..)

	for_each(seen.begin(), seen.end(), [&](LL pwr) {
		LL mp = modular_pow(n, pwr, 1000000000);
		if (mp == pwr)

			cout << "n = " << n << " answer=" << mp << endl;
	});
}

// No set<LL> version of test01
void test02(LL n)
{
	LL bigPower = modular_pow(n, 999999999, 1000000000);
	//cout << "bigPower = " << bigPower << endl;
	LL mp = n;
	for (LL i = 0; i < 1000000000; ++i)
	{
		//LL mp = modular_pow(n, i, 1000000000);
		mp = (mp * n) % 1000000000; // Faster than re-calculating power every time.
		
		if (mp == bigPower)
		{
			cout << "n = " << n << " bigPower = " << bigPower << " at i = " << i << endl;
			break;
		}
	}

#if 0
	for_each(seen.begin(), seen.end(), [&](LL pwr) {
		LL mp = modular_pow(n, pwr, 1000000000);
		if (mp == pwr)

			cout << "n = " << n << " answer=" << mp << endl;
	});
#endif
}

void test03(LL n)
{
	LL pwr = n;
	for (LL i = 1; i < 1000000; ++i)
	{
		LL pwr = modular_pow(n, i, 1000000000);
		LL matches[4];
		matches[0] = modular_pow(n, 1, 10);
		matches[1] = modular_pow(n, 2, 10);
		matches[2] = modular_pow(n, 3, 10);
		matches[3] = modular_pow(n, 4, 10);

		bool match = pwr % 10 == i % 10;
		bool matchsecond = (pwr / 10) % 10 == (i / 10) % 10;
		bool matchthird = (pwr / 100) % 10 == (i / 100) % 10;
		bool matchfourth = (pwr / 1000) % 10 == (i / 1000) % 10;
		bool matchfifth = (pwr / 10000) % 10 == (i / 10000) % 10;
		if (match && matchsecond && matchthird && matchfourth && matchfifth)
		{

			cout << n << "^" << i << " = " << pwr << " match = " << matches[(i + 3) % 4];
			if (match) cout << " *";
			else cout << "  ";
			if (matchsecond) cout << " **";
			else cout << "   ";
			cout << endl;
		}
		pwr *= n;
	}

	LL mp = modular_pow(-6, 411728896, 100);
	cout << "mp=" << mp << endl;
	mp = modular_pow(-6, 411728895, 100)*411728896;
	cout << "mp=" << mp << endl;
	mp = modular_pow(-6, 411728894, 1000);
	cout << "mp=" << mp << endl;
}

// Return true if first digitCount number of digits
// of i and pwr are the same.
bool matchDigits(LL digitCount, LL i, LL pwr)
{
	while (digitCount-- > 0)
	{
		if (i % 10 != pwr % 10) return false;
		i /= 10;
		pwr /= 10;
	}
	return true;
}

// Calculate f(n) starting with lowest matching two digits, working
// upwards. Once we have a two digit match, the three digit matches
// come in jumps of 100 after; 1000 for four digit match, etc
LL f(LL n)
{
	LL pwr = n;
	LL i = 1;
	LL inc = 1;
		
	LL digitCount = 2;
	LL bestMatch = 0;
	while (i < 1000000000)
	{
		LL pwr = modular_pow(n, i, 1000000000);

		if (matchDigits(digitCount, i, pwr))
		{
			if (i == pwr && i > bestMatch)
			{
				bestMatch = i;
				// This still works if I put in this break.
				// Takes 2 seconds off run time.
				// Of course I only know this is allowed because I read this after seeing other solutions:
				// https://cs.uwaterloo.ca/journals/JIS/VOL12/Yebra/yebra4.pdf
				break;
			}

			if (digitCount == 2)
				inc = 100;
			else
				inc *= 10;

			++digitCount;
		}
		i += inc;
	}
	
	return bestMatch;
}

int main()
{
	cout << "Euler 455" << endl;
	
	time_t startTime;
	time(&startTime);

	LL sum = 0;
	for (int n = 2; n <= 1000000; ++n)
	{
		sum += f(n);
	}
	
	time_t endTime;
	time(&endTime);
	cout << "Answer=" << sum << endl;
	cout << "Processing Time = " << difftime(endTime, startTime) << " seconds" << endl;
	return 0;
}