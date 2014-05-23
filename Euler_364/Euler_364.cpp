// Project Euler 364 - Comfortable Distance
// Adrian Dale 18/05/2014
//
// This is the rough version. It gives the final answer in under
// two seconds. Code hasn't been cleaned up and all of the test code
// and unused experimental code is left intact so that the included
// E364_Notes.txt file makes more sense.
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <ctime>
using namespace std;

typedef signed long long ULL;

const ULL MODVAL = 100000007;
//const ULL MODVAL = 13;

vector<ULL> facmap;
vector<ULL> twopowmap;
map<ULL, ULL> invmap;

// First attempt - tried to dive straight in and come up with the divide and conquer
// solution. It didn't work because I'd missed a little nuance of the problem.
// You must fill seats with no adjacent seats filled before filling ones with only one adjacent seat,
// before filling ones with no adjacent seats available.
// I'm not convinced divide and conquer will work here as won't both sides depend on each other?
int solve(bool leftFilled, bool rightFilled, int N, int depth)
{
	//cout << (leftFilled ? "X" : "O") << " " << N << " " <<(rightFilled ? "X" : "O")  << " d=" << depth << endl;
	if (N == 0)return 0;
	if (N == 1)return 1;
	if (N == 2)
	{
		if (leftFilled == rightFilled)
			return 2;
		else
			return 1;
	}

	// Solve all possible splits and add the results together
	int splitTotal = 0;
	int startPos = (leftFilled ? 1 : 0);
	int endPos = (rightFilled ? N-2  : N-1);
	for (int i = startPos; i <= endPos; ++i)
	{
		int leftTotal = solve(leftFilled, true, i, depth+1) % MODVAL;
		int rightTotal = solve(true, rightFilled, N-i-1, depth+1) % MODVAL;
		
		int sumTotal = (leftTotal + rightTotal) % MODVAL;

		
		splitTotal += sumTotal;
		splitTotal %= MODVAL;
	}
	//cout << (leftFilled ? "X" : "O") << " " << N << (rightFilled ? "X" : "O") << " st=" << splitTotal << endl;
	//cout << "st=" << splitTotal << endl;
	return splitTotal;
}

int solCount = 0;
set<int> Solutions;

ULL fac(ULL N)
{
	if (N<0)
	{
		cout << "fac ERROR " << N << endl;
		return 1;
	}
	ULL res = 1;
	while (N > 1)
	{
		res *= N--;
	}
	return res;
}

inline ULL facmod(ULL N)
{
	if (N < facmap.size())
	{
		return facmap[N];
	}

	//map<ULL, ULL>::iterator it = facmap.find(N);
	//if (it != facmap.end()) return it->second;
	ULL count = N;
	ULL res = 1;
	while (N != 0)
	{
		res *= N;
		res %= MODVAL;
		N -= 1;
	}
	//facmap[count] = res;
	return res;
}

// From http://stackoverflow.com/questions/19739492/calculating-ncr-mod-p-efficiently-when-n-is-very-large
ULL modPow(ULL a, ULL x, ULL p) {
	//calculates a^x mod p in logarithmic time.
	ULL res = 1;
	while (x > 0) {
		if (x % 2 != 0) {
			res = (res * a) % p;
		}
		a = (a * a) % p;
		x /= 2;
	}
	return res;
}

ULL modInverse(ULL a, ULL p) {
	//calculates the modular multiplicative of a mod m.
	//(assuming p is prime).
	auto it = invmap.find(a);
	if (it != invmap.end()) return it->second;
	ULL res = modPow(a, p - 2, p);
	invmap[a] = res;
	return res;
}


inline ULL permCount(ULL A, ULL B)
{
	return fac(A+B) / (fac(A)*fac(B));
}

inline ULL permCountMod(ULL A, ULL B)
{
	ULL numerator = facmod(A + B);
	ULL denominator = (facmod(A))*(facmod(B))%MODVAL;

	return (numerator * modInverse(denominator, MODVAL)) % MODVAL;
}

// Ooops! Doesn't actually need x!
inline ULL twoPowMod(ULL x, ULL p)
{
	if (p < twopowmap.size()) return twopowmap[p];

	ULL res = 1;
	for (int i = 0; i < p; ++i)
	{
		res *= 2;
		res %= MODVAL;
	}
	//twopowmap[p] = res;
	return res;
}

// This works - it matches A192008 in oeis.org
// Modified linear "pay-phone sequence": Number of ways to consecutively occupy n pay-phones 
// in a row while minimizing the number of directly adjacent phones picked previously.
//
// Gets slow around N==13
/*
solve(1) = 1
solve(2) = 2
solve(3) = 4
solve(4) = 8
solve(5) = 32
solve(6) = 96
solve(7) = 456
solve(8) = 2016
solve(9) = 11232
solve(10) = 61632
solve(11) = 419328
solve(12) = 2695680
solve(13) = 21358080
*/
void solveII(vector<bool> bits)
{
	int setCount = 0;
	for (auto i = 0; i < bits.size(); ++i)
	{
		if (bits[i]) ++setCount;
	}
	if (setCount == bits.size())
	{
		++solCount;
		return;
	}

	// Recurse for all possible next seats
	// - look for 3-runs first
	bool threeRunFound = false;
	for (auto i = 0; i < bits.size(); ++i)
	{
		bool leftEmpty = (i == 0 || bits[i - 1] == false);
		bool rightEmpty = (i == (bits.size() - 1) || bits[i + 1] == false);
		if (leftEmpty && rightEmpty && bits[i] == false)
		{
			threeRunFound = true;
			vector<bool> newSeats = bits;
			newSeats[i] = true;
			solveII(newSeats);
		}
	}
	// - now look for 2-runs
	bool twoRunFound = false;
	if (threeRunFound == false)
	{
		for (auto i = 0; i < bits.size(); ++i)
		{
			bool leftEmpty = (i == 0 || bits[i - 1] == false);
			bool rightEmpty = (i == (bits.size() - 1) || bits[i + 1] == false);
			if ((leftEmpty || rightEmpty) && bits[i] == false)
			{
				twoRunFound = true;
				vector<bool> newSeats = bits;
				newSeats[i] = true;
				solveII(newSeats);
			}
		}
	}

	// Only want to fill remaining seats when seats with space
	// either side can't be filled
	if (threeRunFound == false && twoRunFound == false)
	{
		for (auto i = 0; i < bits.size(); ++i)
		{
			if (bits[i] == false)
			{
				vector<bool> newSeats = bits;
				newSeats[i] = true;
				solveII(newSeats);
			}
		}
	}
}

// Just count all possible threeRun layouts
void solveIII(vector<bool> bits)
{
	

	// Recurse for all possible next seats
	// - look for 3-runs first
	bool threeRunFound = false;
	for (auto i = 0; i < bits.size(); ++i)
	{
		bool leftEmpty = (i == 0 || bits[i - 1] == false);
		bool rightEmpty = (i == (bits.size() - 1) || bits[i + 1] == false);
		if (leftEmpty && rightEmpty && bits[i] == false)
		{
			threeRunFound = true;
			vector<bool> newSeats = bits;
			newSeats[i] = true;
			solveIII(newSeats);
		}
	}
	
	if (threeRunFound == false)
	{
		// Rough code to store the results
		// Not bothered which way is MSB/LSB, so don't treat
		// results too literally
		int intVal = 0;
		int shiftVal = 1;
		for_each(bits.begin(), bits.end(), [&](bool bit){
			//cout << bit;
			if (bit)
			{
				intVal += shiftVal;
			}
			shiftVal <<= 1;
		});
		//cout << " = " << intVal << endl;
		Solutions.insert(intVal);
		++solCount;
	}
}


void test01()
{
	for (int i = 1; i <= 10; ++i)
	{

		cout << "solve(" << i << ") = " << solve(false, false, i, 0) << endl;
	}
}

void test02()
{
	for (int i = 1; i <= 30; ++i)
	{
		solCount = 0;
		vector<bool> seats(i);
		solveII(seats);
		cout << "solve(" << i << ") = " << solCount << endl;
	}
}

void test03()
{
	for (int i = 5; i <= 5; ++i)
	{
		solCount = 0;
		Solutions.clear();
		vector<bool> seats(i);
		solveIII(seats);
		cout << "solve(" << i << ") = " << solCount << " unique=" << Solutions.size() <<  endl;
		for_each(Solutions.begin(), Solutions.end(), [&](int sol){
			int shiftVal = 1;
			for (int j = 0; j < i; ++j)
			{
				cout << (((sol & shiftVal) == 0) ? "0" : "1");
				shiftVal <<= 1;
			}
			cout << endl;
		});
	}
/*
solve(3) = 3 unique=2
solve(4) = 6 unique=3
solve(5) = 12 unique=4
solve(6) = 26 unique=5
solve(7) = 60 unique=7
solve(8) = 144 unique=9
solve(9) = 366 unique=12
solve(10) = 960 unique=16
solve(11) = 2640 unique=21
solve(12) = 7464 unique=28
solve(13) = 21960 unique=37
solve(14) = 66240 unique=49
solve(15) = 206760 unique=65
solve(16) = 660240 unique=86
solve(17) = 2172240 unique=114
solve(18) = 7298640 unique=151
solve(19) = 25179840 unique=200

unique solution counts follow the Padovan sequence A000931
Also, as suspected, it's the number of compositions (ordered partitions)
into parts 2 and 3
A228361 states it more explicitly:
The number of all possible covers of L-length line segment by
2-length line segments with allowed gaps < 2
*/
}

void test04(int N)
{
	for (int a = 0; a < N; ++a)
	{
		for (int b = 0; b < N; ++b)
		{
			if (2 * a + 3 * b == N)
			{
				cout << "N=" << N << " " << a << " " << b << endl;
			}
		}
	}
	int A_Start = (3 - (N % 3)) % 3;
	int B_Start = (N - 2 * A_Start) / 3;
	cout << "A_Start=" << A_Start << " B_Start=" << B_Start << endl;

	/*
	N = 100 2 32
		N = 100 5 30
		N = 100 8 28
		N = 100 11 26
		N = 100 14 24
		N = 100 17 22
		N = 100 20 20
		N = 100 23 18
		N = 100 26 16
		N = 100 29 14
		N = 100 32 12
		N = 100 35 10
		N = 100 38 8
		N = 100 41 6
		N = 100 44 4
		N = 100 47 2
		N = 100 50 0
		*/
}

int test05(int N, bool leftFilled)
{
	int A_Start = (3 - (N % 3)) % 3;
	int B_Start = (N - 2 * A_Start) / 3;
	int sum = 0;
	do
	{
		// Change this so rule1 is sum of variants1 and variants2
		// where v1 is all perms without B on RHS, v2 is all perms with B on RHS.
		int variants1 = 0;
		int variants2 = 0; // B on RHS
		if (B_Start > 0)
		{
			variants2 = fac(A_Start + B_Start - 1) / (fac(A_Start)*fac(B_Start - 1));
		}
		if (A_Start > 0)
		{
			variants1 = fac(A_Start + B_Start - 1) / (fac(A_Start-1)*fac(B_Start));
		}

		// Sanity check - it's worked so far!
		int variants = fac(A_Start + B_Start) / (fac(A_Start)*fac(B_Start));
		if (variants != (variants1 + variants2))
		{
			cout << "ERROR" << endl;
			break;
		}

		// rule1 is how many different ways can we get to this combination of A and B's
		// for all orderings of A and B.
		// This is now split into rule1_1 and rule1_2 which is as before but the _2 is how
		// many of the orderings have B on RHS and _1 is how many don't.
		int rule1_1 = variants1 * fac(A_Start + B_Start);
		int rule1_2 = variants2 * fac(A_Start + B_Start);
		
		// rule2B is how many B's need filling
		int rule2B = B_Start + (leftFilled ? 0 : 1);
		int rule2_1 = (1 << (B_Start)) * fac(rule2B);
		int rule2_2 = (1 << (B_Start-1)) * fac(rule2B);

		int seatsLeft_1 = fac(B_Start+A_Start-1);// fac(N - A_Start - B_Start);
		int seatsLeft_2 = fac(B_Start+A_Start-1);// fac(N - A_Start - B_Start);

		int rule3_1 = rule1_1*rule2_1 *seatsLeft_1;
		int rule3_2 = rule1_2*rule2_2 *seatsLeft_2;

		int all = rule3_1+rule3_2;
		//cout << "A_Start=" << A_Start << " B_Start=" << B_Start
		//	<< " v1=" << variants1 << " v2=" << variants2 << " all=" << all << endl;

		sum += all;

		A_Start += 3;
		B_Start -= 2;
	} while (B_Start >= 0);
	//cout << "sum=" << sum << endl;
	return sum;
}



ULL test07(ULL N, bool leftFilled)
{
	ULL A_Start = (3 - (N % 3)) % 3;
	ULL B_Start = (N - 2 * A_Start) / 3;
	ULL sum = 0;
	do
	{
		

		// Change this so rule1 is sum of variants1 and variants2
		// where v1 is all perms without B on RHS, v2 is all perms with B on RHS.
		ULL variants1 = 0;
		ULL variants2 = 0; // B on RHS
		if (B_Start != 0)
		{
			//variants2 = (facmod(A_Start + B_Start - 1)%MODVAL) / 
			//	(((facmod(A_Start)%MODVAL)*(facmod(B_Start - 1)%MODVAL))%MODVAL);
			variants2 = permCountMod(A_Start, B_Start - 1);
			//int pcTest = permCount(A_Start, B_Start - 1);
			//if (pcTest%MODVAL != variants2)
			//{
				//cout << "PC ERROR " << A_Start << " " << B_Start - 1 << endl;
			//}
		}
		if (A_Start != 0)
		{
			//variants1 = facmod(A_Start + B_Start - 1) / (facmod(A_Start - 1)*facmod(B_Start));
			//variants1 = (facmod(A_Start + B_Start - 1) % MODVAL) /
			//	(((facmod(A_Start-1) % MODVAL)*(facmod(B_Start) % MODVAL)) % MODVAL);
			variants1 = permCountMod(A_Start - 1, B_Start);
		}

		 
		
		// rule1 is how many different ways can we get to this combination of A and B's
		// for all orderings of A and B.
		// This is now split into rule1_1 and rule1_2 which is as before but the _2 is how
		// many of the orderings have B on RHS and _1 is how many don't.
		ULL rule1_1 = variants1 * facmod(A_Start + B_Start);
		ULL rule1_2 = variants2 * facmod(A_Start + B_Start);
		if (rule1_1 < 0 || rule1_2 < 0)
		{
			cout << "OVERFLOW r1" << endl;
		}
		rule1_1 %= MODVAL;
		rule1_2 %= MODVAL;

		// rule2B is how many B's need filling
		ULL rule2B = B_Start + (leftFilled ? 0 : 1);
		//ULL rule2_1 = (1 << (B_Start)) * facmod(rule2B);
		//ULL rule2_2 = (1 << (B_Start - 1)) * facmod(rule2B);
		ULL rule2_1 = twoPowMod(2, B_Start) * facmod(rule2B);
		ULL rule2_2 = twoPowMod(2, B_Start - 1) * facmod(rule2B);
		if (rule2_1 < 0 || rule2_2 < 0)
		{
			cout << "OVERFLOW r2" << endl;
		}
		rule2_1 %= MODVAL;
		rule2_2 %= MODVAL;

		ULL seatsLeft_1 = facmod(B_Start + A_Start - 1);// fac(N - A_Start - B_Start);
		ULL seatsLeft_2 = facmod(B_Start + A_Start - 1);// fac(N - A_Start - B_Start);

		seatsLeft_1 %= MODVAL;
		seatsLeft_2 %= MODVAL;

		ULL rule3_1 = ((rule1_1*rule2_1)%MODVAL) *seatsLeft_1;
		ULL rule3_2 = ((rule1_2*rule2_2)%MODVAL) *seatsLeft_2;
		if (rule3_1 < 0 || rule3_2 < 0)
		{
			cout << "OVERFLOW r3" << endl;
		}
		rule3_1 %= MODVAL;
		rule3_2 %= MODVAL;

		ULL all = rule3_1 + rule3_2;
		if (all < 0)
		{
			cout << "OVERFLOW all" << endl;
		}
		all %= MODVAL;

		//cout << "A_Start=" << A_Start << " B_Start=" << B_Start
		//	<< " v1=" << variants1 << " v2=" << variants2 << " all=" << all << endl;

		sum += all;
		if (sum < 0)
		{
			cout << "OVERFLOW sum" << endl;
		}

		sum %= MODVAL;

		A_Start += 3;
		B_Start -= 2;
	} while (B_Start >= 0);
	
	//cout << "sum=" << sum << endl;
	return sum;
}

void test06()
{
	for (ULL i = 4; i <= 20; ++i)
	{
		ULL res1 = test07(i, false);  // Works for 05 or 07
		ULL res2 = test07(i + 1, true);
		ULL res = (res1 + res2) % MODVAL;

		res1 = test05(i, false);  // Works for 05 or 07
		res2 = test05(i + 1, true);
		ULL resB = (res1 + res2) % MODVAL;

		cout << "i=" << i << " resultA=" << res << " resultB=" << resB << endl;
	}

	ULL res1 = test07(1000, false);  // Works for 05 or 07
	ULL res2 = test07(1000 + 1, true);
	ULL res = (res1 + res2) % MODVAL;
	cout << "i=" << 1000 << " result=" << res << endl;

	time_t rawtime;
	time(&rawtime);
	cout << "Time=" << ctime(&rawtime) << endl;

	res1 = test07(10000, false);  // Works for 05 or 07
	res2 = test07(10000 + 1, true);
	res = (res1 + res2) % MODVAL;
	cout << "i=" << 10000 << " result=" << res << endl;

	time(&rawtime);
	cout << "Time=" << ctime(&rawtime) << endl;

	res1 = test07(100000, false);  // Works for 05 or 07
	res2 = test07(100000 + 1, true);
	res = (res1 + res2) % MODVAL;
	cout << "i=" << 100000 << " result=" << res << endl;

	time(&rawtime);
	cout << "Time=" << ctime(&rawtime) << endl;

	res1 = test07(1000000, false);  // Works for 05 or 07
	res2 = test07(1000000 + 1, true);
	res = (res1 + res2) % MODVAL;
	cout << "i=" << 1000000 << " result=" << res << endl;

	time(&rawtime);
	cout << "Time=" << ctime(&rawtime) << endl;

}


void testPC()
{
	for (int i = 1000; i <= 1000; ++i)
	{
		for (int j = 380; j < 390; ++j)
		{
			//ULL pc = permCount(i, j)%MODVAL;
			ULL pcm = permCountMod(i, j)%MODVAL;

			
				cout << "pc(" << i << ", " << j<< ")=" << pcm << endl;
			
		}
	}
}

void setupFacMap()
{
	ULL res = 1;
	facmap.clear();
	facmap.push_back(1); // 0! == 1
	for (int i = 1; i < 2500000; ++i)
	{
		res *= i;
		res %= MODVAL;
		facmap.push_back(res);
	}
}

void setupTwoPowMap()
{
	ULL res = 1;
	for (int i = 0; i < 2500000; ++i)
	{
		twopowmap.push_back(res);
		res *= 2;
		res %= MODVAL;
	}
}

void init()
{

	setupFacMap();
	setupTwoPowMap();
}

int main()
{
	init();

	//modint_test_01();
	test06();
	//testPC();
	
	return 0;
}