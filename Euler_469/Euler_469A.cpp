// Project Euler 469
// http://projecteuler.net/problem=469
// Empty Chairs
// Adrian Dale 29/04/2014

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <random>
#include <map>
#include <algorithm>
#include <mpir.h>

using namespace std;

typedef unsigned long long ULL;
const long double EPSILON = 0.00000000000000000000000000000000000000000001;

default_random_engine generator;


ULL factorial(ULL n)
{
	ULL result = 1;
	for (int i = 1; i <= n; ++i)
		result *= i;
	return result;
}

void ULLToFP(mpf_t &outVal, ULL inVal)
{
	ostringstream ourstr;
	ourstr << setprecision(20) << inVal;

	// Assume init is called
	// mpf_init(outVal);

	mpf_set_str(outVal, ourstr.str().c_str(), 10);
}

void test01(ULL N)
{
	ULL A_Start = (3 - (N % 3)) % 3;
	ULL B_Start = (N-2*A_Start)/3;
	
	//cout << "N=" << N << " A=" << A << " B=" << B << endl;
	ULL solCount = 0;
	bool foundFirst = false;
	for (ULL a = 0; a <= N/2; ++a)
	{
		for (ULL b = 0; b <= N/3; ++b)
		{
			ULL check = 2 * a + 3 * b;
			if ( check == N)
			{
				cout << "N=" << N << " A=" << a << " B=" << b << endl;
				++solCount;
				if (foundFirst == false)
				{
					foundFirst = true;
					if (A_Start != a || B_Start != b)
					{
						cout << "ERROR! " << "A_Start=" << A_Start << " A=" << a << endl;
						return;
					}
				}
			}
			if (check > N) break;
		}
	}
	cout << "N=" << N << " solCount=" << solCount << endl;

	/* There seems to be a pattern here:
	N=6 solCount=2
N=10 solCount=2
N=11 solCount=2
N=12 solCount=3
N=10 solCount=2
N=100 solCount=17
N=1000 solCount=167
N=10000 solCount=1667
N=100000 solCount=16667
*/
/* In fact it's pretty straightforward - start with min A and Max B, then count A's up
	in 2's and B's down in 3's
	
	N = 15 A = 0 B = 5
		N = 15 A = 3 B = 3
		N = 15 A = 6 B = 1
		N = 15 solCount = 3
		N = 16 A = 2 B = 4
		N = 16 A = 5 B = 2
		N = 16 A = 8 B = 0
		N = 16 solCount = 3
		N = 17 A = 1 B = 5
		N = 17 A = 4 B = 3
		N = 17 A = 7 B = 1
		N = 17 solCount = 3
		N = 18 A = 0 B = 6
		N = 18 A = 3 B = 4
		N = 18 A = 6 B = 2
		N = 18 A = 9 B = 0
		N = 18 solCount = 4
		N = 19 A = 2 B = 5
		N = 19 A = 5 B = 3
		N = 19 A = 8 B = 1
		N = 19 solCount = 3

	Can ignore a bit of subtlety around whether or not N divides by 2 or 3 by making this
	only run for powers of 10. Or just calculate A/B properly.

	if N%3 == 0 then A=0
	else A = 3 - (N%3)

	or can write A = ((3-(N%3)) % 3
	*/
}

long double getPoC_OLD(ULL AB_Start, ULL AB_End, ULL chairCount)
{
	// Allow for the fact seating is in a circle
	chairCount -= 1;

	long double termSum = 1.0;
	long double cc = static_cast<long double>(chairCount);
	long double ccterm = cc;
	const long double EPSILON = 0.0000000000000001;
	while (true)
	{
		long double termBit = 1.0 / ccterm;
		termSum += termBit;
		ccterm *= cc - 1;
		cc -= 1.0;
		if (cc <= 0.0 || termBit < EPSILON || cc < AB_Start)
		{
			break;
		}

	}

	return 1.0 / termSum;
}

void getPoC_MPIR(mpf_t &percentOfCases, ULL AB_Start, ULL AB_End, mpf_t &chairCount)
{
	//cout << "getPoC_MPIR called with: " << AB_Start << " " << AB_End << endl;

	mpf_t oneConst;
	mpf_init(oneConst);
	mpf_set_d(oneConst, 1.0);

	// Allow for the fact seating is in a circle
	//chairCount -= 1;
	AB_Start -= 1;
	AB_End -= 1;
	mpf_sub(chairCount, chairCount, oneConst);
	
	mpf_t termSum;
	mpf_init_set_d(termSum, 0.0);
	
	mpf_t termBit;
	mpf_init_set_d(termBit, 1.0);

	mpf_t prevResult;
	mpf_init_set_d(prevResult, 0.0);
	mpf_t nextResult;
	mpf_init_set_d(nextResult, 0.0);

	mpf_t relDiff;
	mpf_init_set_d(relDiff, 0.0);

	mpf_t epsilon;
	mpf_init(epsilon);
	mpf_init_set_str(epsilon, "0.000000000000000000000000000000000000001", 10);


	for (ULL i = AB_End; i >= AB_Start; --i)
	{
		mpf_t iFP;
		mpf_init(iFP);
		ULLToFP(iFP, i);

		//if ( i > chairCount)
		if (mpf_cmp(iFP, chairCount) > 0)
		{
			mpf_t tempFP;
			mpf_init(tempFP);
			ULLToFP(tempFP, AB_End);
			mpf_sub(tempFP, tempFP, iFP);
			mpf_add(tempFP, tempFP, oneConst);
			mpf_add(tempFP, tempFP, chairCount);
			mpf_mul(termBit, termBit, tempFP);
			mpf_clear(tempFP);
			//termBit *= AB_End - i + 1 + chairCount;
			//cout << "A" << endl;
		}
		//else if (i < chairCount)
		else if (mpf_cmp(iFP, chairCount) < 0)
		{
			// termBit *= 1.0 / static_cast<long double>(i + 1);
			mpf_t tempFP;
			mpf_init(tempFP);
			mpf_set(tempFP, iFP);
			mpf_add(tempFP, tempFP, oneConst);
			mpf_div(tempFP, oneConst, tempFP);
			mpf_mul(termBit, termBit, tempFP);
			mpf_clear(tempFP);
			// termBit must be evaluating to zero here for large values of iFP??
			//cout << "termBit=";		mpf_out_str(NULL, 10, 30, termBit); cout << endl;
			//cout << "termSum=";		mpf_out_str(NULL, 10, 30, termSum); cout << endl;
			// termBit=0.2000000000000000004e-17
			//cout << "B" << endl;
		}
		else
		{
			mpf_set_d(termBit, 1.0);
			//cout << "C" << endl;
		}

		mpf_add(termSum, termSum, termBit);

		mpf_div(nextResult, oneConst, termSum);

		mpf_reldiff(relDiff, prevResult, nextResult);

		if (mpf_cmp(relDiff, epsilon) < 0)
		{
			break;
		}

		mpf_set(prevResult, nextResult);
	}

	mpf_set(percentOfCases, nextResult);

	mpf_clear(oneConst);
	mpf_clear(relDiff);
	mpf_clear(epsilon);
	mpf_clear(termBit);
	mpf_clear(termSum);
	mpf_clear(nextResult);
	mpf_clear(prevResult);
	
}

long double getPoC(ULL AB_Start, ULL AB_End, ULL chairCount)
{
	// Allow for the fact seating is in a circle
	chairCount -= 1;
	AB_Start -= 1;
	AB_End -= 1;

	long double termSum = 0.0;
	
	long double termBit = 1.0;
	long double prevResult = 0.0;
	for (ULL i = AB_End; i >= AB_Start; --i)
	{

		if (i > chairCount)
			termBit *= AB_End - i + 1 + chairCount;
		else if (i < chairCount)
			termBit *= 1.0 / static_cast<long double>(i+1);
		else
			termBit = 1.0;

		termSum += termBit;

		// This code is failing - seems like it doesn't accept tiny values of epsilon?
		// ie termBit is too small to be represented by long double when added to termSum
		if (fabs(prevResult - (1.0 / termSum)) < EPSILON)
		{
			break;
		}

		prevResult = 1.0 / termSum;
	}

	return 1.0 / termSum;
}

// This code produces the same results as my hand calculations.
// Clearly it won't work for the full size result as it uses factorial and
// also iterates for each possible solution.
// I don't even think this method will work as it needs totalChairLayouts before it
// can start and yet this is a massive number == factorial(10^18)
long double test02(ULL N)
{
	ULL A_Start = (3 - (N % 3)) % 3;
	ULL B_Start = (N - 2 * A_Start) / 3;

	ULL A_End = A_Start + (B_Start / 2) * 3;
	ULL B_End = B_Start % 2;
	//cout << "N=" << N << " A_Start=" << A_Start << " B_Start=" << B_Start << endl;
	//cout << "N=" << N << " A_End=" << A_End << " B_End=" << B_End << endl;

	ULL A = A_Start;
	ULL B = B_Start;
	long double expectation = 0.0;

	ULL totalChairLayouts = 0;
	while (true)
	{
		ULL chairCount = A + B;
		// Need chairCount-1 as first seat position is fixed
		totalChairLayouts += factorial(chairCount-1);
		
		//cout << "A=" << A << " B=" << B << " chairCount=" << chairCount << " totChairs=" << totalChairLayouts << endl;

		// Increase A by 3 and decrease B by 2 until B goes below 0
		// NB Take care as I'm using unsigned long longs
		if (B <= 1) break;
		B -= 2;
		A += 3;
	}

	A = A_Start;
	B = B_Start;

	while (true)
	{
		ULL chairCount = A + B;
		long double emptyChairFraction = (N - chairCount) / static_cast<long double>(N);
		long double percentOfCases = static_cast<long double>(factorial(chairCount-1)) / static_cast<long double>(totalChairLayouts);
		long double testPoC = getPoC(A_Start+B_Start, A_End+B_End, chairCount);
		expectation += emptyChairFraction * percentOfCases;

		//cout << "A=" << A << " B=" << B << setprecision(8) << " ECF=" << emptyChairFraction << " poc=" << percentOfCases << " tpoc=" << testPoC << " exp=" << expectation << endl;
		//cout << "tpoc=" << testPoC << endl;

		// Increase A by 3 and decrease B by 2 until B goes below 0
		// NB Take care as I'm using unsigned long longs
		if (B <= 1) break;
		B -= 2;
		A += 3;
	}

	return expectation;
	/* Notes
	Looking at the output from this fn, it looks like the bulk of the possible cases are for max(A)/min(B)
	so the answer would be to calculate that first, then keep going backwards until the terms don't affect
	the precision of the final result.

	N=15 A_Start=0 B_Start=5
	A=0 B=5 ECF=0.666667 poc=0.0277778 exp=0.0185185
	A=3 B=3 ECF=0.6 poc=0.138889 exp=0.101852
	A=6 B=1 ECF=0.533333 poc=0.833333 exp=0.546296

	N=16 A_Start=2 B_Start=4
	A=2 B=4 ECF=0.625 poc=0.0204082 exp=0.0127551
	A=5 B=2 ECF=0.5625 poc=0.122449 exp=0.0816327
	A=8 B=0 ECF=0.5 poc=0.857143 exp=0.510204

	N=17 A_Start=1 B_Start=5
	A=1 B=5 ECF=0.647059 poc=0.0204082 exp=0.0132053
	A=4 B=3 ECF=0.588235 poc=0.122449 exp=0.0852341
	A=7 B=1 ECF=0.529412 poc=0.857143 exp=0.539016

	ie notice poc (percent of cases) is biggest for high A, low B

	Now need to work out how to deal with the factorial - I'm sure I can cancel out the multiplications
	somewhere.
	I suspect I can't - this page suggests that I could ignore a lot of the lower factorials, though
	https://answers.yahoo.com/question/index?qid=20100326232928AAkiLcz&show=7#profile-info-EQwONWzaaa

	Unfortunately I think this still leaves me with massive numbers.
	I do have a clue from the Japanese guy that there is an approximation involved, though. Sounds like
	he was a bit disappointed, too! I'm sure my idea is sound - just need to work out if I can do it
	without needing factorials.
	*/
}


// MPIR Version
long double test03(ULL N)
{
	mpf_set_default_prec(10000);

	mpf_t epsilon;
	mpf_init(epsilon);
	mpf_init_set_str(epsilon, "0.00000000000000001", 10);

	ULL A_Start = (3 - (N % 3)) % 3;
	ULL B_Start = (N - 2 * A_Start) / 3;

	ULL A_End = A_Start + (B_Start / 2) * 3;
	ULL B_End = B_Start % 2;
	//cout << "N=" << N << " A_Start=" << A_Start << " B_Start=" << B_Start << endl;
	//cout << "N=" << N << " A_End=" << A_End << " B_End=" << B_End << endl;
	
	//ULL A = A_End;
	//ULL B = B_End;

	
	mpf_t A;
	mpf_init(A);
	ULLToFP(A, A_End);

	mpf_t B;
	mpf_init(B);
	ULLToFP(B, B_End);

	mpf_t expectation;
	mpf_init(expectation);
	mpf_set_d(expectation, 0.0);
	mpf_t prevTerm;
	mpf_init(prevTerm);
	mpf_set_d(prevTerm, 0.0);

	mpf_t term;
	mpf_init(term);

	mpf_t Nui;
	mpf_init(Nui);
	ULLToFP(Nui, N);

	mpf_t oneConst;
	mpf_init(oneConst);
	mpf_set_d(oneConst, 1.0);

	mpf_t startChairs;
	mpf_init(startChairs);
	mpf_t endChairs;
	mpf_init(endChairs);

	mpf_t A_Start_MPIR;
	mpf_init(A_Start_MPIR);
	ULLToFP(A_Start_MPIR, A_Start);
	mpf_t A_End_MPIR;
	mpf_init(A_End_MPIR);
	ULLToFP(A_End_MPIR, A_End);

	mpf_t B_Start_MPIR;
	mpf_init(B_Start_MPIR);
	ULLToFP(B_Start_MPIR, B_Start);
	mpf_t B_End_MPIR;
	mpf_init(B_End_MPIR);
	ULLToFP(B_End_MPIR, B_End);

	mpf_add(startChairs, A_Start_MPIR, B_Start_MPIR);
	mpf_add(endChairs, A_End_MPIR, B_End_MPIR);

	mpf_t twoConst;
	mpf_init_set_d(twoConst, 2.0);

	int loopCount = 0;
	while (true)
	{
		++loopCount;

		mpf_t emptyChairFraction;
		mpf_init(emptyChairFraction);
		mpf_t chairCount;
		mpf_init(chairCount);
		mpf_t tempi;
		mpf_init(tempi);

		mpf_add(chairCount, A, B);

		
		mpf_mul(tempi, B, twoConst);
		mpf_add(tempi, tempi, A);
		//cout << "XXX=" << (N - A - B) << " " << static_cast<double>(N - A - B) << endl;
		//cout << "A=";		mpf_out_str(NULL, 10, 30, A);		cout << " B=";		mpf_out_str(NULL, 10, 30, B);
		//cout << endl;
		//cout << " chairCount=";		mpf_out_str(NULL, 10, 30, chairCount);		cout << endl;

		//long double emptyChairFraction = (static_cast<long double>(N) - static_cast<long double>(chairCount)) / 
		//	static_cast<long double>(N);
		mpf_div(emptyChairFraction, tempi, Nui);
		cout << "ECF=";		mpf_out_str(NULL, 10, 30, emptyChairFraction);		cout << endl;

		mpf_t percentOfCases;
		mpf_init(percentOfCases);
		getPoC_MPIR(percentOfCases, A_Start + B_Start, A_End + B_End, chairCount);
		//long double percentOfCases = getPoC(A_Start + B_Start, A_End + B_End, chairCount);
		cout << "PoC=";		mpf_out_str(NULL, 10, 30, percentOfCases);		cout << endl;
		//long double term = emptyChairFraction * percentOfCases;
		mpf_mul(term, emptyChairFraction, percentOfCases);

		//cout << "term=";		mpf_out_str(NULL, 10, 30, term);		cout << endl;

		//expectation += term;
		mpf_add(expectation, expectation, term);
		
		cout << "expectation=";		mpf_out_str(NULL, 10, 30, expectation);		cout << endl;
		//cout << "tempi=";		mpf_out_str(NULL, 10, 30, tempi);		cout << " Nui=";		mpf_out_str(NULL, 10, 30, Nui);		cout << endl;

		//cout << /*"A=" << A << " B=" << B << setprecision(20) << */ " ECF=";		mpf_out_str(NULL, 10, 20, emptyChairFraction);		cout << endl;

		mpf_clear(tempi);
		mpf_clear(emptyChairFraction);
		mpf_clear(chairCount);

	

		mpf_clear(percentOfCases);

		//if (B == 2) break;
		if (loopCount > 5)
		{
			//cout << "DEBUG" << endl;
			//break;
		}
		
		//if (term < EPSILON) break;
		mpf_t relDiff;
		mpf_init(relDiff);
		mpf_sub(relDiff, term, epsilon);
		mpf_abs(relDiff, relDiff);
		if (mpf_cmp(relDiff, epsilon) < 0)
		{
			cout << "EARLY FINISH - E(" << N << ")=";
			mpf_out_str(NULL, 10, 20, expectation);
			cout << endl;
			break;
		}
		mpf_clear(relDiff);

		//cout << "Answer so far=";		mpf_out_str(NULL, 10, 20, expectation);		cout << endl;

		// Increase A by 3 and decrease B by 2 until B goes below 0
		// or desired result precision is reached
		// NB Take care as I'm using unsigned long longs
		//if (B == B_Start) break;
		if (mpf_cmp_d(B, B_Start) == 0)
		{
			cout << "FINISHED - E(" << N << ")=";
			mpf_out_str(NULL, 10, 20, expectation);
			cout << endl;
			break;
		}
		//B += 2;
		//A -= 3;
		mpf_add(B, B, oneConst);
		mpf_add(B, B, oneConst);
		mpf_sub(A, A, oneConst);
		mpf_sub(A, A, oneConst);
		mpf_sub(A, A, oneConst);
	}

	mpf_clear(epsilon);
	// And probably should clear the others too but not too bothered for
	// this crummy throw-away code.

	return 0.0;

	/*
	After a load of hassle writing this to use MPIR, I don't think it works, anyway!
	The first iteration gives 0.499, but then the next zillion or so have such a tiny
	contribution that the expectation never shifts from 0.5
	However perhaps you do need to include all of those? Actual answer (from cheat site!) 
	is 0.56766764161831 - is this correct? My code looks like it is heading towards 0.5 as N gets higher.
	My MonteCarlo code suggests that that is correct - certainly for lower values (eg 1000) the answer
	is roughly that.
	*/
}

long double test04(ULL N)
{
	ULL A_Start = (3 - (N % 3)) % 3;
	ULL B_Start = (N - 2 * A_Start) / 3;

	ULL A_End = A_Start + (B_Start / 2) * 3;
	ULL B_End = B_Start % 2;
	//cout << "N=" << N << " A_Start=" << A_Start << " B_Start=" << B_Start << endl;
	//cout << "N=" << N << " A_End=" << A_End << " B_End=" << B_End << endl;

	ULL A = A_End;
	ULL B = B_End;
	long double expectation = 0.0;
	long double prevTerm = 0.0;
	while (true)
	{
		ULL chairCount = A + B;
		long double emptyChairFraction = (static_cast<long double>(N)-static_cast<long double>(chairCount)) / static_cast<long double>(N);
		//long double percentOfCases = static_cast<long double>(factorial(chairCount - 1)) / static_cast<long double>(totalChairLayouts);
		long double percentOfCases = getPoC(A_Start + B_Start, A_End + B_End, chairCount);
		long double term = emptyChairFraction * percentOfCases;
		expectation += term;

		cout << "A=" << A << " B=" << B << setprecision(20) << " ECF=" << emptyChairFraction << " poc=" << percentOfCases << " exp=" << expectation << endl;

		if (term < EPSILON) break;
		// Increase A by 3 and decrease B by 2 until B goes below 0
		// or desired result precision is reached
		// NB Take care as I'm using unsigned long longs
		if (B == B_Start) break;
		B += 2;
		A -= 3;
	}

	return expectation;

}

void testMPR()
{
	mpf_t fp1;
	mpf_t fp2;
	mpf_t res;
	mpf_init(fp1);
	mpf_init(fp2);
	mpf_init(res);

	mpf_set_d(fp1, 3.3);
	mpf_set_d(fp2, 4.3);
	
	mpf_add(res, fp1, fp2);
	mpf_out_str(NULL, 10, 20, res);
	cout << endl;
	mpf_clear(fp1);
	mpf_clear(fp2);
}

/*
Monte Carlo test results:
MC(4)=0.5
MC(5)=0.6
MC(6)=0.555605
MC(7)=0.571429
MC(8)=0.566581
MC(9)=0.567939
MC(10)=0.567644
MC(11)=0.567691
MC(12)=0.567604
MC(13)=0.56766
MC(14)=0.567706
MC(15)=0.567622
MC(16)=0.567686
MC(17)=0.567641
MC(18)=0.567657
MC(19)=0.567664

Final result = 0.56766764161831
*/
bool isSpaceLeft(vector<int> &table)
{
	int seatRunCount = 0;
	for (int i = 0; i < table.size() + 5; ++i)
	{
		if (table[i % table.size()] == 0) ++seatRunCount;
		else seatRunCount = 0;
		if (seatRunCount >= 3) return true;
	}
	return false;
}

// Do a simulation for size N and return the fraction of empty chairs
// This will be repeated over and over for a Monte Carlo result
double SingleRunOfC(int N, map<int, int> &histogram)
{
	
	uniform_int_distribution<int> distribution(0, N-1);

	vector<int> table(N, 0);

	while (isSpaceLeft(table))
	{
		int seatGuess = distribution(generator);
		if (table[(seatGuess +table.size()-1) % table.size()] == 0 && table[(seatGuess + 1) % table.size()] == 0)
			table[seatGuess] = 1;

		//for (auto it = table.begin(); it != table.end(); ++it)
		//	cout << *it;
		//cout << endl;
	}

	int emptySeatCount = count(table.begin(), table.end(), 0);
	if (histogram.find(emptySeatCount) == histogram.end())
		histogram[emptySeatCount] = 0;
	else
		histogram[emptySeatCount]++;
	return emptySeatCount / static_cast<double>(N);
}

double MonteCarlo(int N, int RC = 1000000)
{
	map<int, int> histogram;

	double ECSum = 0.0;
	int runCount = 0;
	while (true)//runCount < RC)
	{
		double C = SingleRunOfC(N, histogram);
		ECSum += C;
		++runCount;
		if (runCount % 1000000 == 0)
			cout << "Exp=" << setprecision(20) << (ECSum / runCount) << endl;
	}

	for (auto it = histogram.begin(); it != histogram.end(); ++it)
	{
		cout << it->first << " " << (it->second / static_cast<double>(RC)) << endl;
	}

	return ECSum / static_cast<double>(runCount);
}

void traverse(vector<int> table, int depth)
{
	if (isSpaceLeft(table) == false)
	{
		for_each(table.begin(), table.end(), [&](int n){
			cout << n;
		});
		cout << " depth=" << depth << endl;
		return;
	}

	int filledCount = 0;
	for (auto i = 0; i < table.size(); ++i)
	{
		int seatGuess = i;

		if (table[seatGuess] == 1) ++filledCount;

		if (table[(seatGuess + table.size() - 1) % table.size()] == 0 &&
			table[(seatGuess + 1) % table.size()] == 0
			&& table[seatGuess] == 0 && filledCount >= depth)
		{
			table[seatGuess] = 1;
			traverse(table, depth+1);
			table[seatGuess] = 0;
		}
	}
	
}

void testTraverse()
{
	int N = 8;
	vector<int> table(N, 0);
	table[0] = 1;
	traverse(table, 0);
}

int main()
{
	
	//cout << "PoC=" << getPoC(7, 5, 6) << endl;

	//test03(1000000000000000000);
	//test03(10);
#if 0
	for (int i = 2; i < 12000; ++i)
	{

		long double t3 = test03(i);
		//long double t2 = test02(i);
		//cout << "N=" << i << setprecision(16) << " t2=" << t2 << " t3=" << t3 << endl;
	}
#endif
#if 0	
	ULL tv = 10;
	for (int i = 1; i <= 12; ++i)
	{
		//cout << " E(" << tv << ")=" << setprecision(16) << test03(tv) << endl;
		test03(tv);
		tv *= 10;
	}
#endif

	//testMPR();
	//test03(12);
	for (int i = 4; i < 100; ++i)
	{
		//cout << "MC(" << i << ")=" << MonteCarlo(i) << endl;
	}

	MonteCarlo(10, 10000000000);
	//testTraverse();

	return 0;
}