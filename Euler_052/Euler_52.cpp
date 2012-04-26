// Project Euler 52
// It can be seen that the number, 125874, and its double, 251748,
// contain exactly the same digits, but in a different order.
//
// Find the smallest positive integer, x, such that 
// 2x, 3x, 4x, 5x, and 6x, contain the same digits.
//
// Adrian Dale 27/09/2010
//
// Another very quick bit of code - under ten mins for the messy version
// and not much more for the tidy (and faster) version
//
// Some interesting other solutions - use sets, add the digits of the first string,
// then return true if you fail to add the digits of the other multiples.
// Saves doing the string conversion but still needs while loop with div/mod to break
// the numbers down. Also saves doing the sorting, although it happens behind the scenes
// anyway.
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;

typedef long long LLT;

// Return true if x meets the puzzle conditions
// Very lazy bit of code!
// Really should do this properly.
// Worked first go, though!
bool checkX_MESSY(LLT x)
{
	ostringstream x2;
	ostringstream x3;
	ostringstream x4;
	ostringstream x5;
	ostringstream x6;

	x2 << 2*x;
	x3 << 3*x;
	x4 << 4*x;
	x5 << 5*x;
	x6 << 6*x;

	string x2st = x2.str();
	string x3st = x3.str();
	string x4st = x4.str();
	string x5st = x5.str();
	string x6st = x6.str();

	sort(x2st.begin(), x2st.end());
	sort(x3st.begin(), x3st.end());
	sort(x4st.begin(), x4st.end());
	sort(x5st.begin(), x5st.end());
	sort(x6st.begin(), x6st.end());

	return x2st == x3st && x3st == x4st && x4st == x5st && x5st == x6st;
}

// Phew! Dignity saved by the less embarassing version below.
bool checkX(LLT x)
{
	// Convert 2*x to a string and sort it
	ostringstream xas;
	string xa;
	xas << 2*x;
	xa = xas.str();
	sort(xa.begin(), xa.end());

	// Compare sorted 2*x with each of sorted 3*x, 4*x, etc
	for(int i=3; i<=6; ++i)
	{
		ostringstream xbs;
		string xb;
		xbs << i*x;
		xb = xbs.str();
		sort(xb.begin(), xb.end());
		if ( xa != xb )
			return false;
	}
		
	return true;
}

int main()
{
	LLT x = 1;
	while(++x)
	{
		if ( checkX(x) )
			break;
	}
	cout << "Answer=" << x << endl;

	return 0;
}