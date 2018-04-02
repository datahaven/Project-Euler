#pragma once
#include <vector>

// An unwrapped solved cube looks like this:
//       0  1
//	     2  3
// 4  5  8  9 12 13 16 17
// 6  7 10 11 14 15 18 19 
//      20 21
//      22 23

// Where the faces are:
// Top == 8, 9, 10, 11
// Right == 12, 13, 14, 15
// Front == 20, 21, 22, 23
// Bottom == 16, 17, 18, 19
// Back == 0, 1, 2, 3
// Left == 4, 5, 6, 7
enum class TurnType { Top = 0, Right, Front, Bottom, Back, Left };

//typedef vector<char> CubeLayout;
//typedef char CubeLayoutType[24]; // Would this be faster? I think so
class CubeLayout
{
private:
	char m_CubeLayout[24];
public:
	CubeLayout();
	const bool operator< (const CubeLayout &x) const;
	//const bool operator== (const CubeLayout &x) const;
	const bool operator!= (const CubeLayout &x) const;
	void TurnFace(TurnType turnType, bool turnClockwise = true);
	void RotateCube(const std::vector<int> &rotationMap);
	void DumpFaces() const;
	std::vector<int> GetRotationMap() const;
};
