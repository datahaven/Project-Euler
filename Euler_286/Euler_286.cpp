// Project Euler 286
// Adrian Dale
// 18/02/2018
//
// Scoring Probabilities
#include <iostream>
using namespace std;

// Uses the DP algorithm from here:
// https://math.stackexchange.com/questions/429698/probability-of-getting-k-heads-with-n-coins
// It's basically the same problem
// Followed by a manual search by recompiling the code with a few different search
// parameters to narrow down the correct value of q

long double dp[51][51];

long double GetProbability(int n, int k, long double q)
{
	if (dp[n][k] != -1.0L)
	{
		return dp[n][k];
	}
	long double Pn = 1.0L - n / q;
	long double result = GetProbability(n-1, k, q) * (1.0L - Pn) + GetProbability(n-1, k-1, q) * Pn;
	dp[n][k] = result;
	return result;
}

long double GetResultForQ(long double q)
{
	// Reset our memo
	for (int i = 0; i < 51; ++i)
	{
		for (int j = 0; j < 51; ++j)
		{
			dp[i][j] = -1.0L;
		}
	}
	// Set up the memo with our known base cases
	// zero coins, P(zero heads) == 1
	dp[0][0] = 1L;
	for (int i = 1; i < 51; i++)
	{
		// zero coins, non-zero heads == 0
		dp[0][i] = 0L;
	}
	// P(zero heads from n coins)
	for (int i = 1; i < 51; i++)
	{
		long double prob = 1.0L;
		for (int k = 1; k <= i; ++k)
		{
			prob *= (k / q);
		}
		dp[i][0] = prob;
	}

	return GetProbability(50, 20, q);
}

int main()
{ 
	for (int i = 1; i < 1000000; ++i)
	{
		cout.precision(15);
		// Fiddle with this code a bit, trying different Base/Step values
		// until qBase isn't affected by qStep at the precision we're looking for
		long double qBase = 50.0L;
		long double qStep = 0.000000000001L;
		long double q = qBase + i*qStep;
		long double result = GetResultForQ(q);
		cout << "q=" <<  q << " => " << GetResultForQ(q) << endl;
		if (result < 0.02L)
			break;
	}
	return 0;
}