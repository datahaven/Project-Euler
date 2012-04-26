// Project_018.cpp 
// Adrian Dale 10/01/2009
//
// Find the maximum path through the triangle
// ...
// Answer: 1074 for project 18
// (Took exactly 20 mins to code this.)
//
// Same code also works for Euler_067
#if 0
Here is a neat J version:

prob67=: 3 : 0
while. 1<#y. do.
   y.=. ((_2{y.)+(2>./\_1{y.),0) _2}y.
   y.=.}:y.
end.
{.,y.
)

y. is zero-padded matrix of data given so it is 100x100
It executes in 0.00039 secs.
====
And another even shorter one:

{. (+ 0: ,~ 2: >./\ ])/ m

m is a padded matrix of the triangle. The function
f[x,y]: x+(pairwise max y),0
is inserted between the rows of the matrix. Take the
first element of the resultant vector. Voila. 

#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

// Solution is to work upwards and add the max of the two
// child nodes to the parent node until we get to the top.
int main(int argc, char* argv[])
{
	vector< vector<int> > triangle;

	// Read the file
	ifstream puzFile( "Triangle_18.txt" );
	string fileline;

	int triRowNo = 0;
	while( getline(puzFile, fileline) )
	{
		istringstream sr(fileline);
		vector<int> triRow;
		triRow.resize(triRowNo+1);

		for( int i=0; i<=triRowNo; ++i )
			sr >> triRow[i];

		triangle.push_back(triRow);
		triRowNo++;
	}

	int triRows = triRowNo;

	// Test dump
	for( int i=0; i<triRows; ++i )
	{
		for( int j=0; j <=i; ++j )
			cout << triangle[i][j] << " ";
		cout << endl;
	}

	// The magic bits!
	for( int i=triRows-2; i>=0; --i)
	{
		// Starting from second last row,
		// add the max of the two children to each node
		for( int j=0; j<=i; ++j )
			triangle[i][j] += max(triangle[i+1][j], triangle[i+1][j+1]);
	}

	cout << "Answer: " << triangle[0][0] << endl;

	return 0;
}
