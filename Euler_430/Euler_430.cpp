// Project Euler 430
// Range Flips
// Adrian Dale
// 04/03/2018

// Congratulations, the answer you gave to problem 430 is correct.
//
// You are the 635th person to have solved this problem.
//
// This problem had a difficulty rating of 35 % .The highest difficulty rating you have solved so far is 100 %

// Runs in 0.495 seconds

#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>
using namespace std;
using namespace std::chrono;

const double epsilon = 0.000000001;

inline double ProbabilityNotFlipped(double disk, double numDisks)
{
	double lessPart = (numDisks - disk) / numDisks;
	double morePart = (disk - 1.0) / numDisks;
	return lessPart*lessPart + morePart * morePart;
}

inline double ExpectedFlippedInPosX(double disk, double numDisks, double numTurns)
{
	double powerPart = (1.0 - 2.0 * ProbabilityNotFlipped(disk, numDisks));
	return (1.0 - pow(powerPart, numTurns)) / 2.0;
}

double ExpectedTotal(double numDisks, double numTurns)
{
	double total = 0.0;
	double numDisksD = numDisks / 2.0;
	double previousEstimate = 0.0f;
	for (double x = 1.0; x <= numDisksD; ++x)
	{
		double efipx = ExpectedFlippedInPosX(x, numDisks, numTurns);
		total += efipx;

		if (0.5 - efipx < epsilon)
		{
			double estimatedFinalTotal = total + (numDisksD - x)*0.5;
			if (fabs(estimatedFinalTotal - previousEstimate) < epsilon)
			{
				return 2.0 * ((numDisks / 2.0) - estimatedFinalTotal);
			}
			previousEstimate = estimatedFinalTotal;
		}
	}
	return 2.0 * ((numDisks / 2.0) - total);
}

int main()
{
	high_resolution_clock::time_point startTime = high_resolution_clock::now();
	cout << "Answer = " << setprecision(15) << ExpectedTotal(10000000000.0, 4000.0) << endl;
	high_resolution_clock::time_point endTime = high_resolution_clock::now();
	duration<double> timeSpan = duration_cast<duration<double>>(endTime - startTime);
	cout << "Time: " << timeSpan.count() << " seconds" << endl;
	return 0;
}