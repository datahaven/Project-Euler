// Project Euler 144
// Adrian Dale
// 08/10/2011
//
// Had some "help" from http://0x5a4d.blogspot.com/2009/03/projecteuler-problem-144.html
// Not yet sure how helpful it actually is - may be incorrect.
// It was. Doh!
//
// Congratulations, the answer you gave to problem 144 is correct.
//
// You are the 1347th person to have solved this problem.
// Answer: 354
#include <iostream>
#include <cmath>
using namespace std;

// Ray Start
const double srsx = 0.0;
const double srsy = 10.1;
// Ray Start Impact Point
const double srsix = 1.4;
const double srsiy = -9.6;

void quadratic(double a, double b, double c, double &r1, double &r2)
{
	double b2m4ac = (b*b)-(4.0*a*c);
	if (b2m4ac < 0.0)
	{
		cout << "ERROR - complex roots" << endl;
		exit(-1);
	}
	r1 = (-b - sqrt(b2m4ac))/(2.0*a);
	r2 = (-b + sqrt(b2m4ac))/(2.0*a);

}

void test01(double rsx, double rsy, double rsix, double rsiy, double &nx, double &ny)
{
	double m0 = (rsiy-rsy)/(rsix-rsx);
	double m1 = -4.0*rsix/rsiy;

	double Q = (m0-m1)/(1+(m0*m1));
	double m2 = (m1-Q)/(1+(Q*m1));
	
	double a = m2;
	double b = rsiy-(m2*rsix);

	double qa = (a*a)+4.0;
	double qb = 2.0*a*b;
	double qc = (b*b)-100.0;

	double r1 = 0.0;
	double r2 = 0.0;

	quadratic(qa,qb,qc,r1,r2);

	// decide which root to use
	double ny1 = 100.0 - (4.0*r1*r1);
	double ny2 = 100.0 - (4.0*r2*r2);
	
	if (ny1 < 0.0 && ny2 < 0.0)
	{
		cout << "ERROR 2: " << r1 << " " << r2 << endl;
		exit(-1);
	}

	if ( ny1 > 0.0 && ny2 < 0.0)
	{
		cout << "r1" << endl;
		ny = sqrt(ny1);
		nx = r1;
	}
	else if (ny2 > 0.0 && ny1 < 0.0)
	{
		cout << "r2" << endl;
		ny = sqrt(ny2);
		nx = r2;
	}
	else
	{
		// both roots are positive
		// We know the line intersects at the point it bounced off, so
		// select the root that is NOT the xcoord of that point
		if ( abs(abs(rsix)-abs(r1)) < 0.0001 )
			nx = r2;
		else
			nx = r1;

		if ( !( (abs(abs(rsix)-abs(r1)) < 0.0001) || (abs(abs(rsix)-abs(r2)) < 0.0001)) )
			cout << "intersect assumption wrong" << endl;

		ny = (a*nx + b);
		// slope back to intersect point should be same slope as m2, otherwise
		// we've picked the wrong y
		double ns = (ny-rsiy)/(nx-rsix);
		//if ( abs( abs(ns)-abs(m2) ) < 0.0001 )
		//	ny *= -1.0;
		
	}

	cout << nx << " " << ny << endl;

}

int main()
{
	double nx=0.0;
	double ny=0.0;

	unsigned long count = 0;
	double sx = srsx;
	double sy = srsy;
	double ex = srsix;
	double ey = srsiy;

	while( true )
	{
		test01(sx, sy, ex, ey, nx, ny);
		++count;
		if (ny > 0.0 && nx >= -0.01 && nx <= 0.01)
		{
			break;
		}
		sx = ex; sy = ey;
		ex = nx; ey = ny;
		
	};

	cout << "Reflections=" << count << endl;

	return 0;
}
