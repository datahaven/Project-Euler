// Project Euler 88
// Minimal Product-Sum Sets
// Adrian Dale 01/08/2012

/*
Congratulations, the answer you gave to problem 88 is correct.

You are the 3105th person to have solved this problem.

You have earned 1 new award:

    Centurion: Solve one hundred consecutive problems
*/

// Answer = 7587457

// This is the messy version of the code, including all of my scrap code that I wrote
// to get ideas.

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

// Useful info: http://www-users.mat.umk.pl/~anow/ps-dvi/si-krl-a.pdf

const int KMAX = 12000;
int bestkValues[KMAX+2];

// I have no idea how to solve this!
// Writing some noddy code to work out some more values
void test01()
{
	const int NMAX = 15;
	for(int a=1; a<NMAX; ++a)
		for(int b=a; b<NMAX; ++b)
			for(int c=b; c<NMAX; ++c)
				for(int d=c; d<NMAX; ++d)
					for(int e=d; e<NMAX; ++e)
						for(int f=e; f<NMAX; ++f)
							for(int g=f; g<NMAX; ++g)
							{
								for(int h=g; h<NMAX; ++h)
								{
									for(int i=h; i<NMAX; ++i)
									{
										for(int k=i; k<NMAX; ++k)
										{
										if (a+b+c+d+e+f+g+h+i+k == a*b*c*d*e*f*g*h*i*k)
										{
											cout << "ps=" << a+b+c+d+e+f+g+h+i+k << endl;
											cout << a << " " << b << " " << c << " "
												<< d << " " << e << " " << f << " "
												<< g << " " << h << " " << i << " " << k << endl;
										}
										}
									}
								}
							}

}

void combs(int digitNo, int maxDigits, int *digits)
{
	cout << "combs(" << digitNo << ", " << maxDigits << ", ";
	int product = 1;
	int sum = 0;
	for(int i=0; i<maxDigits; ++i)
	{
		product *= digits[i];
		sum += digits[i];
		cout << digits[i] << " ";
	}
	int k=maxDigits-sum +product;
	cout << ")" << " sum=" << sum << " prod=" << product << endl;

	


	for(int i=0; i<maxDigits; ++i)
	{
		int index = maxDigits - 1 - i;
		while( digits[index] < digitNo - i)
		{
			digits[index]++;
			
			product = 1;
			sum = 0;
			for(int i=0; i<maxDigits; ++i)
			{
				product *= digits[i];
				sum += digits[i];
			}
			
			
			for(int j= index + 1; j<maxDigits; ++j)
			{
				digits[j] = digits[j - 1] /*+ 1*/;
			}
			combs(digitNo, maxDigits, digits);
		}
	}
}

// Find all factorisations of length N
void allNLengthFacs(int N)
{
	int Digits[20];
	for(int i=0; i<N; ++i)
		Digits[i] = 1;
	
	const int NMAX = 15;

	combs(3, 3, Digits);
/*	
		int product = 1;
		int sum = 0;
		for(int i=0; i<N; ++i)
		{
			product *= Digits[i];
			sum += Digits[i];
		}
		int k=N-sum+product;
		*/

	
}

bool incDigits(int maxDigits, int maxDigitVal, int *digits)
{
	int incIndex = maxDigits-1;
	
	digits[incIndex]++;
	
	bool carry = digits[incIndex] > maxDigitVal;

	while(carry)
	{
		incIndex--;
		if (incIndex >= 0)
		{
			digits[incIndex]++;
			carry = digits[incIndex] > maxDigitVal;
		}
		else
			return false; // overflow

		for(int j=incIndex+1; j<maxDigits; ++j)
		{
			digits[j] = digits[j-1];
		}
	}
	
	return true;
}

bool incDigitsII(int maxDigits, int maxDigitVal, int *digits)
{
	const int maxprod = 24000;

	int incIndex = maxDigits-1;
	
	digits[incIndex]++;
	
	// See if we can carry straight away
	int product = 1;
	for(int i=0; i<maxDigits; ++i)
	{
		product *= digits[i];
	}

	bool carry = (digits[incIndex] > maxDigitVal) || product > maxprod;

	while(carry)
	{
		incIndex--;
		if (incIndex >= 0)
		{
			digits[incIndex]++;
			carry = digits[incIndex] > maxDigitVal;
		}
		else
			return false; // overflow

		for(int j=incIndex+1; j<maxDigits; ++j)
		{
			digits[j] = digits[j-1];
		}

		product = 1;
		for(int i=0; i<maxDigits; ++i)
		{
			product *= digits[i];
		}
		carry |= (product > maxprod);
	}
	
	return true;
}


void test02()
{
	// How to make the generic version of this?
	for(int a=1; a<4; ++a)
		for(int b=a; b<4; ++b)
			for(int c=b; c<4; ++c)
				cout << a << " " << b << " " << c << endl;
	// Easy - see incDigits
	cout << "===" << endl;

	int digits[50];
	for(int i=0; i<sizeof(digits)/sizeof(digits[0]); ++i)
	{
		digits[i] = 1;
	}

	int callcount = 0;
	do
	{
		++callcount;

		int product = 1;
		for(int i=0; i<3; ++i)
		{
	//		cout << digits[i] << " ";
			product *= digits[i];
		}
	//	cout << " prod=" << product << endl;

	} while(incDigitsII(15, 24000, digits));

	cout << "callcount=" << callcount << endl;
}

void test03()
{
	int digits[15];

	int digitCount = 2;
	
	int callcount = 0;

	while(digitCount < 16)
	{
		for(int i=0; i<15; ++i)
			digits[i] = 1;

		do
		{
			int sum=0;
			int product = 1;
			for(int i=0; i<digitCount; ++i)
			{
				sum += digits[i];
				product *= digits[i];
			}
			int k = digitCount - sum + product;
			if (k <= KMAX && k >= 2)
			{
				if (bestkValues[k] > product)
					bestkValues[k] = product;
			}

			++callcount;
		} while( incDigitsII(digitCount, 24000, digits) );

		++digitCount;
	}

	cout << "callcount=" << callcount << endl;
}

int main()
{
	for(int k=0; k<KMAX; ++k)
	{
		bestkValues[k] = 25000;
	}

	test03();

	/*
	int sum = 0;
	for(int k=2; k<=10; ++k)
	{
		cout << "k=" << k << " => " << bestkValues[k] << endl;
		sum += bestkValues[k];
	}
	cout << "Answer=" << sum << endl;
	*/
	// No! Needs to be sum of unique bestkValues

	vector<int> kVector(bestkValues+2, bestkValues+KMAX+1);
	sort(kVector.begin(), kVector.end());
	auto it = unique(kVector.begin(), kVector.end());
	kVector.resize( it - kVector.begin() );

	int sum = accumulate(kVector.begin(),kVector.end(),0);
	cout << "Answer=" << sum << endl;

	//allNLengthFacs(3);
	
}