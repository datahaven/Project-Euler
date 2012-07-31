// Project Euler 72
// Adrian Dale 31/07/2012
//
// 303963552391
//
// Congratulations, the answer you gave to problem 72 is correct.
//
// You are the 8122nd person to have solved this problem.

#include <iostream>
using namespace std;

// NB not actually a u32. Code fails if I use actual u32 type
typedef unsigned long long u32;

const u32 PHISIZE = 1000000;
//const u32 PHISIZE = 8;
u32 gPhi[PHISIZE];
// Code by CFB from another Euler solution thread
// (Won't spoil it by saying which one!)
void eulerphi(u32 n, u32 *phi)
{
  for (u32 i=1; i<=n; i++) {
    phi[i]=i;
  }
  for (u32 i=2; i<=n; i++) {
    if (phi[i]==i) {                // i is prime
      phi[i]-=1;                    // phi(i)=i-1
      for (u32 j=i+i; j<=n; j+=i) { // Handle all multiples of i
        phi[j]=phi[j]/i*(i-1);      // Apply (i-1)/i to this multiple of i
      }
    }
  }
}

const int MAXD = 500000;

// Should only be a million x a million, but it's way too slow.
// Maybe last time I attempted BF it was only 100 million that worked?
// Thought m x m seemed a bit much to try!
void test01()
{
	unsigned long long count = 0;
	for(int d=1; d<=MAXD; ++d)
	{
		for(int n=1; n<d; ++n)
		{
			++count;
		}
	}
	cout << "count=" << count << endl;
}

// Pretty sure the answer is the sum of euler phi values from 2 to d
void test02()
{
	eulerphi(PHISIZE, gPhi);

	u32 count = 0;
	for(u32 i=2; i<=PHISIZE; ++i)
	{
		count += gPhi[i];
	}
	cout << "count=" << count << endl;
}

int main()
{
	test02();
	return 0;
}