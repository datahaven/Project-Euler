// Project_006.cpp 
// Adrian Dale 10/01/2009

/*
The sum of the squares of the first ten natural numbers is,
1^(2) + 2^(2) + ... + 10^(2) = 385

The square of the sum of the first ten natural numbers is,
(1 + 2 + ... + 10)^(2) = 55^(2) = 3025

Hence the difference between the sum of the squares of the first ten natural numbers and the square of the sum is 3025 − 385 = 2640.

Find the difference between the sum of the squares of the first one hundred natural numbers and the square of the sum.
*/

// Answer=25164150
// Very easy! Much easier to write the simple code below than to mess about with
// algebra. Of course it depends whether or not one wants the answer, or a clever (but same!) answer.

#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	int sum_of_squares = 0;
	int square_of_sum = 0;
	for(int i=1; i<=100; ++i)
	{
		sum_of_squares += i*i;
		square_of_sum += i;
	}
	square_of_sum *= square_of_sum;

	cout << "Difference=" << square_of_sum - sum_of_squares << endl;

	return 0;
}

