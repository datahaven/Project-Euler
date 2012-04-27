// Project Euler 339 - Peredur fab Efrawg
// Adrian Dale
//
// Notes:
// http://www.amstat.org/sections/srms/proceedings/papers/1981_122.pdf
// http://www.math.uah.edu/stat/markov/Ehrenfest.xhtml
// http://www.dartmouth.edu/~chance/teaching_aids/books_articles/probability_book/Chapter11.pdf
// This is the maths: http://www.math.ncsu.edu/ma114/PDF/6.4.pdf
// http://eaton.math.rpi.edu/CourseMaterials/Fall08/PK6790/stochnotes100608.pdf

#include <iostream>
#include <iomanip>
using namespace std;

const int N = 10000;

// Calculates the probability that we'll go either left
// or right at this node.
// Takes prob of left, prob of right, then prob of
// move from right to left child/right child.
// Assumes an initial move to the right is an absorbing state
void calculateProbabilities(double &left, double &right,
			double l1, double r1,
			double l2, double r2)
{
	// N inv = I - Q
	double n11 = 1.0; double n12 = 0.0 - r1;
	double n21 = 0.0 - l2;  double n22 = 1.0;
	double det = (n11*n22 - n12*n21);

	left = 0.0;
	right = 0.0;

	double r11 = 0.0; double r12 = l1;
	double r21 = r2; double r22 = 0.0;

	// ni is invert of n, hence ni is N
	double ni11 = n22 / det; double ni12 = -1.0 * n12 / det;
	double ni21 = -1.0 * n21 / det; double ni22 = n11 / det;

	// NB back to front in matrix
	// B=NR and we just need the top row
	right = ni11*r11 + ni12*r21;
	left = ni11*r12 + ni12*r22;
}

// Calculate the probability of either getting absorbed (the right branch)
// or going to the left, starting with startw/startb sheep and finishing with
// endw white sheep
void calculateBranch(double &left, double &right, int startw, int startb, int endw)
{
	int nw = startw;
	int nb = startb;

	double l2 = (double)(nw-1) / (double)(nw+nb);
	double r2 = (double)(nb+1) / (double)(nw+nb);

	while( nw != endw )
	{
		double l1 = (double)nw / (double)(nw+nb);
		double r1 = (double)nb / (double)(nw+nb);
		calculateProbabilities(left, right, l1, r1, l2, r2);
		l2 = left;
		r2 = right;
		++nw; --nb;
	}
}

void calculateTree()
{
	int startw = 2;
	int startb = 2*N-startw;
	int endw = N;
	double expected = 0.0;
	double left = 0.0;
	double right = 0.0;
	// Calculate probabilities for the two branches to the right
	// of the starting state
	calculateBranch(left, right, startw, startb, endw);

	expected += 2.0 * N * right * 0.5;

	double left2;
	calculateBranch(left2, right, startw, startb-1, endw);

	expected += (2.0*N-1.0) * 0.5 * left * right;

	double leftfixed = 0.5 * left * left2 + 0.5;

	// Calculate branches in left side of tree
	for( int i=N-2; i>=2; --i)
	{	
		int topw = i;
		int topb = topw+1;
		topw = topw+topw-1;
		
		calculateBranch(left, right, startw, topw, i+1);

		expected += (double)(topw+startw) * leftfixed * right ;
		
		// Update static probability ready to calculate next branch
		leftfixed *= left;
	}
	
	// Last two branches are a static calculation
	expected += leftfixed * 3.0 * 2.0 / 3.0;
	expected += leftfixed * 1.0 * 1.0 / 3.0;

	// Output the final answer
	cout << "E(" << N << ")=" << fixed << setprecision(7) << expected << endl;
}

int main()
{
	calculateTree();
	return 0;
}