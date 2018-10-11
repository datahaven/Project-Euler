// Project Euler 321
// Swapping Counters
// Adrian Dale
// 10/10/2018

// Congratulations, the answer you gave to problem 321 is correct.
// You are the 1364th person to have solved this problem.
// This problem had a difficulty rating of 30 % .The highest difficulty rating you have solved so far is 100 %
#include <iostream>
#include <utility>
#include <vector>

#if 0
Solution steps taken:
Write a solver for the basic puzzle.
That's fn test01() which can build a board of size n, then calls Solve() to recursively try out
all possible moves in order to find the fastest solution.
Output below:

Solution Found : B.R 3
Solution Found : B.R 3
Solution Found : BB.RR 8
Solution Found : BB.RR 8
Solution Found : BBB.RRR 15
Solution Found : BBB.RRR 15
Solution Found : BBBB.RRRR 24
Solution Found : BBBB.RRRR 24
Solution Found : BBBBB.RRRRR 35
Solution Found : BBBBB.RRRRR 35
Solution Found : BBBBBB.RRRRRR 48
Solution Found : BBBBBB.RRRRRR 48
Solution Found : BBBBBBB.RRRRRRR 63
Solution Found : BBBBBBB.RRRRRRR 63
Solution Found : BBBBBBBB.RRRRRRRR 80
Solution Found : BBBBBBBB.RRRRRRRR 80
Solution Found : BBBBBBBBB.RRRRRRRRR 99
Solution Found : BBBBBBBBB.RRRRRRRRR 99
Solution Found : BBBBBBBBBB.RRRRRRRRRR 120
Solution Found : BBBBBBBBBB.RRRRRRRRRR 120
Solution Found : BBBBBBBBBBB.RRRRRRRRRRR 143
Solution Found : BBBBBBBBBBB.RRRRRRRRRRR 143
Solution Found : BBBBBBBBBBBB.RRRRRRRRRRRR 168
Solution Found : BBBBBBBBBBBB.RRRRRRRRRRRR 168
Solution Found : BBBBBBBBBBBBB.RRRRRRRRRRRRR 195
Solution Found : BBBBBBBBBBBBB.RRRRRRRRRRRRR 195
Solution Found : BBBBBBBBBBBBBB.RRRRRRRRRRRRRR 224
Solution Found : BBBBBBBBBBBBBB.RRRRRRRRRRRRRR 224
Solution Found : BBBBBBBBBBBBBBB.RRRRRRRRRRRRRRR 255
Solution Found : BBBBBBBBBBBBBBB.RRRRRRRRRRRRRRR 255
Solution Found : BBBBBBBBBBBBBBBB.RRRRRRRRRRRRRRRR 288
Solution Found : BBBBBBBBBBBBBBBB.RRRRRRRRRRRRRRRR 288
Solution Found : BBBBBBBBBBBBBBBBB.RRRRRRRRRRRRRRRRR 323
Solution Found : BBBBBBBBBBBBBBBBB.RRRRRRRRRRRRRRRRR 323
Solution Found : BBBBBBBBBBBBBBBBBB.RRRRRRRRRRRRRRRRRR 360
Solution Found : BBBBBBBBBBBBBBBBBB.RRRRRRRRRRRRRRRRRR 360

From this we can see a pattern:
Solution is (n + 1) ^ 2 - 1
oeis.com helped out here ;-)

Now test02() tries out all values of n in the above formula and tests
to see if the number produced is a triangle number.
Note that the perfect square test functions both give incorrect answers for large
input, as the large ints don't convert to floats precisely enough

I re-wrote this code in python (e321.py) which handles large ints better, and also with
a better square checking function. That will only get values up to about 20
terms in the sequence before becoming too slow to run.

However, that's enough to see that the Triangle numbers produced match sequence
A006454 in the OEIS. This conveniently gives values for the 40 terms we need, but
doesn't tell us which values of n produce those numbers

We know that (n+1)^2 -1 == X, where X is A006454, so after that we can use the
standard quadratic equation solution to find a value for n for each of the A006454
numbers that we need.

I did this manually with Wolfram Alpha
http ://www.wolframalpha.com/widgets/view.jsp?id=9492504056afc5ada7c27e54c54e4bec
Then summed up the numbers in python to get the final answer
e321_2.py
#endif

using namespace std;

enum class CellType { Empty, Red, Blue };

typedef unsigned long long ULL;

class Board
{
public:
	Board() {}
	~Board() {}
	void AddCell(CellType cellType);
	vector<pair<int, int>> GetAllPossibleMoves() const;
	void MakeMove(pair<int, int> move);
	void UndoMove(pair<int, int> move);
	bool IsSolved() const;
private:
	int m_BoardSize;
	vector<CellType> m_Cells;
	friend ostream& operator<<(ostream &out, const Board& o);
};

ostream &operator<<(ostream &os, Board const &b)
{
	for (CellType ct : b.m_Cells)
	{
		if (ct == CellType::Red)
			os << "R";
		else if (ct == CellType::Blue)
			os << "B";
		else if (ct == CellType::Empty)
			os << ".";
	}
	return os;
}
void Board::AddCell(CellType ct)
{
	m_Cells.push_back(ct);
}
vector<pair<int,int>> Board::GetAllPossibleMoves() const
{
	vector<pair<int, int>> moves;
	int spacePos = find(m_Cells.begin(), m_Cells.end(), CellType::Empty) - m_Cells.begin();
	if (spacePos < m_Cells.size() - 1 && m_Cells[spacePos + 1] == CellType::Blue)
	{
		moves.push_back(make_pair(spacePos + 1, spacePos));
	}
	if (spacePos < m_Cells.size() - 2 && m_Cells[spacePos + 2] == CellType::Blue)
	{
		moves.push_back(make_pair(spacePos + 2, spacePos));
	}
	if (spacePos > 0 && m_Cells[spacePos - 1] == CellType::Red)
	{
		moves.push_back(make_pair(spacePos - 1, spacePos));
	}
	if (spacePos > 1 && m_Cells[spacePos - 2] == CellType::Red)
	{
		moves.push_back(make_pair(spacePos - 2, spacePos));
	}
	return moves;
}

void Board::MakeMove(pair<int, int> move)
{
	CellType t = m_Cells[move.first];
	m_Cells[move.first] = m_Cells[move.second];
	m_Cells[move.second] = t;
}

void Board::UndoMove(pair<int, int> move)
{
	// Since it's just a swap
	MakeMove(move);
}

bool Board::IsSolved() const
{
	// I got my Blue/Red on opposite sides to the puzzle definition,
	// which could be slightly confusing, although it doesn't really matter
	int n = m_Cells.size() / 2;
	for (int i = 0; i < n; ++i)
	{
		if (m_Cells[i] != CellType::Blue)
			return false;
		if (m_Cells[i+n+1] != CellType::Red)
			return false;
	}
	return true;
}
int Solve(Board b, vector<pair<int, int>> movesSoFar)
{
	if (b.IsSolved())
	{
		cout << "Solution Found: " << b << " " << movesSoFar.size() << endl;
		return movesSoFar.size();
	}

	vector<pair<int, int>> moves = b.GetAllPossibleMoves();
	for (auto &m : moves)
	{
		b.MakeMove(m);
		movesSoFar.push_back(m);
		Solve(b, movesSoFar);
		b.UndoMove(m);
		movesSoFar.pop_back();
	}
	return movesSoFar.size();
}
void test01(int n)
{
	Board board;
	for (int i = 0; i < n; ++i)
		board.AddCell(CellType::Red);
	board.AddCell(CellType::Empty);
	for (int i = 0; i < n; ++i)
		board.AddCell(CellType::Blue);

	vector<pair<int, int>> moves;
	cout << "n=" << n << " => " << Solve(board, moves) << endl;
	//cout << "-------" << endl;
	//cout << board << endl;
}

bool IsPerfectSquare_OLD(ULL n)
{
	// https://stackoverflow.com/questions/1549941/perfect-square-and-perfect-cube
	ULL root = sqrt(n);
	return n == root * root;
}
bool IsPerfectSquare(ULL n)
{
	if ((n & 7ull) == 1ull || (n & 31ull) == 4ull || (n & 127ull) == 16ull || (n & 191ull) == 0ull)
	{
		// return n is probably square
		// so test it properly!
		ULL nroot = static_cast<ULL>(sqrt(static_cast<double>(n)) + 0.5);
		return nroot * nroot == n;
	}
	//else
	//return n is definitely not square
	return false;
}
//def isPerfectSquare(n) :
//	sq = long(math.sqrt(n))
//	return (sq ** 2) == n

bool IsTriangle(ULL x)
{
	int part = 1 + (8 * x);
	if (part % 2 == 0)
		return false;
	return IsPerfectSquare(part);
}
void test02()
{
	ULL numTerms = 0;
	ULL sum = 0;
	for (ULL n = 1; n < 1000000; ++n)
	{
		ULL swapCount = ((n + 1)*(n + 1)) - 1;
		if (IsTriangle(swapCount))
		{
			++numTerms;
			cout << "(" << n << "," << swapCount<<")" << endl;
			sum += n;
		}
		if (numTerms >= 40)
		{
			cout << "Solution Found!" << endl;
			break;
		}
	}
	cout << "sum=" << sum << endl;
#if 0
	1, 3, 10, 22, 63, 133, 372, 780, 2173, 4551, 12670, 245240, 245305,
		311107, 329573, 329694, 370282, 387791, 390497, 411253, 445715,
		454056, 493847, 513945, 563803, 567573, 579728, 605149, 665547,
		774132, 775114, 775808, 801673, 824805, 868309, 884141, 894151, 897732,
		906673, 933756, sum = 17267177
		Except site says this is wrong :-(
		I suspect the perfect square fn, as I've had trouble with those before
		It works for first 5 numbers

I tried this in oeis and it is A006454 (maybe!), although it stops matching
my results after a while
They have a list of this sequence:
https://oeis.org/A006454/b006454.txt

(1, 3)
(3, 15)
(10, 120)
(22, 528)
(63, 4095)
(133, 17955)
(372, 139128)
(780, 609960)
(2173, 4726275)
(4551, 20720703)
(12670, 160554240)
(245240, 60143148080) // This one is wrong - s/b ???, 703893960

Numbers a(n) (ie the RHS, which I don't really need) which are the triangular number T(b(n)),
where b(n) is the sequence A006451(n) of numbers n such that T(n) + 1 is a square.
I need b(n)

Final number has 31 digits, so won't fit in 64 bits

I have this working in python but it is too slow to get an answer
#endif
}
int main()
{
	//for (int i = 1; i < 100; ++i)
	//	test01(i);
	test02();
	return 0;
}