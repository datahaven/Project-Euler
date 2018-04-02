// Project Euler 599
// Distinct Colourings of a Rubik's Cube
// Adrian Dale
// 01/04/2018

// Congratulations, the answer you gave to problem 599 is correct.
// You are the 203rd person to have solved this problem.

// Uses Burnside's Lemma to count number of possible colourings
// Takes way longer than one minute to run, though.
// Almost fifteen mins - could definitely be improved!
// Needs to be build on x64 to be able to access enough memory.

// Needs the extra python script to calculate the final result as it
// is too big to calculate using 64-bit ints
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <chrono>

#include "CubeLayout.h"
using namespace std;
using namespace std::chrono;

set<CubeLayout> CubePermutations;

// Final solution doesn't use FullCubeRotations, or GetCanonical,
// but I've kept them in in case improved versions of this code use them.
vector<vector<int>> FullCubeRotations;

CubeLayout GetCanonical(CubeLayout cube)
{
	vector<CubeLayout> allRotations;
	allRotations.reserve(24);
	for (const auto &rotationMap : FullCubeRotations)
	{
		CubeLayout rotatedCube = cube;
		rotatedCube.RotateCube(rotationMap);
		allRotations.push_back(rotatedCube);
	}
	sort(allRotations.begin(), allRotations.end());
	return allRotations[0];
}

void AddAllPermutations(CubeLayout cube)
{
	// I thought a std::queue would be faster than a list, but
	// it didn't seem to make much difference. Maybe even slower.
	queue<CubeLayout> queue;
	
	queue.push(cube);
	CubePermutations.insert(cube);
	while (!queue.empty())
	{
		CubeLayout testCube = queue.front();
		queue.pop();
		for (int t = 0; t < 6; ++t)
		{
			CubeLayout turnedCube = testCube;
			turnedCube.TurnFace(static_cast<TurnType>(t));

			if (CubePermutations.find(turnedCube) == CubePermutations.end())
			{
				queue.push(turnedCube);
				CubePermutations.insert(turnedCube);
			}
		}
	}
}

void BuildCubePermutations()
{
	// Build CubePermutations vector of all possible permutations of the cube using the
	// moves in FaceTurns. Keep trying all moves recursively until we can't add any more
	CubePermutations.clear();
	CubeLayout startingCube;

	AddAllPermutations(startingCube);
	// Should be 3674160 permutations x 24 == 88179840
	// TODO - Knowing this, I'm sure there's a way to instantly make these perms, rather than
	// having to simulate cube turns and store them all.
	// Obviously this would be a lot faster
	cout << "Found " << CubePermutations.size() << " permutations" << endl;
}

void BuildFullCubeRotations()
{
	FullCubeRotations.clear();
	CubeLayout identityRotationCube;
	// There are 24 different orientations of the cube.
	// I don't want to specify all of these by hand, but can work them out by rotating opposite faces,
	// which equates to turning the entire cube over in my hand
	// identity is 8,9,10,11 on top
	vector<CubeLayout> faceUpCubes(6, identityRotationCube);
	faceUpCubes[1].TurnFace(TurnType::Right);       // Put F on top
	faceUpCubes[1].TurnFace(TurnType::Left, false);
	faceUpCubes[2].TurnFace(TurnType::Right, false); // Put Back on top
	faceUpCubes[2].TurnFace(TurnType::Left);
	faceUpCubes[3].TurnFace(TurnType::Front, false); // Put Right on top
	faceUpCubes[3].TurnFace(TurnType::Back);
	faceUpCubes[4].TurnFace(TurnType::Front); // Put Left on top
	faceUpCubes[4].TurnFace(TurnType::Back, false);
	faceUpCubes[5].TurnFace(TurnType::Front); // Put Bottom on top
	faceUpCubes[5].TurnFace(TurnType::Front);
	faceUpCubes[5].TurnFace(TurnType::Back);
	faceUpCubes[5].TurnFace(TurnType::Back);

	for (CubeLayout cube : faceUpCubes)
	{
		FullCubeRotations.push_back(cube.GetRotationMap());
		// For each face-up cube add all three rotations around z-axis
		for (int i = 0; i < 3; ++i)
		{
			cube.TurnFace(TurnType::Top);
			cube.TurnFace(TurnType::Bottom, false);
			FullCubeRotations.push_back(cube.GetRotationMap());
		}
	}

	// Don't really need this sort
	sort(FullCubeRotations.begin(), FullCubeRotations.end());
}

unsigned long long Solve(int N)
{
	map<int, int> cycleCounts;

	unsigned long long result = 0;
	for (const auto &permutationFromSet : CubePermutations)
	{
		// Remove constness from perm being in a set
		vector<int> permutation = permutationFromSet.GetRotationMap();
		int cycleCount = 0;
		int permPos = 0;
		while (permPos < 24)
		{
			int nextCyclePos = permPos;
			int cyclePos = nextCyclePos;
			while ((nextCyclePos = permutation[nextCyclePos]) != permPos)
			{
				permutation[cyclePos] = -1;
				cyclePos = nextCyclePos;
			}
			permutation[cyclePos] = -1;
			permutation[nextCyclePos] = -1; // s/b nextCyclePos == permPos anyway
			++cycleCount;
			++permPos;
			// Move to next permPos, that isn't marked as seen
			while (permutation[permPos] == -1 && permPos < 24)
			{
				++permPos;
			}			
		}
		result += static_cast<unsigned long long>(pow(N, cycleCount)); // NB to power of!

		if (cycleCounts.find(cycleCount) == cycleCounts.end())
		{
			cycleCounts[cycleCount] = 1;
		}
		else
		{
			cycleCounts[cycleCount] = cycleCounts[cycleCount] + 1;
		}
	}

	for (const auto & c : cycleCounts)
	{
		cout << c.first << "," << c.second << endl;
	}

	return result / CubePermutations.size();
}

void test01()
{
	// Do four of each turn type, and make sure cube comes back to starting position.
	// Just to make sure I typed the FaceTurns map out properly.
	// This test isn't exhaustive, so if I have any bother, I'll add some more.
	CubeLayout startingCube;
	CubeLayout testCube = startingCube;
	for (int t = 0; t < 6; t++)
	{
		for (int i = 0; i < 4; ++i)
		{
			testCube.TurnFace(static_cast<TurnType>(t));
		}
		if (startingCube != testCube)
		{
			cout << "ERROR in FaceTurns test: " << t << endl;
			testCube.DumpFaces();
		}
	}
}

void test02()
{
	CubeLayout startingCube;
	CubeLayout testCube = startingCube;
	for (int i = 0; i < 15; i++)
	{
		testCube.TurnFace(TurnType::Top);
		testCube.TurnFace(TurnType::Right);
	}
	for (int i = 0; i < 15; i++)
	{
		testCube.TurnFace(TurnType::Top);
		testCube.TurnFace(TurnType::Left);
	}
	for (int i = 0; i < 15; i++)
	{
		testCube.TurnFace(TurnType::Bottom);
		testCube.TurnFace(TurnType::Front);
	}
	if (startingCube != testCube)
	{
		cout << "ERROR in test02" << endl;
	}
}

void test03()
{
	CubePermutations.clear();
	CubeLayout cube;
	CubePermutations.insert(cube);
	cube.TurnFace(TurnType::Top);
	CubePermutations.insert(cube);
	cube.TurnFace(TurnType::Top);
	CubePermutations.insert(cube);

	cube.DumpFaces();

	cube.TurnFace(TurnType::Top);
	CubePermutations.insert(cube);

	Solve(2);
}

int main()
{
	//test01();
	//test02();
	//test03();
	
	BuildFullCubeRotations();

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	BuildCubePermutations();
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
	cout << "BuildCubePermutations() ran in " << time_span.count() << " seconds" << endl;
	
	// NB Need to paste the cycle counts into separate python script to get the
	// correct answer. The one below will have overflowed and will be wrong.
	cout << "Answer(10)=" << Solve(10) << endl;

	high_resolution_clock::time_point t3 = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(t3 - t1);
	cout << "Total Run Time: " << time_span.count() << " seconds" << endl;
	return 0;
}