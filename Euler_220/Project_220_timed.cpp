// Project Euler 220
// Heighway Dragon
// Adrian Dale
// 16/6/2011
//
// You are the 1001st person to have solved this problem.
// (139776,963904) in 3810.29 seconds (1hr, 3min, 30seconds)
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <Windows.h>
using namespace std;

typedef long long IntType;

long long counter() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return li.QuadPart;
}

long long frequency() {
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
    return li.QuadPart;
}

string format_time(const long long start, const long long finish) {
    ostringstream oss;

    if ((finish - start) * 1000 < frequency()) {
        oss << (finish - start) * 1000000.0 / frequency() << " microseconds";
    } else if (finish - start < frequency()) {
        oss << (finish - start) * 1000.0 / frequency() << " milliseconds";
    } else {
        oss << (finish - start) * 1.0 / frequency() << " seconds";
    }

    return oss.str();
}

void test01()
{
	int x = 0;
	int y = 0;
	int facing = 0;

	int newfacingL[] = {3,0,1,2,3,0,1,2};
	int newfacingR[] = {1,2,3,0,1,2,3,0};
	int newxf[] = {0,1,0,-1};
	int newyf[] = {1,0,-1,0};

	for( IntType n=1; n<=1000000000000; ++n )
	{
		// turn is true for L, false for R
		if ((((n & (-1*n)) << 1) & n) != 0)
			facing = newfacingL[facing];	
		else
			facing = newfacingR[facing];

		x += newxf[facing];
		y += newyf[facing];
	}
	cout << "(" << x << "," << y << ")" << endl;
	
}

int main()
{
	

	const long long start = counter();
	
	IntType result = 10;
	test01();
	const long long finish = counter();

	cout << result << " in " << format_time(start, finish) << endl;

	return 0;
}
