// Project Euler 75
// Adrian Dale
// 08/03/2012
/*
Congratulations, the answer you gave to problem 75 is correct.

You are the 5519th person to have solved this problem.

You have earned 1 new award:

    Pythagorean Triplet: Solve three Pythagorean problems

Return to Problems page or go to thread 75 in the forum.
*/

#include <iostream>
using namespace std;
typedef unsigned long long ull;
struct Triple
{
	ull a;
	ull b;
	ull c;
};

const int L = 1500000;

Triple found[L+1];

ull gcd(ull a, ull b)
{
    while( 1 )
    {
        a = a % b;
		if( a == 0 )
			return b;
		b = b % a;

        if( b == 0 )
			return a;
    }
}

// http://www.mathblog.dk/project-euler-75-lengths-of-wire-right-angle-triangle/
const int limit = 1500000;
int triangles[limit+1];
void test02()
{ 
	int result =0;
	int mlimit = 866;
 
	int tcount = 0;
for (long m = 2; m < mlimit; m++) {
    for (long n = 1; n < m; n++) {
        if (((n + m) % 2) == 1 && gcd(n, m) == 1) {
            long a = m * m + n * n;
            long b = m * m - n * n;
            long c = 2 * m * n;
            long p = a + b + c;
            while(p <= limit){
				++tcount;
                triangles[p]++;
                if (triangles[p] == 1) result++;
                if (triangles[p] == 2) result--;
                p += a+b+c;
            }
        }
    }
}
cout << "tcount=" << tcount << endl;
cout << "res=" << result << endl;

}

int main()
{
	test02();
	//exit(0);

	// Set record of triples found to -1.
	// This means we don't yet know if this perimeter
	// has any triples
	for(int i=0; i<=L; ++i)
	{
		found[i].a = -1;
		found[i].b = -1;
		found[i].c = -1;
	}

	int tcount = 0;
	// Generate all triples
	
	for(int n=1; n<1226; ++n)
	{
		for(int m=n+1; m<1226; ++m)
		{
			if ( (m-n)%2 == 0 || gcd(m,n) != 1)
				continue;
			// Problem was here -> k needs to be really big sometimes.
			// Other solutions didn't have k but just repeatedly added the
			// perimeter to itself.
			// Should have known - 3 4 5 has lots of multiples less than 1500000
			for(int k=1; k<1226000; ++k)
			{
				Triple t;
				t.a = (m*m - n*n)*k;
				t.b = 2*m*n*k;
				t.c = (m*m + n*n)*k;

				ull perimeter = t.a + t.b + t.c;

				if (perimeter > L)
					break;
				++tcount;
				// These don't always come out in order, so swap them
				// into ascending order. Otherwise we see 6,8,10 and 8,6,10 as
				// different triangles
				if (t.a < t.b) {
					if (t.c < t.a) swap(t.a,t.c);
				} else {
					if (t.b < t.c) swap(t.a,t.b);
					else swap(t.a,t.c);
				} 
				if(t.c<t.b) swap(t.b,t.c);

				//if (perimeter == 120)
				//	cout << "      => a=" << t.a << " b=" << t.b << " c=" << t.c << endl;

				if (found[perimeter].a == -1)
				{
					found[perimeter] = t;
					//if (t.a < 130)
					//	cout << "a=" << t.a << " b=" << t.b << " c=" << t.c << endl;
				}
				else
				{
					if (found[perimeter].a != -2 && (found[perimeter].a != t.a ||
														found[perimeter].b != t.b || 
														found[perimeter].c != t.c ))
					// Mark this perimeter as having multiple solutions
					found[perimeter].a = -2;
				}
			}
		}
	}

	cout << "tcount=" << tcount << endl;
	// See how many perimeters have a single solution
	int solcount = 0;
	for(int i=1; i<=L; ++i)
	{
		if (found[i].a != -1 && found[i].a != -2)
		{
			if (i < 130)
				cout << i << endl;
			++solcount;
		}
	}
	cout << "Solution Count = " << solcount << endl;
	return 0;
}