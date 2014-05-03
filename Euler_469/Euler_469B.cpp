// Adrian Dale
// Project Euler 469 test code

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
using namespace std;

typedef unsigned long long ULL;

int SolCount = 0;
map<int, int> Histogram;
map<ULL, long double> Memo;
long double Result = 0.0;

inline int getbit(ULL x, int bit)
{
	return (x >> bit) & 1;
}

inline void setbit(ULL &x, int bit)
{
	x |= (1 << bit);
}

inline void clearbit(ULL &x, int bit)
{
	x &= ~(1 << bit);
}

inline ULL reverse_bits(ULL x, int N)
{
	ULL reversed = 0;
	for (int i = 0; i < N; ++i)
	{
		if (getbit(x, i) == 1) setbit(reversed, N - i - 1 );
	}
	return reversed;
}

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

bool isSpaceLeft_II(ULL table, int N)
{
	int seatRunCount = 0;
	for (int i = 0; i < N + 5; ++i)
	{
		if (getbit(table, i % N)== 0) ++seatRunCount;
		else seatRunCount = 0;
		if (seatRunCount >= 3) return true;
	}
	return false;
}

void traverse(vector<int> table, int depth, long double parentProbability)
{
	if (isSpaceLeft(table) == false)
	{
		
		for_each(table.begin(), table.end(), [&](int n){
			cout << n;
		});
		cout << " depth=" << depth << " prob=" << setprecision(16) << parentProbability << endl;
		
		++SolCount;

		
		long double c = (table.size() - depth - 1) / static_cast<long double>(table.size());
		Result += parentProbability * c;
		/*
		if (Histogram.find(depth) == Histogram.end())
			Histogram[depth] = 1;
		else
			Histogram[depth]++;
		*/
		return;
	}

	ULL noOfChildren = 0;
	for (auto i = 0; i < table.size(); ++i)
	{
		int seatGuess = i;

		if (table[(seatGuess + table.size() - 1) % table.size()] == 0 &&
			table[(seatGuess + 1) % table.size()] == 0
			&& table[seatGuess] == 0 /*&& filledCount >= depth*/)
		{
			++noOfChildren;
		}
	}
	long double newParentProbability = parentProbability * 1.0 / static_cast<long double>(noOfChildren);

	int filledCount = 0;
	for (auto i = 0; i < table.size(); ++i)
	{
		int seatGuess = i;

		if (table[seatGuess] == 1) ++filledCount;

		if (table[(seatGuess + table.size() - 1) % table.size()] == 0 &&
			table[(seatGuess + 1) % table.size()] == 0
			&& table[seatGuess] == 0 /*&& filledCount >= depth*/)
		{
			table[seatGuess] = 1;
			traverse(table, depth + 1, newParentProbability);
			table[seatGuess] = 0;
		}
	}

}

void traverseII(ULL table, int depth, long double parentProbability, int N)
{
	if (isSpaceLeft_II(table, N) == false)
	{
		//for_each(table.begin(), table.end(), [&](int n){
		//	cout << n;
		//});
		//cout << " depth=" << depth << " prob=" << setprecision(16) << parentProbability << endl;

		++SolCount;


		long double c = (N - depth - 1) / static_cast<long double>(N);
		Result += parentProbability * c;
		/*
		if (Histogram.find(depth) == Histogram.end())
		Histogram[depth] = 1;
		else
		Histogram[depth]++;
		*/
		return;
	}

	ULL noOfChildren = 0;
	for (auto i = 0; i < N; ++i)
	{
		int seatGuess = i;

		if (getbit(table, (seatGuess + N - 1) % N) == 0 &&
			getbit(table, (seatGuess + 1) % N) == 0
			&& getbit(table, seatGuess) == 0 )
		{
			++noOfChildren;
		}
	}
	long double newParentProbability = parentProbability * 1.0 / static_cast<long double>(noOfChildren);

	int filledCount = 0;
	for (auto i = 0; i < N; ++i)
	{
		int seatGuess = i;

		if (getbit(table, (seatGuess + N - 1) % N) == 0 &&
			getbit(table, (seatGuess + 1) % N) == 0
			&& getbit(table, seatGuess) == 0)
		{
			setbit(table, seatGuess);
			traverseII(table, depth + 1, newParentProbability, N);
			clearbit(table, seatGuess);
		}
	}

}

#if 0
long double traverseIII(ULL table, int depth, ULL parentProbability, int N)
{
	// Aside from the fact this gives the wrong answers it only gets up to around N==30 anyway before
	// it crashes, presumably out of memory.
	// I suspect that we can see the same nodes but with different parent probabilities, if we used
	// a different route down the tree to reach them.
	// Could I memo on table+parentProbability? I think I could and it ought to work, but I don't
	// know if it'll get high enough for answer, anyway. Plus, how to implement?
	// Might not get many hits on our memo?
	// This is not much better than the non-memo version - and may not be correct anyway. (Possibly overflow related?)
	// It can get to about N==26 or 28, although answers go weird from around 24
	//
	// Idea - use symmetry? For each set of tables generated we only need to calculate for half of them.
	if (Memo.find(table) != Memo.end())
	{
		map<ULL, long double> &pmap = Memo[table];
		if (pmap.find(parentProbability) != pmap.end())
		{

			return pmap[parentProbability];
		}
	}

	if (isSpaceLeft_II(table, N) == false)
	{
		//for_each(table.begin(), table.end(), [&](int n){
		//	cout << n;
		//});
		//cout << " depth=" << depth << " prob=" << setprecision(16) << parentProbability << endl;

		++SolCount;

		long double c = (N - depth - 1) / static_cast<long double>(N);
		
		//Memo[table] = parentProbability * c;
		if (Memo.find(table) == Memo.end())
		{
			map<ULL, long double> pmap;
			pmap[parentProbability] = (1.0 / static_cast<long double>(parentProbability)) * c;
			Memo[table] = pmap;
		}
		else
		{
			auto &pmap = Memo[table];
			pmap[parentProbability] = (1.0 / static_cast<long double>(parentProbability)) *c;
		}
		return (1.0 / static_cast<long double>(parentProbability)) * c;
	}

	ULL noOfChildren = 0;
	for (auto i = 0; i < N; ++i)
	{
		int seatGuess = i;

		if (getbit(table, (seatGuess + N - 1) % N) == 0 &&
			getbit(table, (seatGuess + 1) % N) == 0
			&& getbit(table, seatGuess) == 0)
		{
			++noOfChildren;
		}
	}
	//long double newParentProbability = parentProbability * 1.0 / static_cast<long double>(noOfChildren);
	int newParentProbability = parentProbability * noOfChildren;

	long double cvalue = 0.0;
	for (auto i = 0; i < N; ++i)
	{
		int seatGuess = i;

		if (getbit(table, (seatGuess + N - 1) % N) == 0 &&
			getbit(table, (seatGuess + 1) % N) == 0
			&& getbit(table, seatGuess) == 0)
		{
			setbit(table, seatGuess);
			cvalue += traverseIII(table, depth + 1, newParentProbability, N);
			clearbit(table, seatGuess);
		}
	}
	
	//Memo[table] = cvalue;
	if (Memo.find(table) == Memo.end())
	{
		map<ULL, long double> pmap;
		pmap[parentProbability] = cvalue;
		Memo[table] = pmap;
	}
	else
	{
		auto &pmap = Memo[table];
		pmap[parentProbability] = cvalue;
	}
	return cvalue;
}
#endif

// This will be the bitfield version again, no memoing, but halving the 
// search space wherever possible.
// I think parentProbability needs to be a long double as I get errors when it is a ULL,
// which I suspect are overflow related
long double traverseIV(ULL table, int depth, long double parentProbability, int N)
{
	// Idea - use symmetry? For each set of tables generated we only need to calculate for half of them.
	// This doesn't get N much higher (26 or 27) and so far is giving slightly incorrect
	// results anyway. Would maybe memo up a bit more?
	if (isSpaceLeft_II(table, N) == false)
	{
		++SolCount;

		long double c = (N - depth - 1) / static_cast<long double>(N);

		return (1.0 / static_cast<long double>(parentProbability)) * c;
	}

	// Generate a vector of all of the possible children
	vector<ULL> children;
	for (auto i = 0; i < N; ++i)
	{
		int seatGuess = i;

		if (getbit(table, (seatGuess + N - 1) % N) == 0 &&
			getbit(table, (seatGuess + 1) % N) == 0
			&& getbit(table, seatGuess) == 0)
		{
			ULL child = table;
			setbit(child, seatGuess);
			children.push_back(child);
		}
	}

	
	long double newParentProbability = parentProbability * static_cast<long double>(children.size());

	long double cvalue = 0.0;
//#if 0
	for (size_t i = 0; i < ((children.size()+1) / 2); ++i)
	{
		ULL child = children[i];
		long double term = traverseIV(child, depth + 1, newParentProbability, N);
		if (i == children.size() / 2)
		{
			cvalue += term;
			//cout << setprecision(20) << term << " * 1.0" << " depth=" << depth << endl;
		}
		else
		{
			cvalue += term;
			//cout << setprecision(20) << term << " * 2.0" << " depth=" << depth << endl;
		}
	}
//#endif
	// Why do these two give similar but different results?
	// Precision problems again? Or is the above one skipping possibilities?
	// Answer: Children aren't always symmetrical - there are often duplicates, but
	// not always.
	// Solution: Memo on each row?
#if 0	
	for (size_t i = 0; i < children.size(); ++i)
	{
		ULL child = children[i];
		long double term = traverseIV(child, depth + 1, newParentProbability, N);
		cvalue += term;
		cout << setprecision(20) << term << endl;
	}
#endif
	
	return cvalue;
}

void breakIntoRuns(vector<int> &runs, ULL child, int N)
{
	int runCount = 0;
	for (auto i = 0; i < N; ++i)
	{
		int seatGuess = i;

		if (getbit(child, seatGuess) == 1)
		{
			if (runCount != 0)
				runs.push_back(runCount);
			runCount = 0;
		}
		else
			++runCount;	
	}
	if (runCount != 0)
		runs.push_back(runCount);
}

// This version uses a memo for E(n).
// It returns expected number of free chairs, NOT fraction of free chairs
// Could speed this up with symmetry trick? It's looking like it is nearly fast enough
long double traverseV(int N)
{
	auto memoValue = Memo.find(N);
	if (memoValue != Memo.end())
		return memoValue->second;

	long double result = 0.0;
	int A = 1;
	int B = N - 3;
	int childCount = 0;
	
	while (A <= N - 3)
	{
		result += (traverseV(A+1) + traverseV(B+1));
		++A; --B;
		++childCount;
	}

	result = result / static_cast<long double>(childCount);
	Memo[N] = result;
	return result;
}


// Version II is bitfield optimised version of traverse()
// As expected, it doesn't really speed things up much.
// The problem is the exponential growth in the size of the tree.
// This version just saves a bit of stack space when running.
// Neat practice to write it so quickly, though.
void testTraverseII()
{
	// 
	long double prevResult = 0.0;
	Memo.clear();
	Memo[0] = 0;
	Memo[1] = 0;
	Memo[2] = 1;
	Memo[3] = 2;
	Memo[4] = 2;
	for (int n = 5; n <= 10000000; ++n)
	{

		ULL table = 1;
		
		SolCount = 0;
		Result = 0.0;
		
		//long double result = traverseV(table, 0, 1.0, n);
		long double result = traverseV(n) / static_cast<long double>(n);
		if (fabs(result - prevResult) < 0.000000000000001)
		//if (fabs(result - prevResult) < 0.000000001)
		{
			cout << "N=" << n << setprecision(14) << " Result=" << (result) << endl;
			cout << "FINISHED" << endl;
			break;
		}
		prevResult = result;
		//if (n % 1000 == 0)
		//	cout << "N=" << n << setprecision(20) << " Result=" << (result) << endl;
	}
}


void testTraverse()
{
	// This code appears to work, although it is too slow beyond n == 20
	// Well, I get 0.56 for n==10 but forum claims that is incorrect.
	// MC(10)=0.567644
	// Not all outcomes are equally likely? Correct! Need to include probability
	// of getting to each node in tree, which depends on parent
	// That's now in the code.
	// Final result (from cheat site) = 0.56766764161831
	//
	// I think I might not need to calculate for n=10^18 as around n=20 we seem
	// to converge on the correct answer. Unfortunately my code slows to a crawl
	// around then. I don't think optimising it will help, although if I could
	// memo-ize the search it should get n up a lot higher.
	/*
N=4 SolCount=1
Result=0.5
N=5 SolCount=2
Result=0.59999999999999998
N=6 SolCount=3
Result=0.55555555555555547
N=7 SolCount=6
Result=0.5714285714285714
N=8 SolCount=12
Result=0.56666666666666665
N=9 SolCount=26
Result=0.56790123456790131
N=10 SolCount=60
Result=0.56761904761904758
N=11 SolCount=144
Result=0.56767676767676734
N=12 SolCount=366
Result=0.56766607877718989
N=13 SolCount=960
Result=0.567667887667888
N=14 SolCount=2640
Result=0.56766760576284592
N=15 SolCount=7464
Result=0.56766764648246126
N=16 SolCount=21960
Result=0.56766764100101319
N=17 SolCount=66240
Result=0.56766764169194162
N=18 SolCount=206760
Result=0.56766764161023653
N=19 SolCount=660240
Result=0.56766764161766836
N=20 SolCount=2172240
Result=0.56766764161849392
N=21 SolCount=7298640
Result=0.56766764162565031
N=22 SolCount=25179840
Result=0.5676676416208466
N=23 SolCount=88583040
Result=0.56766764162326067
N=24 SolCount=319097520
Result=0.56766764157746175
	*/
	for (int n = 6; n <= 9; ++n)
	{

		vector<int> table(n, 0);
		table[0] = 1;
		SolCount = 0;
		Result = 0.0;
		Histogram.clear();
		traverse(table, 0, 1.0);
		cout << "N=" << n << " SolCount=" << SolCount << endl;
		/*
		long double EC = 0.0;
		for (auto it = Histogram.begin(); it != Histogram.end(); ++it)
		{
			long double poc = it->second / static_cast<long double>(SolCount);
			long double c = (n - it->first - 1) / static_cast<long double>(n);
			EC += poc * c;
			cout << it->first << " " << it->second << endl;
		}
		*/
		cout << setprecision(20) << "Result=" << Result << endl;
	}
}

void testReverseBits()
{
	for (ULL i = 0; i < 10; ++i)
	{

		cout << "i=" << i << " rev=" << reverse_bits(i, 8) << endl;
	}
}

void test01()
{
	int N = 9;
	for (size_t i = 0; i < ((N + 1) / 2); ++i)
	{
		int mult = 0;
		if (i == N / 2)
			mult = 1;
		else
			mult = 2;
		cout << "i=" << i << " m=" << mult << endl;
	}
}

void test02()
{
	vector<int> runs;
	ULL t1 = 73;
	int N = 8;
	breakIntoRuns(runs, t1, 8);
	copy(runs.begin(), runs.end(), ostream_iterator<int>(cout, " "));
}

int main()
{
	testTraverseII();

	//testReverseBits();
	//test01();
	//test02();
	return 0;
}
