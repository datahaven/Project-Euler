// Project Euler 345
// Matrix Sum
// Adrian Dale
// 21/11/2017

// Congratulations, the answer you gave to problem 345 is correct.
// You are the 3802nd person to have solved this problem.

// NB I used the brute force version of the code to get the correct answer.
// It runs in just under 5 hours.
//
// The recursive version of the code calculates the max sum for each of the 
// four corner n-1 sub matrices, then picks the one that can be improved to
// the highest sum with a number from the row/column outside the sub-matrix.
//
// This gives the correct answer for the problem but is actually incorrect
// for smaller top left corners of the test data.
// I suspect this is because some sub-matrices have the same max sum,
// but using different columns.
//
// There are far better solutions than this on the forum.

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>

using namespace std;

typedef vector< vector<int> > Matrix;

bool gDebugPrint = false;

Matrix gSmallInput = {
	{7, 53, 183, 439, 863},
	{ 497, 383, 563,  79, 973 },
	{ 287,  63, 343, 169, 583 },
	{ 627, 343, 773, 959, 943 },
	{ 767, 473, 103, 699, 303 }
};


Matrix gFullSize = {
	{ 7,53,183,439,863,497,383,563,79,973,287,63,343,169,583 },
	{ 627,343,773,959,943,767,473,103,699,303,957,703,583,639,913 },
	{ 447,283,463,29,23,487,463,993,119,883,327,493,423,159,743 },
	{ 217,623,3,399,853,407,103,983,89,463,290,516,212,462,350 },
	{ 960,376,682,962,300,780,486,502,912,800,250,346,172,812,350 },
	{ 870,456,192,162,593,473,915,45,989,873,823,965,425,329,803 },
	{ 973,965,905,919,133,673,665,235,509,613,673,815,165,992,326 },
	{ 322,148,972,962,286,255,941,541,265,323,925,281,601,95,973 },
	{ 445,721,11,525,473,65,511,164,138,672,18,428,154,448,848 },
	{ 414,456,310,312,798,104,566,520,302,248,694,976,430,392,198 },
	{ 184,829,373,181,631,101,969,613,840,740,778,458,284,760,390 },
	{ 821,461,843,513,17,901,711,993,293,157,274,94,192,156,574 },
	{ 34,124,4,878,450,476,712,914,838,669,875,299,823,329,699 },
	{ 815,559,813,459,522,788,168,586,966,232,308,833,251,631,107 },
	{ 813,883,451,509,615,77,281,613,459,205,380,274,302,35,805 }
};

void DumpMatrix(Matrix &m, int rs, int cs, int ms)
{
	for (int r = rs; r < rs + ms; ++r)
	{
		for (int c = cs; c < cs + ms; ++c)
		{
			cout << setw(3) << m[r][c] << " ";
		}
		cout << endl;
	}
}
struct Solution
{
	int m_MaxSum;
	vector<int> m_Cols;
};

Solution SmallTest(int startRow, int startCol, int matSize)
{
	cout << "SmallTest(" << startRow << ", " << startCol << ", " << matSize << ")" << endl;
	int maxSum = 0;
	vector<int> colPerm(matSize);
	iota(colPerm.begin(), colPerm.end(), startCol);
	
	vector<int> bestSoFar;
	do
	{
		int sum = 0;
		for (int row = 0; row < matSize; ++row)
		{
			sum += gFullSize[row+startRow][colPerm[row]];
		}
		if (sum > maxSum)
		{
			maxSum = sum;
			bestSoFar = colPerm;
		}
		
	} while (next_permutation(colPerm.begin(), colPerm.end()));
	
	if (gDebugPrint)
	{
		cout << "maxSum=" << maxSum << endl;
		for (auto i : bestSoFar)
		{
			cout << i << " ";
		}
		cout << endl;
	}
	Solution s;
	s.m_Cols = bestSoFar;
	s.m_MaxSum = maxSum;
	return s;
}

Solution ImproveTL(Matrix &matrix, int rowStart, int colStart, int matSize, Solution s)
{
	// matSize is size of the matrix without the improved row
	Solution improvedSolution = s;
	
	if (gDebugPrint)
	{
		cout << "ImproveTL(" << rowStart << "," << colStart << "," << matSize << ")" << endl;
		cout << " bestSol in == " << s.m_MaxSum << " : ";
		for (auto col : s.m_Cols)
		{
			cout << col << " ";
		}
		cout << endl;
		DumpMatrix(matrix, rowStart, colStart, matSize);
		//cout << endl;
	}
	int bestColSwapIndex = matSize;
	int bestMatrixSum = improvedSolution.m_MaxSum + matrix[rowStart+matSize][colStart+matSize];
	
	for (int i=0; i<matSize; ++i)
	{
		int p1 = matrix[rowStart + i][s.m_Cols[i]];
		int p2 = matrix[rowStart + matSize][s.m_Cols[i]];
		int p3 = matrix[rowStart + i][colStart + matSize];
		int matSum = s.m_MaxSum - p1 + p2 + p3;
		
		if (matSum > bestMatrixSum)
		{
			bestMatrixSum = matSum;
			bestColSwapIndex = i;
		}
	}
	improvedSolution.m_MaxSum = bestMatrixSum;

	improvedSolution.m_Cols.push_back(colStart + matSize);		
	swap(improvedSolution.m_Cols[bestColSwapIndex], improvedSolution.m_Cols[matSize]);

	if (gDebugPrint)
	{
		cout << " == " << improvedSolution.m_MaxSum << " : ";
		int sum = 0;
		int i = 0;
		for (auto col : improvedSolution.m_Cols)
		{
			sum += gFullSize[rowStart + i++][col];
			cout << col << " ";
		}
		cout << endl;
		if (sum != improvedSolution.m_MaxSum)
		{
			cout << "ERROR: Sum mismatch in TL" << endl;
			exit(0);
		}
	}

	return improvedSolution;
}

Solution ImproveTR(Matrix &matrix, int rowStart, int colStart, int matSize, Solution s)
{
	// matSize is size of the matrix without the improved row
	Solution improvedSolution = s;

	if (gDebugPrint)
	{
		cout << "ImproveTR(" << rowStart << "," << colStart << "," << matSize << ")" << endl;
		cout << " bestSol in == " << s.m_MaxSum << " : ";
		for (auto col : s.m_Cols)
		{
			cout << col << " ";
		}
		cout << endl;
		DumpMatrix(matrix, rowStart, colStart, matSize);
		//cout << endl;
	}
	int bestColSwapIndex = matSize;
	int bestMatrixSum = improvedSolution.m_MaxSum + matrix[rowStart + matSize][colStart - 1];

	for (int i = 0; i<matSize; ++i)
	{
		int p1 = matrix[rowStart + i][s.m_Cols[i]];
		int p2 = matrix[rowStart + matSize][s.m_Cols[i]];
		int p3 = matrix[rowStart + i][colStart - 1];
		int matSum = s.m_MaxSum - p1 + p2 + p3;

		if (matSum > bestMatrixSum)
		{
			bestMatrixSum = matSum;
			bestColSwapIndex = i;
		}
	}
	improvedSolution.m_MaxSum = bestMatrixSum;

	improvedSolution.m_Cols.push_back(colStart  - 1);
	swap(improvedSolution.m_Cols[bestColSwapIndex], improvedSolution.m_Cols[matSize]);

	if (gDebugPrint)
	{
		cout << " == " << improvedSolution.m_MaxSum << " : ";
		int sum = 0;
		int i = 0;
		for (auto col : improvedSolution.m_Cols)
		{
			sum += gFullSize[rowStart + i++][col];
			cout << col << " ";
		}
		cout << endl;
		if (sum != improvedSolution.m_MaxSum)
		{
			cout << "ERROR: Sum mismatch in TR" << endl;
			exit(0);
		}
	}

	return improvedSolution;
}

Solution ImproveBL(Matrix &matrix, int rowStart, int colStart, int matSize, Solution s)
{
	// matSize is size of the matrix without the improved row
	Solution improvedSolution = s;

	if (gDebugPrint)
	{
		cout << "ImproveBL(" << rowStart << "," << colStart << "," << matSize << ")" << endl;
		cout << " bestSol in == " << s.m_MaxSum << " : ";
		for (auto col : s.m_Cols)
		{
			cout << col << " ";
		}
		cout << endl;
		DumpMatrix(matrix, rowStart, colStart, matSize);
		//cout << endl;
	}
	int bestColSwapIndex = 0;
	int bestMatrixSum = improvedSolution.m_MaxSum + matrix[rowStart - 1][colStart + matSize];

	for (int i = 0; i<matSize; ++i)
	{
		int p1 = matrix[rowStart + i][s.m_Cols[i]];
		int p2 = matrix[rowStart - 1][s.m_Cols[i]];
		int p3 = matrix[rowStart + i][colStart+matSize];
		int matSum = s.m_MaxSum - p1 + p2 + p3;

		if (matSum > bestMatrixSum)
		{
			bestMatrixSum = matSum;
			bestColSwapIndex = i+1;
		}
	}
	improvedSolution.m_MaxSum = bestMatrixSum;

	
	improvedSolution.m_Cols.insert(improvedSolution.m_Cols.begin(), colStart +matSize);
	swap(improvedSolution.m_Cols[bestColSwapIndex], improvedSolution.m_Cols[0]);
	if (gDebugPrint)
	{
		cout << " == " << improvedSolution.m_MaxSum << " : ";
		int sum = 0;
		int i = 0;
		for (auto col : improvedSolution.m_Cols)
		{
			sum += gFullSize[rowStart-1 + i++][col];
			cout << col << " ";
		}
		cout << endl;
		if (sum != improvedSolution.m_MaxSum)
		{
			cout << "ERROR: Sum mismatch in BL: " << (sum - improvedSolution.m_MaxSum) <<endl;
			exit(0);
		}
	}

	return improvedSolution;
}

Solution ImproveBR(Matrix &matrix, int rowStart, int colStart, int matSize, Solution s)
{
	// matSize is size of the matrix without the improved row
	Solution improvedSolution = s;

	if (gDebugPrint)
	{
		cout << "ImproveBR(" << rowStart << "," << colStart << "," << matSize << ")" << endl;
		cout << " bestSol in == " << s.m_MaxSum << " : ";
		for (auto col : s.m_Cols)
		{
			cout << col << " ";
		}
		cout << endl;
		DumpMatrix(matrix, rowStart, colStart, matSize);
		//cout << endl;
	}
	int bestColSwapIndex = 0;
	int bestMatrixSum = improvedSolution.m_MaxSum + matrix[rowStart - 1][colStart - 1];

	for (int i = 0; i<matSize; ++i)
	{
		int p1 = matrix[rowStart + i][s.m_Cols[i]];
		int p2 = matrix[rowStart - 1][s.m_Cols[i]];
		int p3 = matrix[rowStart + i][colStart - 1];
		int matSum = s.m_MaxSum - p1 + p2 + p3;
			 

		if (matSum > bestMatrixSum)
		{
			bestMatrixSum = matSum;
			bestColSwapIndex = i+1;
		}
	}
	improvedSolution.m_MaxSum = bestMatrixSum;

	improvedSolution.m_Cols.insert(improvedSolution.m_Cols.begin(), colStart - 1);
	swap(improvedSolution.m_Cols[bestColSwapIndex], improvedSolution.m_Cols[0]);

	if (gDebugPrint)
	{
		cout << " == " << improvedSolution.m_MaxSum << " : ";
		int sum = 0;
		int i = 0;
		for (auto col : improvedSolution.m_Cols)
		{
			sum += gFullSize[rowStart - 1 + i++][col];
			cout << col << " ";
		}
		cout << endl;
		if (sum != improvedSolution.m_MaxSum)
		{
			cout << "ERROR: Sum mismatch in BR" << endl;
			exit(0);
		}
	}

	return improvedSolution;
}

Solution Solve(Matrix &matrix, int rowStart, int colStart, int matSize)
{
	if (gDebugPrint)
	{
		cout << "Solve(" << rowStart << "," << colStart << "," << matSize << ")" << endl;
	}
	Solution sol;
	sol.m_MaxSum = 0;
	sol.m_Cols.clear();

	if (matSize == 2)
	{
		if (matrix[rowStart][colStart] + matrix[rowStart + 1][colStart + 1] >
			matrix[rowStart + 1][colStart] + matrix[rowStart][colStart + 1])
		{
			sol.m_MaxSum = matrix[rowStart][colStart] + matrix[rowStart + 1][colStart + 1];
			sol.m_Cols.push_back(colStart);
			sol.m_Cols.push_back(colStart+1);
		}
		else
		{
			sol.m_MaxSum = matrix[rowStart+1][colStart] + matrix[rowStart][colStart + 1];
			sol.m_Cols.push_back(colStart+1);
			sol.m_Cols.push_back(colStart);
		}

		return sol;
	}

	// Solve can't return int.
	// It needs to return the best value AND the cols we chose to get there
	Solution TL_TR_BL_BR[4];
	TL_TR_BL_BR[0] = Solve(matrix, rowStart, colStart, matSize - 1);
	TL_TR_BL_BR[1] = Solve(matrix, rowStart, colStart + 1, matSize - 1);
	TL_TR_BL_BR[2] = Solve(matrix, rowStart + 1, colStart, matSize - 1);
	TL_TR_BL_BR[3] = Solve(matrix, rowStart + 1, colStart + 1, matSize - 1);

	
	// Improve(tl, rowStart, colStart, matSize)
	// etc for tr, bl, br
	TL_TR_BL_BR[0] = ImproveTL(matrix, rowStart, colStart, matSize - 1,     TL_TR_BL_BR[0]);
	TL_TR_BL_BR[1] = ImproveTR(matrix, rowStart, colStart + 1, matSize - 1,     TL_TR_BL_BR[1]);
	TL_TR_BL_BR[2] = ImproveBL(matrix, rowStart + 1, colStart, matSize - 1,     TL_TR_BL_BR[2]);
	TL_TR_BL_BR[3] = ImproveBR(matrix, rowStart + 1, colStart + 1, matSize - 1,  TL_TR_BL_BR[3]);
	//cout << TL_TR_BL_BR[0].m_MaxSum << endl;
	//cout << TL_TR_BL_BR[1].m_MaxSum << endl;
	//cout << TL_TR_BL_BR[2].m_MaxSum << endl;
	//cout << TL_TR_BL_BR[3].m_MaxSum << endl;

	// Return largest of the four improved solutions
	int bestIndex = 0;
	int bestSum = -1;
	for (int i = 0; i < 4; ++i)
	{
		if (TL_TR_BL_BR[i].m_MaxSum > bestSum)
		{
			bestSum = TL_TR_BL_BR[i].m_MaxSum;
			bestIndex = i;
		}
	}

	if (gDebugPrint)
	{
		cout << "Solve(" << rowStart << "," << colStart << "," << matSize << ") returned " << TL_TR_BL_BR[bestIndex].m_MaxSum << " : ";
		for (auto col : TL_TR_BL_BR[bestIndex].m_Cols)
		{
			cout << col << " ";
		}
		cout << endl;
	}

	return TL_TR_BL_BR[bestIndex];
}


void Test01()
{
	for (int matSize = 2; matSize < 15; ++matSize)
	{
		for (int startCol = 0; startCol < 15 - matSize; ++startCol)
		{
			for (int startRow = 0; startRow < 15 - matSize; ++startRow)
			{
				Solution sol1 = Solve(gFullSize, startRow, startCol, matSize);
				Solution sol2 = SmallTest(startRow, startCol, matSize);
				if (sol1.m_MaxSum != sol2.m_MaxSum || (sol1.m_Cols != sol2.m_Cols && sol1.m_Cols.size() > 2))
				{
					cout << "ERROR:" << startRow << "," << startCol << "," << matSize << " : " << sol1.m_MaxSum << "," << sol2.m_MaxSum << endl;
					DumpMatrix(gFullSize, startRow, startCol, matSize);
					for (auto col : sol1.m_Cols)
					{
						cout << col << " ";
					}
					cout << endl;
					for (auto col : sol2.m_Cols)
					{
						cout << col << " ";
					}
					cout << endl;
					exit(0);
				}
				
			}
		}
	}
}

int main()
{
	//Test01();
	//return 0;
	int ms = 6;
	int rs = 0;
	int cs = 0;
	DumpMatrix(gFullSize, rs, cs, ms);
	SmallTest(rs + 0, cs + 0, ms - 1);
	SmallTest(rs + 0, cs + 1, ms - 1);
	SmallTest(rs + 1, cs + 0, ms - 1);
	SmallTest(rs + 1, cs + 1, ms - 1);
	SmallTest(rs + 0, cs + 0, ms);
	//return 0;
	
	
	cout << "---" << endl;
	for (ms = 5; ms <= 15; ++ms)
	{
		Solution sol = Solve(gFullSize, rs + 0, cs + 0, ms);
		cout << "Result=" << sol.m_MaxSum << endl;
		int sum = 0;
		int i = 0;
		for (auto col : sol.m_Cols)
		{
			sum += gFullSize[rs + i++][col];
			cout << col << " ";
		}
		cout << endl;
		//cout << "sum=" << sum << endl;

		Solution sol2 = SmallTest(rs, cs, ms);
		cout << "Check Result=" << sol2.m_MaxSum << endl;
		sum = 0;
		i = 0;
		for (auto col : sol2.m_Cols)
		{
			sum += gFullSize[rs + i++][col];
			cout << col << " ";
		}
		cout << endl;
		cout << "---" << endl;
	}
	return 0;
}