#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> permDigits;
vector<int> products;

// Caching 10^x her to save casting to and from floats to use pow()
int tenPow[13];

// Not really required - curiosity only

int solutionCount = 0;

int buildNo(int x, int y)
{
	int result = 0;
	
	for( int i=x; i<y; ++i)
	{
		result += tenPow[y-i-1] * permDigits[i];
	}
	return result;
}

// Pushes all possible products onto our list
void testPerm()
{
	// March through all possible * positions
	// and = positions
	for( int xp=1; xp<7;++xp)
	{
		for( int ep=xp+1;ep<9;++ep)
		{
			int a = buildNo(0,xp);
			int b = buildNo(xp, ep);
			int c = buildNo(ep, 9);

			if ( a*b == c)
			{
				products.push_back(c);
				// cout << c << endl;
				++solutionCount;
			}

			// cout << a << " * " << b << " = " << c << endl;
		}
	}
}

int main()
{
	for(int i=1;i<10;++i)
		permDigits.push_back(i);

	tenPow[0] = 1;
	for(int i=1; i<sizeof(tenPow)/sizeof(int); ++i)
	{
		tenPow[i] = tenPow[i-1] * 10;
	}

	do
	{
		testPerm();
	}
	while ( next_permutation(permDigits.begin(), permDigits.end()) );
	
	/*for( vector<int>::iterator it = permDigits.begin(); it < permDigits.end(); ++it )
			{
				cout << *it;
			}
	cout << endl << endl;*/
	
	cout << "Found " << solutionCount << " solutions" << endl;

	sort(products.begin(), products.end());
	for( vector<int>::iterator it = products.begin(); it < products.end(); ++it )
	{
				cout << *it << endl;
	}

	// Laziness - added them up by hand, cos couldn't be bothered to look up how to
	// remove duplicates and sum up
	/*Found 18 solutions
4396
4396
5346
5346
5346
5346
5796
5796
5796
5796
6952
6952
7254
7254
7632
7632
7852
7852*/

#if 0
	In J:
[hide code]

p=:   (10+i.90) (*/ ,&.>&,,&.>/)  100+i. 900
p=: p,( 1+i. 9) (*/ ,&.>&,,&.>/) 1000+i.9000
p=:   p #~ 1e4 > {.&>p
p=: > p #~ '   123456789' -:"1 /:~"1 ":>p
+/~.{."1 p


A 1-9 pandigital product is a 2-digit number
times a 3-digit number, or a 1-digit number
times a 4-digit number, with a product less
than 1e4. (3 2 and 4 1 also works, but don't
give any extra products.) 
#endif

#if 0
J:
pd=. ((-:&'123456789')@(/:~)@(9&":))"0
p =. pd ,(,&.": ,&.": *)"0/~2+i.1999
+/ ~.p# ,*"0/~2+i.1999
45228 
#endif
	return 0;
}
