// Project Euler 101
// http://projecteuler.net/problem=101
// Adrian Dale 22/01/2014

/*
Notes:

This was very helpful and is the method my code uses:
http ://mathforum.org/library/drmath/view/56864.html

Congratulations, the answer you gave to problem 101 is correct.

You are the 5228th person to have solved this problem.

Return to Problems page.

This is the rough and ready solution, complete with awful C++ code.
(Polynomial isn't implemented properly as a canonical type and the arithmetic
functions shouldn't really be implemented the way I've done them. It got
something out working quickly, though!)

My code calculates one more level than needed. It doesn't affect the end result
but does allow me to print out the final calculated polynomial and show that it
matches the given polynomial.

I used PARI/GP to produce correct values to check my code with
> mypoly = Pol([1,-1,1,-1,1,-1,1,-1,1,-1,1])
> x = 9
> eval(mypoly)
*/

#include <iostream>
#include <map>
#include <vector>
using namespace std;

// TODO - Fix lazy hardcoded polynomial size
const int MAX_POLY = 15;

typedef signed long long LL;

LL givenPolyExponents[] = {1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1};

// The example Euler gives us
//LL givenPolyExponents[] = { 0,0,0,1 };

// Cut and pasted (and modified) from stackoverflow
// http://stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int
// A bug where I forgot to change "int result" to "LL result" took me quite a while
// to track down! Would have found it if I'd been paying attention to warnings.
LL ipow(LL base, LL exp)
{
	LL result = 1;
	while (exp)
	{
		if (exp & 1)
			result *= base;
		exp >>= 1;
		base *= base;
	}

	return result;
}

class Polynomial
{
public:
	Polynomial();
	~Polynomial() {};
	void setCoefft(LL c, LL n);
	LL getCoefft(LL n);
	void display();
	LL evaluate(LL x);
	void levelUp(LL n);
	void timesConst(LL c);
	void add(Polynomial &p2);
private:
	vector<LL> mPoly;
};

Polynomial::Polynomial()
{
	for (int i = 0; i < MAX_POLY; ++i)
		mPoly.push_back(0);
}

void Polynomial::setCoefft(LL c, LL n)
{
	// TODO - check size and expand if necessary
	mPoly[c] = n;
}

LL Polynomial::getCoefft(LL n)
{
	return mPoly[n];
}

void Polynomial::display()
{
	// Pretty crude print routine - adds an extra plus at the end.
	for (int i = 0; i < mPoly.size(); ++i)
	{
		if (mPoly[i] != 0)
		{
			cout << mPoly[i];
			if (i>0)
			{
				cout << "x";
				if (i > 1)
					cout << "^" << i;
			}
			cout << " + ";
		}
	}
	cout << endl;
}

void Polynomial::add(Polynomial &p)
{
	for (int i = 0; i < mPoly.size(); ++i)
	{
		mPoly[i] += p.mPoly[i];
	}
}

void Polynomial::timesConst(LL c)
{
	for (int i = 0; i < mPoly.size(); ++i)
	{
		mPoly[i] *= c;
	}
}

LL Polynomial::evaluate(LL x)
{
	LL result = 0;
	for (LL i = 0; i < mPoly.size(); ++i)
	{
		result += mPoly[i] * ipow(x, i);
	}
	return result;
}

// Multiplies the polynomial by (x-n)
// Horribly crude but saved me having to write a proper
// multiplication routine!
void Polynomial::levelUp(LL n)
{
	// The times x part:
	vector<LL> p1 = mPoly;
	for (int i = mPoly.size() - 2; i >= 0; --i)
	{
		p1[i + 1] = p1[i];
	}
	p1[0] = 0;
	

	// The times -n part:
	vector<LL> p2 = mPoly;
	for (int i = 0; i < mPoly.size(); ++i)
	{
		p2[i] = p2[i] * n * -1;
	}
	
	// Add them together
	for (int i = 0; i < mPoly.size(); ++i)
	{
		mPoly[i] = p1[i] + p2[i];
	}
}

LL findFIT(Polynomial &p1, Polynomial &p2)
{
	LL FIT = 0;
	bool found = false;
	for (int n = 1; n < MAX_POLY; ++n)
	{
		LL p1e = p1.evaluate(n);
		if (p1e != p2.evaluate(n))
		{
			cout << "FIT at n=" << n << endl;
			return p1e;
		}
	}
	cout << "FIT not found!" << endl;
	return 0;
}

void solve()
{
	Polynomial Seq;
	for (LL i = 0; i < sizeof(givenPolyExponents) / sizeof(givenPolyExponents[0]); ++i)
	{
		Seq.setCoefft(i, givenPolyExponents[i]);
	}
	//Seq.display();

	vector<Polynomial> OP;
	Polynomial OP1;
	OP1.setCoefft(0, Seq.evaluate(1));
	cout << "Poly 1:" << endl;
	OP1.display();
	OP.push_back(OP1);

	// f_1(x) = (x-1)(a2-a1) + a1
	Polynomial OP2;
	LL a1 = Seq.evaluate(1);
	LL a2 = Seq.evaluate(2);
	OP2.setCoefft(0, -1 * a2 + 2 * a1);
	OP2.setCoefft(1, a2 - a1);
	OP.push_back(OP2);
	cout << "Poly 2:" << endl;
	OP2.display();

	// (x-1)
	Polynomial multiplierPoly;
	multiplierPoly.setCoefft(0, -1);
	multiplierPoly.setCoefft(1, 1);
	cout << "multiplierPoly (x-1):" << endl;
	multiplierPoly.display();
	
	int level = 2;
	while (level <= 10)
	{
		cout << "Level " << level << ":" << endl;

		// Multiply multiplierPoly by (x-level)
		multiplierPoly.levelUp(level);
		cout << " muliplierPoly:" << endl;
		multiplierPoly.display();

		LL y = Seq.evaluate(level + 1);
		LL bMult = multiplierPoly.evaluate(level+1);
		LL fPrev = OP[level - 1].evaluate(level + 1);
		LL b = (y - fPrev) / bMult;

		Polynomial nextPoly = multiplierPoly;
		nextPoly.timesConst(b);
		nextPoly.add(OP[level - 1]);
		OP.push_back(nextPoly);
		cout << "Next Poly:" << endl;
		nextPoly.display();
		cout << endl;
		++level;
	}

	LL FITsum = 0;
	for (size_t i = 0; i < OP.size(); ++i)
	{
		FITsum += findFIT(OP[i], Seq);
	}
	cout << "Answer=" << FITsum << endl;
}

int main()
{
	cout << "Euler 101 - Adrian Dale" << endl;
	
	solve();

	return 0;
}
