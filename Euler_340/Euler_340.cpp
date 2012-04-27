// Project Euler 340 - Crazy Function
// Adrian Dale
// 29/05/2011
//
// This file is just test code.
// The working solution is written in Python in Euler_340.py inside this
// directory

#include <iostream>

#include <string>
#include <sstream>
#include <windows.h> // For QPC timer

using namespace std;

// Timer code from Stephan T. Lavavej's Nurikabe solver
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

typedef unsigned long long INTTYPE;

INTTYPE bigmod = 10000000000000;

//INTTYPE a = 500;
//INTTYPE b = 20000;
//INTTYPE c = 9;

// 21^7 = 1801088541
// NB This value is NOT 7^21 - stupid J was wrong
// 7^21 = 558545864083284030
// 12^7 = 35831808

INTTYPE a = 1801088541;
INTTYPE b = 558545864083284030;
INTTYPE c = 35831808;

INTTYPE callcount = 0;
INTTYPE callleafcount = 0;

bool verbose = false;

INTTYPE f(INTTYPE n)
{
	++callcount;

	//cout << n << " ";
	if ( n > b )
	{
		if (verbose)
			cout << "f(" << n << ") returning n-c: " << n-c << endl;
		++callleafcount;
		return n-c;
	}
	INTTYPE fn = f(a + f(a + f(a + f(a+n))));
	if (verbose)
	{
		cout << "f(" << n << ") returning f(a + f(a + f(a + f(a+" << n << ")))) = " << fn 
			<< " callleafcount=" << callleafcount << endl;
	}
	return fn;
}

INTTYPE fa(INTTYPE n)
{
	// Never happens?
	if ( n > b )
		return n-c;

	//INTTYPE leafcount = (b / (a+((n+a-1)/a)) )*3 + 4;
	INTTYPE leafcount;
	
	//leafcount = (n+a-1)/a;
	//leafcount = ((b/a)-leafcount)*3+4;

	leafcount = (((b-n)/a))*3+4;

	INTTYPE f0 = (leafcount-1) * (a-c);
	f0 = f0 % bigmod;
	INTTYPE base;
	if ( n%a == 0 )
	{
		base = a-(b%a)+b-c;
	}
	else
	{
		base = n + a -c + ((b-n)/a)*a  ;
	}
	base = base % bigmod;
	f0 += base;
	f0 = f0 % bigmod;
	if (verbose)
		cout << "fa=" << f0 << " leafcount=" << leafcount << " base=" << base << endl;
	return f0;
}

inline INTTYPE funb(INTTYPE n)
{
	return ((((((b-n)/a)+1)*3)+1)*(a-c))+b-((b-n)%a);
}

inline INTTYPE sumn(INTTYPE n)
{
	return (n*(n+1))/2;
}

// Make the right calls to f
INTTYPE test02()
{
	INTTYPE s = 0;

	// First block runs from f(0) to f(b%a)
	INTTYPE start = fa(0);
	callleafcount = 0;
	//INTTYPE start2 = fa(0);

	//cout << "f(0)=" << start << " fa(0)=" << start2 << (start == start2 ? " OK" : " ERROR") << endl;
	INTTYPE base = start;
	INTTYPE sum = 0;
	//for(INTTYPE i=0; i<=b%a; ++i)
	//	sum += i;
	sum = sumn(b%a);
	s += base*(1+(b%a)) + sum;

	sum = 0;
	//for(INTTYPE i=0; i<a; ++i)
	//	sum += i;
	sum = sumn(a-1);

	for(INTTYPE n=1+(b%a); n<=b; n+=a)
	{
		callleafcount = 0;
		base = fa(n);
		//INTTYPE base2 = fa(n);
		//cout << "f(" << n << ")=" << base << " fa()=" << base2 << (base == base2 ? " OK" : " ERROR") << endl;
		s += base*a + sum;
	}

	return s;
}

// Make the right calls to f
// Allow for overflow
INTTYPE test03()
{
	INTTYPE s = 0;

	// First block runs from f(0) to f(b%a)
	INTTYPE base = funb(0);
	callleafcount = 0;
	
	INTTYPE sum = 0;
	sum = sumn((b%a)-c);
	//sum = sumn(b%a);
	sum = sum % bigmod;
	s += base*(1+(b%a)-c);
	s = s % bigmod;
	s += sum;
	s = s % bigmod;

	sum = sumn(a-1) % bigmod;
	INTTYPE n;
	for(n=1+(b%a)-c; n<=b; n+=a)
	{
		base = funb(n);
		base = (base*a) % bigmod;
		s += base;
		s = s % bigmod;
		s += sum;
		s = s % bigmod;
	}

	//cout << "remaining n=" << n << endl;

	return s;
}

INTTYPE test01()
{
	INTTYPE s=0;
	// f0
	//for(INTTYPE n=1+(b%a); n<=b; n+=a)
	for(INTTYPE n=0; n<=b; ++n)
	{
		callleafcount = 0;
		INTTYPE fn = f(n);
		INTTYPE fan = fa(n);
		s += fn;
		//cout << "f(" << n << ")=" << fn << " fa()=" << fan << " s=" << s << endl;
		if ( fn != fan )
		{
			cout << "f(" << n << ")=" << fn << " fa=" << fan << " calleafcount=" << callleafcount << endl;
			cout << "ERROR" << endl;
		}
		//else
		//	cout << "OK: " << n << endl;
	}

	return s;
}

void test04()
{
	for(a=10; a<100; ++a)
	{
		for(b=a; b<3000; b+=17)
		{
			for(c=10; c<a; ++c)
			{
				INTTYPE t1 = test01();
				INTTYPE t3 = test03();
				if ( t1 != t3 )
				{
					cout << "Mismatch: a=" << a << " b=" << b << " c=" << c << endl;
					return;
				}
				else
					cout << "OK: a=" << a << " b=" << b << " c=" << c << endl;
			}
		}
	}
}

void test05()
{
	//a = 50;
	//b = 2000;
	//c = 40;
	INTTYPE prev=0;
	INTTYPE sum=0;
	for( INTTYPE n=0; n<=b; ++n)
	{
		callleafcount = 0;
		
		INTTYPE fan=funb(n);
		
		//cout << "f(" << n << ")=" << fan << endl;
		if ( fan != prev+1 )
			cout << "BREAK " <<  n << " f(n)=" << fan << " sum=" << sum << endl;
		sum += fan;
		sum = sum % bigmod;
		prev = fan;
	}
}

INTTYPE test06()
{
	INTTYPE s=0;
	// f0
	//for(INTTYPE n=1+(b%a); n<=b; n+=a)
	for(INTTYPE n=1611673673; n<=1611673676; ++n)
	{
		callleafcount = 0;
		INTTYPE fan = fa(n);
		s += fan;
		cout << "f(" << n << ")=" << fan << " s=" << s << endl;	
	}

	return s;
}

INTTYPE test07()
{
	INTTYPE s = 0;

	// First block runs from f(0) to f(b%a)
	INTTYPE base = funb(0);
	cout << "f(0)=" << base << endl;

	callleafcount = 0;
	
	INTTYPE sum = 0;
	sum = sumn(1611673675-1);
	sum = sum % bigmod;
	s += base*(1611673675);
	s = s % bigmod;
	s += sum;
	s = s % bigmod;

	cout << "f(1611673674)=" << fa(1611673674) << endl;
	cout << "sum so far=" << s << endl;

	sum = sumn(a-1) % bigmod;
	cout << "sumn(a-1)=" << sum << endl;

	INTTYPE n;
	for(n=1611673675; n<=b; n+=a)
	{
		cout << "n=" << n << " sum=" << s << endl;
		base = funb(n) % bigmod;
		base = (base*a) % bigmod;
		s += base;
		s = s % bigmod;
		s += sum;
		s = s % bigmod;
		
	}

	//cout << "remaining n=" << n << endl;

	return s;
}

int main()
{
	//cout << "test01()=" << test01() << endl;
	const long long start = counter();
	cout << "test07()=" << test07() << endl;

	//test05();

	const long long finish = counter();
	cout << "Time Taken: " << format_time(start, finish) << endl;
	return 0;
}