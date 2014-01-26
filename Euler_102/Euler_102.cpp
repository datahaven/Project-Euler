// Project Euler 102 - Triangle Containment
// http://projecteuler.net/problem=102
// Adrian Dale 26/01/2014

/*
Congratulations, the answer you gave to problem 102 is correct.

You are the 11076th person to have solved this problem.

NB I tweaked the input file slightly to remove the trailing end of line
character. Probably should have altered the code to handle this case,
instead.
*/
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

typedef int LL;

struct Vertex
{
	LL x;
	LL y;
};

// Code from:
// http://stackoverflow.com/questions/2049582/how-to-determine-a-point-in-a-triangle
// A bit cheaty, given that this basically solves the entire problem!
// I found it by accident whilst investigating Euler 456
// I modified it to be slightly faster by assuming it always asks about the origin
LL signO(const Vertex & p2, const Vertex & p3)
{
	return (-p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (-p3.y);
}

bool OriginInTriangle(const Vertex & v1, const Vertex & v2, const Vertex & v3)
{
	bool b1, b2, b3;

	b1 = signO(v1, v2) < 0;
	b2 = signO(v2, v3) < 0;
	b3 = signO(v3, v1) < 0;

	return ((b1 == b2) && (b2 == b3));
}

LL getNextCoord(istringstream &p)
{
	string coord;
	getline(p, coord, ',');
	istringstream ss(coord);
	LL result;
	ss >> result;
	return result;
}

void solve()
{
	LL tcoCount = 0;
	ifstream trianglesFile;
	trianglesFile.open("triangles.txt");
	if (trianglesFile.is_open() == false)
	{
		cout << "ERROR - couldn't open triangles.txt" << endl;
		return;
	}
	while (!trianglesFile.eof())
	{
		string triangleLine;
		trianglesFile >> triangleLine;
		istringstream parser(triangleLine);

		Vertex v0;
		v0.x = getNextCoord(parser);
		v0.y = getNextCoord(parser);
		Vertex v1;
		v1.x = getNextCoord(parser);
		v1.y = getNextCoord(parser);
		Vertex v2;
		v2.x = getNextCoord(parser);
		v2.y = getNextCoord(parser);

		//cout << v0.x << "," << v0.y << " ";
		//cout << v1.x << "," << v1.y << " ";
		//cout << v2.x << "," << v2.y << " " << endl;
		
		if (OriginInTriangle(v0, v1, v2) == true)
		{
			++tcoCount;
		}

	}
	trianglesFile.close();
	cout << "Answer=" << tcoCount << endl;
}


int main()
{
	solve();
	return 0;
}