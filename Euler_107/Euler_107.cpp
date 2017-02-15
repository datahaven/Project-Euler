// Project Euler 107
// Minimal Network
// https://projecteuler.net/problem=107
//
// Adrian Dale
// 15/02/2017
//
// Congratulations, the answer you gave to problem 107 is correct.
// You are the 8016th person to have solved this problem.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int network[100][100];

void ReadFile(string name)
{
	memset(&network[0][0], '\0', sizeof(&network[0][0]) * 100 * 100);

	ifstream inputFile(name);
	string fileLine;
	int row = 0;
	while (getline(inputFile, fileLine))
	{
		vector<string> tokens;
		std::size_t start = 0, end = 0;
		while ((end = fileLine.find(",", start)) != std::string::npos) {
			tokens.push_back(fileLine.substr(start, end - start));
			start = end + 1;
		}
		tokens.push_back(fileLine.substr(start));

		for (int col = 0; col < tokens.size(); ++col)
		{
			if (tokens[col] == "-")
				continue;
			int edgeValue = atoi(tokens[col].c_str());
			network[col][row] = edgeValue;
		}
		++row;
	}
}

void FindMST()
{
	int numVertices = 40;
	vector<int> connectedSet(100);
	connectedSet[0] = 1;
	int sum = 0;
	while (true)
	{
		// Go through all connected vertices
		// Find the cheapest one to connect that isn't already connected
		int cheapestConnection = 10000;
		int cheapestConnectionIndex = -1;
		for (int c = 0; c < numVertices; c++)
		{
			if (connectedSet[c] == 1)
			{
				for (int j = 0; j < numVertices; j++)
				{
					if (network[c][j] != 0 && network[c][j] < cheapestConnection && connectedSet[j] == 0)
					{
						cheapestConnection = network[c][j];
						cheapestConnectionIndex = j;
					}
				}
			}
		}
		//cout << "cc=" << cheapestConnection << " cci=" << cheapestConnection << endl;
		if (cheapestConnectionIndex == -1)
		{
			break;
		}
		connectedSet[cheapestConnectionIndex] = 1;
		sum += cheapestConnection;
	}
	cout << "sum=" << sum << endl;

	// Need full network sum
	int fullSum = 0;
	for (int r = 0; r < numVertices; r++)
	{
		for (int c = 0; c < numVertices; c++)
		{
			fullSum += network[c][r];
		}
	}
	cout << "fullSum=" << fullSum << endl;
	cout << "diff=" << (fullSum / 2 - sum) << endl;
}

int main()
{
	ReadFile("p107_network.txt");
	FindMST();
	return 0;
}