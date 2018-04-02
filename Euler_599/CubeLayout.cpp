#include "CubeLayout.h"
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

const vector<vector<int> > FaceTurns = {
	// {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23}, // Identity, do nothing
	{ 0, 1, 7, 5, 4, 20, 6, 21, 10, 8, 11, 9, 2, 13, 3, 15, 16, 17, 18, 19, 14, 12, 22, 23 }, // T+
	{ 0, 9, 2, 11, 4, 5, 6, 7, 8, 21, 10, 23, 14, 12, 15, 13, 3, 17, 1, 19, 20, 18, 22, 16 }, // R+
	{ 0, 1, 2, 3, 4, 5, 18, 19, 8, 9, 6, 7, 12, 13, 10, 11, 16, 17, 14, 15, 22, 20, 23, 21 }, // F+
	{ 13, 15, 2, 3, 1, 5, 0, 7, 8, 9, 10, 11, 12, 23, 14, 22, 18, 16, 19, 17, 20, 21, 4, 6 }, // Bottom+
	{ 2, 0, 3, 1, 8, 9, 6, 7, 12, 13, 10, 11, 16, 17, 14, 15, 4, 5, 18, 19, 20, 21, 22, 23 }, // Back+
	{ 19, 1, 17, 3, 6, 4, 7, 5, 0, 9, 2, 11, 12, 13, 14, 15, 16, 22, 18, 20, 8, 21, 10, 23 } // Left
};

CubeLayout::CubeLayout()
{
	// Initialise to the identity layout
	for (int i = 0; i < 24; ++i)
	{
		m_CubeLayout[i] = i;
	}
}

const bool CubeLayout::operator< (const CubeLayout &x) const
{
	return lexicographical_compare(&m_CubeLayout[0], &m_CubeLayout[24], &x.m_CubeLayout[0], &x.m_CubeLayout[24]);
}

const bool CubeLayout::operator!= (const CubeLayout &x) const
{
	for (int i = 0; i < 24; i++)
	{
		if (x.m_CubeLayout[i] != m_CubeLayout[i])
			return true;
	}
	return false;
}

void CubeLayout::TurnFace(TurnType turnType, bool turnClockwise)
{
	// An anti-clockwise turn is just three clockwise turns
	int turnCount = turnClockwise ? 1 : 3;
	for (int tc = 0; tc < turnCount; ++tc)
	{
		// TODO - There's probably a faster way to do this using swapping, instead of having
		// to make a whole new face array
		char newCube[24];
		for (int i = 0; i < 24; ++i)
		{
			newCube[i] = m_CubeLayout[FaceTurns[static_cast<int>(turnType)][i]];
		}
		for (int i = 0; i < 24; ++i)
		{
			m_CubeLayout[i] = newCube[i];
		}
	}
}

void CubeLayout::RotateCube(const vector<int> &rotationMap)
{
	char newCube[24];
	for (int i = 0; i < 24; ++i)
	{
		newCube[i] = m_CubeLayout[rotationMap[i]];
	}
	for (int i = 0; i < 24; ++i)
	{
		m_CubeLayout[i] = newCube[i];
	}
}

void CubeLayout::DumpFaces() const
{
	for (int i = 0; i < 24; ++i)
	{
		cout << static_cast<int>(m_CubeLayout[i]) << " ";
	}
	cout << endl;
}

vector<int> CubeLayout::GetRotationMap() const
{
	// A bit of a hack to expose the cube data for when I'm
	// building rotation maps.
	// I'm sure there's a way I could just do without rotation maps
	// and rotate the cube directly
	vector<int> rotationData;
	for (int i = 0; i < 24; ++i)
	{
		rotationData.push_back(m_CubeLayout[i]);
	}
	return rotationData;
}