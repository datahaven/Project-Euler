// Project Euler 118
// Pandigital prime sets
//
// Adrian Dale
// 02/04/2018

// Congratulations, the answer you gave to problem 118 is correct.
// You are the 5434th person to have solved this problem.
// This problem had a difficulty rating of 45 % .

// Generate all partitions of the number 9 to get a set of the different
// digit lengths the primes in a set can have

// Then read in the pre-prepared primes list and store the primes indexed 
// by digit count. (And sorted)

// For each partition recursively generate all the sets of primes of the correct
// digit lengths where there aren't any duplicate digits.

// Takes one or two seconds to run.
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

vector<vector<int>> Partitions;

vector<vector<int>> Primes;

void MakePartitions(vector<int> currentPartition)
{
	int partitionSum = accumulate(currentPartition.begin(), currentPartition.end(), 0);
	if (partitionSum == 9)
	{
		// Slightly faster, if messier, to do this count here, so we save on some recursion.
		// Not that this takes long, anyway
		int onesCount = count(currentPartition.begin(), currentPartition.end(), 1);
		if (onesCount <= 4)
		{
			// Only include partitions with less than four 1's, as there are only four
			// single digit prime numbers
			Partitions.push_back(currentPartition);
		}
	}
	else if (partitionSum < 9)
	{
		int startNum = 1;
		if (currentPartition.empty() == false)
		{
			startNum = currentPartition.back();
		}
		for (int i = startNum; i < 10; ++i)
		{
			vector<int> nextPartition = currentPartition;
			nextPartition.push_back(i);
			MakePartitions(nextPartition);
		}
	}
}

void PopulatePartitions()
{
	Partitions.clear();
	vector<int> emptyPartition;
	MakePartitions(emptyPartition);
}

// Quickly hacked in from stack overflow
int NumDigits(int number)
{
	int digits = 0;
	if (number < 0) digits = 1; // remove this line if '-' counts as a digit
	while (number) {
		number /= 10;
		digits++;
	}
	return digits;
}

void ReadPrimes()
{
	// Fill Primes with vector<int> for each different digit length of primes
	Primes.clear();
	for (int i = 0; i < 10; ++i)
	{
		vector<int> primes;
		Primes.push_back(primes);
	}

	ifstream inputFile("C:\\Users\\Adrian\\Documents\\Visual Studio 2017\\Projects\\Euler_118\\Primes_Formatted.txt");
	int prime;
	while (inputFile >> prime)
	{
		int digitCount = NumDigits(prime);
		Primes[digitCount].push_back(prime);
		//cout << digitCount << " " << prime << " " << endl;
	}

	//for (int i = 0; i < 10; ++i)
	//{
	//	cout << "There are " << Primes[i].size() << " " << i << " digit length primes" << endl;
	//}
}

bool IsValidSet(vector<int> set)
{
	vector<int> digitCounts(10);
	for (int digits : set)
	{
		while (digits) {
			++digitCounts[digits % 10];
			digits /= 10;	
		}
	}
	return count_if(digitCounts.begin(), digitCounts.end(), [&](int digitCount) {return digitCount > 1; }) == 0;
}

void MakeSets(vector<vector<int>> &allSets, vector<int> currentSet, vector<int> remainingPartition)
{
	if (remainingPartition.empty())
	{
		//for (const auto num : currentSet)
		//{
		//	cout << num << ",";
		//}
		//cout << endl;
		allSets.push_back(currentSet);
		return;
	}
	int nextDigitCount = remainingPartition.front();
	remainingPartition.erase(remainingPartition.begin());
	int startDigit = 0;
	if (currentSet.empty() == false)
	{
		startDigit = currentSet.back();
	}
	for (int i = 0; i < Primes[nextDigitCount].size(); ++i)
	{
		int nextPrime = Primes[nextDigitCount][i];
		if (nextPrime > startDigit)
		{
			vector<int> nextSet = currentSet;
			nextSet.push_back(nextPrime);
			if (IsValidSet(nextSet))
			{
				MakeSets(allSets, nextSet, remainingPartition);
			}
		}
	}
}

int CountSets(vector<int> partition)
{
	int result = 0;
	vector<vector<int>> allSets;
	vector<int> currentSet;
	MakeSets(allSets, currentSet, partition);
	return allSets.size();
}

int CountAllSets()
{
	int result = 0;
	for (const auto &partition : Partitions)
	{
		result += CountSets(partition);
	}
	return result;
}

int main()
{
	PopulatePartitions();
	
	ReadPrimes();

	cout << "Answer = " << CountAllSets() << endl;

	return 0;
}