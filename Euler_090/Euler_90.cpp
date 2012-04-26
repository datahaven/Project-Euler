// Project Euler 90
// Adrian Dale 08/03/2012
// Answer: 1217
/*
Congratulations, the answer you gave to problem 90 is correct.

You are the 3276th person to have solved this problem.

Return to Problems page or go to thread 90 in the forum.
*/
#include <iostream>
#include <set>
using namespace std;

typedef unsigned long long ull;

#define IN_C1(d) (C1.find(d) != C1.end())
#define IN_C2(d) (C2.find(d) != C2.end())

#define CHECK1(d1,d2) if (! ((IN_C1(d1) && IN_C2(d2)) || (IN_C1(d2) && IN_C2(d1)))) return false;

#define CHECK2(d1)	if (! ((IN_C1(d1) && (IN_C2(9) || IN_C2(6)) ) || (IN_C2(d1) && (IN_C1(9) || IN_C1(6)) ) )) return false;

bool checkComb(int a1, int b1, int c1, int d1, int e1, int f1,
				int a2, int b2, int c2, int d2, int e2, int f2)
{
	set<int> C1;
	C1.insert(a1);	C1.insert(b1);	C1.insert(c1);
	C1.insert(d1);	C1.insert(e1);	C1.insert(f1);

	set<int> C2;
	C2.insert(a2);	C2.insert(b2);	C2.insert(c2);
	C2.insert(d2);	C2.insert(e2);	C2.insert(f2);

	CHECK1(0,1)
	CHECK1(0,4)
	CHECK2(0) // 09
	CHECK2(1) // 16
	CHECK1(2,5)
	CHECK2(3) // 36
	CHECK2(4) // 49
	CHECK2(4) // 64
	CHECK1(8,1)
	
	return true;
}

void solve()
{
	ull combCount = 0;
	ull sameCount = 0;
	for(int a=0; a<10; ++a)
	{
		for(int b=a+1; b<10; ++b)
		{
			for(int c=b+1; c<10; ++c)
			{
				for(int d=c+1; d<10; ++d)
				{
					for(int e=d+1; e<10; ++e)
					{
						for(int f=e+1; f<10; ++f)
						{
							//cout << a << " " << b << " " << c << " "
							//	<< d << " " << e << " " << f << endl;
							
	for(int a2=0; a2<10; ++a2)
	{
		for(int b2=a2+1; b2<10; ++b2)
		{
			for(int c2=b2+1; c2<10; ++c2)
			{
				for(int d2=c2+1; d2<10; ++d2)
				{
					for(int e2=d2+1; e2<10; ++e2)
					{
						for(int f2=e2+1; f2<10; ++f2)
						{
							if (checkComb(a,b,c,d,e,f,a2,b2,c2,d2,e2,f2))
							{
								++combCount;
								if (a==a2 && b==b2 && c==c2 && d==d2 && e==e2 && f==f2)
									++sameCount;
#if 0
								cout << "{" << a << "," << b << "," << c << ","
									<< d << "," << e << "," << f << "} , {"
									<< a2 << "," << b2 << "," << c2 << ","
									<< d2 << "," << e2 << "," << f2 << "}" << endl;
#endif
							}
						}
					}
				}
			}
		}
	}
						}
					}
				}
			}
		}
	}
	cout << "combCount=" << combCount << endl;
	cout << "sameCount=" << sameCount << endl;
	// Don't actually need to consider sameCount as we know there aren't
	// any valid combinations where both cubes are the same.
	// If there were then they'd only be counted once in combCount and
	// we'd have to allow for that. Instead we can just divide the count
	// by two.
	cout << "Answer = " << combCount / 2 << endl;
}

int main()
{
	solve();
	return 0;
}