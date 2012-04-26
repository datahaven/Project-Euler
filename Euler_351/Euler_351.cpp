// Project Euler 351
// Hexagonal Orchards
//
// by Adrian Dale
//
// You are the 202nd person to have solved this problem.
// 11762187201804552
//
// Didn't use this program in the end, though I think it would have worked.
// Probably would take about seven minutes.
// Could have been overflow issues, though?

// In the end I used PARI to calculate the sum of the euler function from 1 to 100000000,
// subtracted 2. (Or could have just started from 3)
// a(n)=sum(k=1, n, eulerphi(k))
// Added 899999988 which I'd already calculated is the number of hidden points on 
// symmetrical lines
// Calculated total area, then subtracted the euler phi sum and multiplied by 6

// Looking at forum, the only thing I really missed was using a sieve to
// calculate phi, which I'd pretty much knew anyway, but hadn't looked up.

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

typedef unsigned long long ull;

const ull N = 100000000;

#include <windows.h> // For QPC timer

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

// TODO - Check this needs to be ull.
// Would be better if we could use a smaller size
int totient[100000000];

int phi(int n) 
{ 
	int result = n; 
	for(int i=2;i*i <= n;i++) 
	{ 
		if (n % i == 0) result -= result / i; 
		while (n % i == 0) n /= i; 
	} 
	if (n > 1) result -= result / n; 
	return result; 
} 

ull gcd(ull a, ull b)
{
        if(b == 0)
        {
                return a;
        }
        else
        {
                return gcd(b, a % b);
        }
}

/*
Totient(positive integer N)
  integer t <-- 0;
  for i <-- 1 to N-1, do:
      if GCD(i,N) = 1 then
            t <-- t + 1;
      end if
  next
  return t;
End
*/
void fillTotientCache(int n)
{
	// Initialise cache to 0
	memset(totient, 0, sizeof(0)*100000000);
}

ull test01()
{
	ull count=0;
	// Step along the start coords for each row
	for(ull x=3, y=1; x<=N/2; x+=2, ++y)
	{
		//cout << "ROW: " << x << ", " << y << endl;

		// Try each point along the row to see if it is hidden
		for(ull px=x; px<=N/2; ++px)
		{
			// Only need to check when px can't be divided by y,
			// otherwise that means we've already checked this gradient
			if ( gcd(px,y)==1 )
			{
				// Need to see how many hidden points there are on the line
				// between 0,0 and px,y
				ull hidden_count = N/px - 1;
				//cout << px << ", " << y << "(" << hidden_count << ")" << "   ";

				count += hidden_count;
			}
		}
		//cout << endl;
	}

	return count;
}

ull test02()
{
	bool debug = false;
	ull count=0;
	ull profile1 = 0;
	ull profile2 = 0;
	// Step along the start coords for each row
	for(ull x=3, y=1; x<=N/2; x+=2, ++y)
	{
		if (debug)
			cout << "ROW: " << x << ", " << y << endl;

		// Try each point along the row to see if it is hidden
		// TODO - I suspect this needs to increment by an amount that
		// isn't always 1.
		// I checked using y and that doesn't work!
		for(ull px=x; px<=N/2; px+=1)
		{
			// Only need to check when px can't be divided by y,
			// otherwise that means we've already checked this gradient
			if ( gcd(px,y)==1 )
			{
				++profile1;
				// Need to see how many hidden points there are on the line
				// between 0,0 and px,y
				ull hidden_count = N/px - 1;
				if (debug)
					cout << px << ", " << y << "(" << hidden_count << ")" << "   ";

				count += hidden_count;
			}
			else
				++profile2;
		}
		if (debug)
			cout << endl;
	}

	cout << "profile1=" << profile1 << endl;
	cout << "profile2=" << profile2 << endl;
	cout << "interior pts=" << count << endl;
	return count;
}

ull test03()
{
	bool debug = false;
	ull count=0;
	ull profile1 = 0;
	ull profile2 = 0;
	// Calculate no. of visible points for each row
	//for(ull y=1; y<N/2; ++y)
	//{
	//	if (debug)
	//		cout << "ROW: " << y << endl;

	//	count += phi(N) - phi(y);
	//}

	for(int i=3; i<=N; ++i)
		count += phi(i);

	// This counts total number of points between our
	// two lines. (Not including lines)
	ull total_points = 0;
	ull a = (N-2)/2;
	total_points = (a * a) + a;
	cout << "tp=" << total_points << endl;

	cout << "profile1=" << profile1 << endl;
	cout << "interior pts=" << count << endl;
	return 2*total_points - count;
}

int main(int argc, char** argv)
{
	cout << "Project Euler 351" << endl;

	const long long start = counter();
    
	fillTotientCache(100000000);

	unsigned long long answer = 0;

	// First row
	answer += N - 1;
	
	// Mid-line of first triangle
	answer += (N/2 - 1);
	answer *= 6;

	cout << "answer so far = " << answer << endl;

	// Now the hard part!
	// 12 * no. of points between mid line and base line
	ull t3 = test03();
	//ull t2 = test02();
	answer += 6*t3;
	
	cout << "answer=" << answer << endl;
	
	const long long finish = counter();
	cout << "Time Taken: " << format_time(start, finish) << endl;

	return 0;
}