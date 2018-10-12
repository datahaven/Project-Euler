// Project Euler 504
// Square on the Inside
// Adrian Dale
// 12/10/2018

// Congratulations, the answer you gave to problem 504 is correct.
// You are the 2203rd person to have solved this problem.

#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

typedef unsigned long long ULL;

const int m = 100;

vector<int> g_PointsOnLine((m+1)*(m+1));

// NB gcd is part of c++ 17, but compiler doesn't seem to recognise it.
// I borrowed this code from
// https://developercommunity.visualstudio.com/content/problem/233356/stdgcd-performance-issue.html
int gcd_positive_numbers(const int a, const int b)
{
	if (a == 0 || a == b)
		return b;
	if (b == 0)
		return a;
	if (a > b)
		return gcd_positive_numbers(a % b, b);
	return gcd_positive_numbers(a, b % a);
}
int gcd(int a, int b)
{
	a = std::abs(a);
	b = std::abs(b);
	if (a == 0 || a == b)
		return b;
	if (b == 0)
		return a;
	if (a > b)
		return gcd_positive_numbers(a % b, b);
	return gcd_positive_numbers(a, b % a);
}

int CountPointsOnLine(int a, int b)
{
	return gcd(a, b) + 1;
}

void BuildPointsOnLineCache()
{
	for (int a = 1; a <= m; ++a)
	{
		for (int b = a; b <= m; ++b)
		{
			g_PointsOnLine[b*m + a] = CountPointsOnLine(a, b);
		}
	}
}

int GetPointsOnLine(int a, int b)
{
	// NB The cache includes the end points
	if (a > b)
		return g_PointsOnLine[a*m + b];
	return g_PointsOnLine[b*m + a];
}

// This code is borrowed from stackoverflow, or somewhere similar.
// It fails for large numbers, but works fine for the smaller size of this problem
bool IsPerfectSquare(ULL n)
{
	if ((n & 7ull) == 1ull || (n & 31ull) == 4ull || (n & 127ull) == 16ull || (n & 191ull) == 0ull)
	{
		// return n is probably square
		// so test it properly!
		ULL nroot = static_cast<ULL>(sqrt(static_cast<double>(n)) + 0.5);
		return nroot * nroot == n;
	}
	//else
	//return n is definitely not square
	return false;
}

void Solve()
{
	int validQuadCount = 0;

	// This performs a lot of redundant calculations, ignoring symmetry but code runs really quickly,
	// anyway, so I haven't bothered to speed it up.
	for (int a = 1; a <= m; ++a)
	{
		for (int b = 1; b <= m; ++b)
		{
			for (int c = 1; c <= m; ++c)
			{
				for (int d = 1; d <= m; ++d)
				{
					int pointsOnPerimeter = GetPointsOnLine(a, b) + GetPointsOnLine(a, d) +
						GetPointsOnLine(c, b) + GetPointsOnLine(c, d) - 4;
					// Subtract the double-counted line end points (8), then add 4 verts, for - 4
					int areaTimesTwo = (a * b) + (a * d) + (c * b) + (c * d);
					int twoI = areaTimesTwo - pointsOnPerimeter + 2;
					if (twoI % 2 == 0)
					{
						if (IsPerfectSquare(twoI / 2))
						{
							++validQuadCount;
						}
					}
				}
			}
		}
	}
	cout << "Answer: " << validQuadCount << endl;
}
int main()
{
	BuildPointsOnLineCache();
	Solve();
	return 0;
}
