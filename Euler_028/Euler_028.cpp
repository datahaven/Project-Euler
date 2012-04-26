// Project Euler 028
// Adrian Dale
#include <iostream>
using namespace std;
int main( int argc, char **argv )
{
	int retstatus = 0;
	__int64 sum=1;
	__int64 trail=1;
	for( int i=1; i<501; ++i )
	{
		for( int j=0; j<4; ++j )
		{
			trail += 2*i - 1;
			sum += ++trail;
			cout << trail << " ";
		}
	}      
	cout << "sum=" << sum << endl;
	return( retstatus );
}