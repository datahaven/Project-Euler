// Project Euler 587
// Concave Triangle
// by Adrian Dale
// 28/01/2017

// A strangely easy problem to solve.
// Answer is a four digit number, much lower than I expected.
// Didn't even need to do a binary search to find it

#include <iostream>

using namespace std;

const double PI = 3.14159265359;

double IntersectXPosition(double n)
{
	// Returns x value for where line intersects curve
	double a = 1.0f + n * n;
	double b = -2.0f - 2.0f * n;
	double c = 1.0f;
	return (-b - sqrt(b*b - 4.0f * a * c)) / (2.0f*a);
}

double IntersectYPosition(double x)
{
	return 1.0f - sqrt(2.0f * x - x * x);
}

double TriangleArea(double x, double y)
{
	return x * y * 0.5f;
}

double CurveArea(double x, double y)
{
	double angle = atan2(1.0f - x, 1.0f - y);
	double segmentArea = (angle / (2.0f * PI)) * PI;
	double segmentTriangleArea = (1.0f - y) * (1.0f - x) * 0.5f;
	return ((1.0f - x) * y) - (segmentArea - segmentTriangleArea);
}
double Area(double n)
{
	double x = IntersectXPosition(n);
	double y = IntersectYPosition(x);
	double ta = TriangleArea(x, y);
	return ta + CurveArea(x, y);
}

double AreaAsPercent(double n)
{
	// I think I've got x and y back to front somewhere, as I've ended up with the percentage
	// or the area above the line. Or maybe I should have used asin, instead of atan?
	double a = Area(n);
	return 100.0f - (100.0f * a / ((4.0f - PI)/4.0f));
}

void test()
{
	double n = 1.0f;
	for (int i = 0; i < 20; ++i)
	{
		cout << "AreaAsPercent(" << n << ")=" << AreaAsPercent(n) << endl;
		n = n + 1.0f;
	}
}

double FindUpperBound(double target)
{
	// Find a value for n where the area is less than the target
	double n = 100.0f;
	while (true)
	{
		double a = AreaAsPercent(n);
		cout << "AreaAsPercent(" << n << ")=" << a << endl;
		if (a < target)
		{
			cout << "Target Hit at n=" << n << endl;
			return n;
		}
		n = n + 100.0f;
	}
	return -1.0f;
}

double FindAnswer(double target)
{
	// Find a value for n where the area is less than the target
	double n = 2200.0f;
	while (true)
	{
		double a = AreaAsPercent(n);
		cout << "AreaAsPercent(" << n << ")=" << a << endl;
		if (a < target)
		{
			cout << "Target Hit at n=" << n << endl;
			return n;
		}
		n = n + 1.0f;
	}
	return -1.0f;
}
int main()
{
	FindAnswer(0.1f);
	return 0;
}