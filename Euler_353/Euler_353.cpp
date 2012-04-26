// Project Euler 353 - Risky Moon
// Adrian Dale
//
/*
n=1 r=1 lp=4 d=0.5
n=2 r=3 lp=10 d=0.236045666446135
n=3 r=7 lp=16 d=0.178494399755149
n=4 r=15 lp=44 d=0.10714794796212
n=5 r=31 lp=52 d=0.0874453003437861
n=6 r=63 lp=232 d=0.0415156841491886
n=7 r=127 lp=196 d=0.0463421852318835
n=8 r=255 lp=656 d=0.0239515581045124
n=9 r=511 lp=992 d=0.0179048895428282
n=10 r=1023 lp=3220 d=0.010340579258887
n=11 r=2047 lp=3344 d=0.0109810920240292
n=12 r=4095 lp=14936 d=0.00502894983572062
n=13 r=8191 lp=12292 d=0.00560073081502647
n=14 r=16383 lp=43540 d=0.00292629241037604
n=15 r=32767 lp=68164 d=0.00226075719964466
Answer=1.27598603307929
(ie Answer=1.2759860331 to 10dp)

Congratulations, the answer you gave to problem 353 is correct.

You are the 73rd person to have solved this problem.

You have earned 1 new award:

    One In A Hundred: Be among the first hundred to solve a problem
*/
#include <iostream>
#include <ostream>
#include <iomanip>
#include <vector>
#include <set>
#include <tuple>
#include <algorithm>
#include <cmath>
using namespace std;

// Using the puzzle's ordering this is z,x,y if I want to
// sort coords by z
typedef tuple<int,int,int> Coord;

// Nice way to output coordinates
ostream &operator<<(ostream &out, const Coord &a)
{
	// Note weird tuple accessing syntax!
	out << "(" << get<0>(a) << "," << get<1>(a) << "," << get<2>(a) << ")";
	return out;
}

// Lattice Points
vector<Coord> LP;

double const Pi=4.0*atan(1.0); // Pi=3.14

// Radius
int r=7;

// Return the distance between points a and b
// on circle of radius r
inline double GCdistance( Coord const &a, Coord const &b )
{
	double dotProduct = get<0>(a)*get<0>(b) +
						get<1>(a)*get<1>(b) +
						get<2>(a)*get<2>(b);
	double cosA = dotProduct / static_cast<double>(r*r);

	return static_cast<double>(r) * acos(cosA);
}

// Risk is defined as (distance/(pi*r))^2
inline double risk( double dist )
{
	double rsk = dist / (Pi*static_cast<double>(r));
	return rsk * rsk;
}

int getLatticePoints()
{
	LP.clear();
	int r2 = r*r; // Minor speed optimisation
	for(int x=0; x<=r; ++x)
	{
		for(int y=x; y<=r; ++y)
		{
			int x2y2 = (x*x)+(y*y);
			if (x2y2 > r2)
				break;
			int c2=r2-x2y2;
			int z = static_cast<int>(sqrt(static_cast<double>(r2-x2y2))/*+0.5*/);
			if (z*z == c2)
			{
				// The sort/unique will
				// handle cases where x==y or x==0, etc.
				// A bit lazy and inefficient, though.
				Coord a;
				a = make_tuple(x,y,z); LP.push_back(a);
				a = make_tuple(-x,y,z); LP.push_back(a);
				a = make_tuple(y,x,z); LP.push_back(a);
				a = make_tuple(-y,x,z); LP.push_back(a);
			}
		}
	}
	return LP.size();
}

void mirrorLPs()
{
	// Mirroring of the points is now done as they
	// are generated. I left the sort/unique code here, though.
	sort(LP.begin(), LP.end());
	auto it = unique(LP.begin(), LP.end());
	LP.resize( it - LP.begin() );
}

double Dijkstra()
{
	double r2 = static_cast<double>(r*r);

	vector<double> cost(LP.size(), r2);

	// Don't need (or use) this. It could be used to re-create
	// the path once we're done, though.
	vector<int> previous(LP.size(), -1);

	// Starting node is 0
	cost[0] = 0.0;

	// Q is all nodes in the graph
	vector<pair<double, int> > Q;
	for(int i=0; i<LP.size(); ++i)
		Q.push_back(make_pair(r2, i));
	Q[0] = make_pair(0.0, 0);
		

	while( Q.empty() == false )
	{
		// u = vertex in Q with smallest distance in dist
		pair<double, int> up = *Q.begin();
		
		int u = up.second;
		// Remove u from Q
		Q.erase(Q.begin());
		
		// Can terminate if u is the target
		if ( u == LP.size()-1 )
			break;

		// For each neighbour v of u
		// ie all lower nodes still in Q
		// Could trim this to, say, nodes within a certain distance
		for(size_t i=0; i<Q.size(); ++i)
		{
			pair<double, int> vp = Q[i];
			int v = vp.second;
			
			double alt = cost[u] + risk( GCdistance(LP[u], LP[v]) );
			if (alt < cost[v])
			{
				// Relax (u,v,a)
				cost[v] = alt;
				previous[v] = u;
				// Decrease key v in Q
				Q[i] = make_pair(alt, v);
			}
		}
		// Is this really slow?
		// Not the best implementation of Dijksta.
		// Should try using a set to hold Q
		sort(Q.begin(), Q.end());
	}

	return cost[ LP.size()-1 ];
}

void test03()
{
	double dsum = 0.0;
	int twoPow = 2;
	for(int n=1; n<=15; ++n)
	{
		r=twoPow-1;
		getLatticePoints();
		mirrorLPs();
		double dijk = Dijkstra();
		dsum += dijk;
		cout << "n=" << n << " r=" << twoPow - 1 << " lp=" << LP.size() << " d="
			<< setprecision(15) << dijk << endl;
		twoPow = twoPow << 1;
	}
	cout << "Answer=" << setprecision(15) << dsum << endl;
}

int main()
{
	test03();
	return 0;
}