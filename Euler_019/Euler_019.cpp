// Project_019.cpp : Defines the entry point for the console application.
// Adrian Dale
//
// How many 1st of the months between 1/1/1901 and 31/12/2000
// were Sundays?
//
// Answer: 171
//
// I _nearly_ got this right first go.
// First attempt worked perfectly but didn't print the answer out. Doh!

#include <iostream>

using namespace std;

int DaysInMonth[] =
{ 31, 28, 31, 30, 31, 30, 31,31, 30, 31, 30, 31 };

bool isLeap(int year)
{
	return year%400 ==0 || (year%100 != 0 && year%4 == 0);
}

int main(int argc, char* argv[])
{
	int SundayCount = 0;
	int knownDay = 2;
	for( int year=1901; year <= 2000; ++year)
	{
		for( int month=0; month < 12; ++month )
		{
			if ( knownDay % 7 == 0 )
				SundayCount++;

			// Handle leap years
			if ( month == 1 && isLeap(year) )
			{
				knownDay++;
			}
			knownDay += DaysInMonth[month];
		}
	}

	cout << "SundayCount=" << SundayCount << endl;

	return 0;
}

