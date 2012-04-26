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
	// Note funky new tuple accessing syntax!
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

#if 0
				// Only produces top quadrant
				Coord a = make_tuple(x,y,z);
				LP.push_back(a);
				if (y!=x)
				{
					Coord b = make_tuple(y,x,z);
					LP.push_back(b);
				}
#endif
				// Try whole sphere - the sort/unique will
				// handle cases where x==y or x==0, etc
				Coord a;
				a = make_tuple(x,y,z); LP.push_back(a);
				//a = make_tuple(x,y,-z); LP.push_back(a);
				//a = make_tuple(x,-y,z); LP.push_back(a);
				//a = make_tuple(x,-y,-z); LP.push_back(a);
				a = make_tuple(-x,y,z); LP.push_back(a);
				//a = make_tuple(-x,y,-z); LP.push_back(a);
				//a = make_tuple(-x,-y,z); LP.push_back(a);
				//a = make_tuple(-x,-y,-z); LP.push_back(a);

				a = make_tuple(y,x,z); LP.push_back(a);
				//a = make_tuple(y,x,-z); LP.push_back(a);
				//a = make_tuple(y,-x,z); LP.push_back(a);
				//a = make_tuple(y,-x,-z); LP.push_back(a);
				a = make_tuple(-y,x,z); LP.push_back(a);
				//a = make_tuple(-y,x,-z); LP.push_back(a);
				//a = make_tuple(-y,-x,z); LP.push_back(a);
				//a = make_tuple(-y,-x,-z); LP.push_back(a);
			}
		}
	}
	return LP.size();
}

void mirrorLPs()
{
	// Would probably be slightly quicker to do this
	// in getLPs
	size_t LPsize = LP.size();
	for(size_t i=0; i<LPsize; ++i)
	{
		Coord mirror = LP[i];
		if (get<0>(mirror) != 0)
		{
			get<0>(mirror) *= -1;
		//	LP.push_back(mirror);
		}

		
	}
	// Now sort
	sort(LP.begin(), LP.end());
	auto it = unique(LP.begin(), LP.end());
	LP.resize( it - LP.begin() );

	//for(auto itt=LP.begin(); itt != LP.end(); ++itt)
	//	cout << *itt;
	//cout << endl;
}

#if 0
void getLatticePoints_SLOW()
{
	// Find all lattice points on sphere of radius 7
	LP.clear();
	// In mathlab this is r(k, n^2)
	// ie number of ways to get sum of k==3 squares == n^2
	//
	// This is the slow way!
	// Suspect we can optimise, but not massively improve.
	// eg do a=0, b=a, c=b, then perm results afterwards
	// eg do break early once r^2 is exceeded.
	// Must be some other logic?
	for(int a=0; a<=r; ++a)
	{
		for(int b=0; b<=r; ++b)
		{
			for(int c=0; c<=r; ++c)
			{
				if ((a*a)+(b*b)+(c*c)==(r*r))
				{
					Coord lp = make_tuple(a,b,c);
					LP.push_back(lp);

					// Add -ve z ones, too
					if (a != 0)
					{
						Coord lpz = make_tuple(-a,b,c);
						LP.push_back(lpz);
					}
				}
			}
		}
	}
	//cout << "no of lattice pts = " << LP.size() << endl;

	// Sort in order of descending Z so we can start search at north pole
	sort(LP.begin(), LP.end(), greater<Coord>());

	//for_each(LP.begin(), LP.end(), [&](Coord c)
	//{
	//	cout << c << endl;
	//});
}
#endif

#if 0
double minDistance( vector<int> route, double distanceSoFar )
{
	// See if final coord on route is an end point
	int lastPoint = route.back();
	Coord &lastCoord = LP[lastPoint];
	// if it is, we're done at this depth
	if (get<0>(lastCoord) == -r)
		return distanceSoFar;

	// Find closest next point to current lastCoord
	// This is done recursively, rather than greedily
	double minDist = (r*r);
	for(int i=lastPoint+1; i<LP.size(); ++i)
	{
		// If z doesn't decrease then no need to search that direction.
		// (Seems to be correct!))
		if (get<0>(lastCoord) == get<0>(LP[i]))
			continue;

		route.push_back(i);
		double dist = minDistance(route, distanceSoFar + risk(r, distance(r, lastCoord, LP[i])));
		if ( dist < minDist)
		{
			minDist = dist;
		}
		route.pop_back();
	}
	return minDist;
}
#endif

double Dijkstra()
{
	double r2 = static_cast<double>(r*r);

	vector<double> cost(LP.size(), r2);
	vector<int> previous(LP.size(), -1);

	// Starting node is 0
	cost[0] = 0.0;

	// Q is all nodes in the graph
	vector<int> Q;
	// Bet there's a neat C++ way to do this!
	for(size_t i=0; i<LP.size(); ++i)
		Q.push_back(i);

	while( Q.empty() == false )
	{
		// u = vertex in Q with smallest distance in dist
		vector<int>::iterator uit = Q.end();
		double minCost = r2;
		for(auto it=Q.begin(); it != Q.end(); ++it)
		{
			if (cost[ *it ] < minCost)
			{
				minCost = cost[ *it ];
				uit = it;
			}
		}

		if (uit == Q.end())
		{
			// I think this only happens if there are 
			// inaccessible vertices
			// We'll get this because all "upwards" vertices are
			// inaccessible
			//cout << "u not found" << endl;
			break;
		}

		int u = *uit;
		// Remove u from Q
		Q.erase(uit);
		
		// Can terminate if u is the target
		if ( u == LP.size()-1 )
		{
			//cout << "Target reached" << endl;
			break;
		}

		// For each neighbour v of u
		// ie all lower nodes still in Q
		for(size_t i=0; i<Q.size(); ++i)
		{
			int v = Q[i];
			// This bit was causing a breakage for big values of r
			// Turns out we DO need to go up-over sometimes
			//if ( get<0>(LP[u]) > get<0>(LP[v]) )
			//	continue;
			
			double alt = cost[u] + risk( GCdistance(LP[u], LP[v]) );
			if (alt < cost[v])
			{
				// Relax (u,v,a)
				cost[v] = alt;
				previous[v] = u;
				// Decrease key v in Q
				// - no need, since my Q isn't ordered
			}
		}
	}
	//cout << "Finished: " << r << endl;

	return cost[ LP.size()-1 ];
}

double Dijkstra_Set()
{
	double r2 = static_cast<double>(r*r);

	vector<double> cost(LP.size(), r2);
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
		{
			//cout << "Target reached" << endl;
			break;
		}

		// For each neighbour v of u
		// ie all lower nodes still in Q
		for(size_t i=0; i<Q.size(); ++i)
		{
			pair<double, int> vp = Q[i];
			int v = vp.second;
			//if ( get<0>(LP[u]) > get<0>(LP[v]) )
			//	continue;
			
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
				sort(Q.begin(), Q.end());
	}
	//cout << "Finished: " << r << endl;

	return cost[ LP.size()-1 ];
}
#if 0
// Try some routes out
void test02()
{
	// This gets stuck at r=15, which is 44 LPs.
	// So, need to improve search
	// As well as needing to improve LP generation.
	for( r=1; r<20; ++r )
	{
		cout << "Generating LPs: ";
		getLatticePoints();
		cout << LP.size() << endl;

		cout << "Searching route" << endl;
		vector<int> route;
		route.push_back(0);
		double d=minDistance( route, 0.0 );
		cout << "r=" << r << " d=" << d << endl;
	}
}
#endif

void test03()
{
	double dsum = 0.0;
	int twoPow = 2;
	for(int n=1; n<=15; ++n)
	{
		r=twoPow-1;
		r=4681;
		getLatticePoints();
		mirrorLPs();
		double dijk = Dijkstra_Set();
		dsum += dijk;
		cout << "n=" << n << " r=" << twoPow - 1 << " lp=" << LP.size() << " d="
			<< setprecision(15) << dijk << endl;
		twoPow = twoPow << 1;
	}
	cout << "Answer=" << setprecision(15) << dsum << endl;
}

int main()
{
	cout << numeric_limits<unsigned long long>::max() << endl;
	test03();

	return 0;
}