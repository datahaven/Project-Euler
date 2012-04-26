// Project Euler 70
// Adrian Dale
// 08/10/2011
//
// 8319823
// You are the 6499th person to have solved this problem.
// Nothing clever - calculates all values of phi(n) up to 10^^7
// and checks them all to see if they are a perm of n.
// Uses conversion to a string and sorting to do that, so
// code isn't super-quick.
// Takes about 40 seconds to produce an answer
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;
typedef unsigned int u32;

const u32 PHISIZE = 10000000;
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

bool is_perm(u32 n, u32 phin)
{
	ostringstream nss;
	nss << n;

	ostringstream phinss;
	phinss << phin;

	string nstr = nss.str();
	string phinstr = phinss.str();

	if (nstr.length() != phinstr.length())
		return false;

	sort(nstr.begin(), nstr.end());
	sort(phinstr.begin(), phinstr.end());

	return nstr == phinstr;
}

int main()
{
	eulerphi(PHISIZE, gPhi);
	double minRatio = 10000000.0;
	u32 mini=0;
	for(u32 i=2; i<PHISIZE; ++i)
	{
		if ( is_perm(i, gPhi[i]) == true )
		{
			//cout << i << " " << gPhi[i] << endl;
			double ratio = (double)i / (double)gPhi[i];
			if (ratio < minRatio)
			{
				minRatio=ratio;
				mini = i;
			}
		}
	}
	cout << "answer=" << mini << endl;
	return 0;
}