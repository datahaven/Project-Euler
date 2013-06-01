// Project Euler 122
// Adrian Dale 27/05/2013
//
// Congratulations, the answer you gave to problem 122 is correct.
//
// You are the 3527th person to have solved this problem.
//
// Return to Problems page.
//
// This version of the code produces the correct answer but takes more than
// a minute to do so. (132.8 seconds)
//
// It seems (from the answers forum) that searching for "Addition Chains"
// would have helped a lot here. I solved this without doing any internet
// searching at all, btw, so there are better solutions available than
// mine. My implementation is also quite slow since it relies on sets a 
// lot, when I suspect it could be re-written to not need them.
#include <iostream>
#include <set>
#include <vector>
#include <ctime>
#include <algorithm>

using namespace std;

const int MAX_M = 200;

vector< vector< set<int> > > memo;

// Research on paper suggests that the correct answer is to calculate all the
// possible trees that split m in half.
// The count of the number of distinct powers in the tree is the number of
// multiplications needed. Need to get that for all trees and take minimum.
//
// This appears not to work!
// It fails because there is more than one best answer for each number, so
// it depends which best answer you use when calculating higher up answers.
// This first goes wrong when n==15 uses the 1,2,4,8 version of n^12, when
// the 1,2,3,6 version gives the best answer.
// Problem doesn't decompose into sub-problems.
//
// Therefore the working version looks at all best results so far.
void fillNextMemo(int n)
{
	vector< set<int> > bestSetListSoFar;
	size_t bestSetSizeSoFar = 10000;
	
	for(int i=1; i<=n/2; ++i)
	{
		// Making these references, rather than copies took 7 seconds off
		// the total 139 seconds run time.
		vector< set<int> > &vec1 = memo[i];
		vector< set<int> > &vec2 = memo[n-i];

		for( auto iv1 = vec1.begin(); iv1 != vec1.end(); ++iv1)
		{
			for( auto iv2 = vec2.begin(); iv2 != vec2.end(); ++iv2)
			{
				set<int> s1 = *iv1;
				set<int> s2 = *iv2;
				// Merge the two sets
				s1.insert(s2.begin(), s2.end());
				// Inserting i and n-i represents the multiplication
				// of i and n-i
				s1.insert(i);
				s1.insert(n-i);
				if (s1.size() < bestSetSizeSoFar)
				{
					bestSetSizeSoFar = s1.size();
					
					bestSetListSoFar.clear();
					bestSetListSoFar.push_back(s1);
				}
				else if (s1.size() == bestSetSizeSoFar)
				{
					bestSetListSoFar.push_back(s1);
				}
			}
		}
	}
	memo[n] = bestSetListSoFar;
}

void run()
{
	set<int> start;
	vector< set<int> > firstMemo;

	firstMemo.push_back( start );

	memo.push_back(firstMemo);
	memo.push_back(firstMemo);

	start.insert(1);
	vector< set<int> > secondMemo;
	secondMemo.push_back(start);

	memo.push_back(secondMemo);

	memo.resize(MAX_M+1);

	for(int i=3; i<=MAX_M; ++i)
	{
		cout << "Calculating i=" << i << endl;
		fillNextMemo(i);
	}

	size_t sum=0;
	for(int i=1; i<=MAX_M; ++i)
	{
		cout << "i=" << i << " -> " << memo[i][0].size() << endl;
		sum += memo[i][0].size();
	}
	cout << "sum=" << sum << endl;
}

int main()
{
	clock_t t = clock();
	run();
	t = clock() - t;
	cout << ((double)t)/CLOCKS_PER_SEC << " seconds" << endl;
	return 0;
}
