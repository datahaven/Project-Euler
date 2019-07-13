// Project Euler 387 - Harshad Numbers
// Adrian Dale
// 13/07/2019

// Congratulations, the answer you gave to problem 387 is correct.
//
// You are the 3853rd person to have solved this problem.
//
// This problem had a difficulty rating of 10%. The highest difficulty rating you have solved so far is 100%
using System;

namespace Euler_387
{
    class Program
    {
        static void Main(string[] args)
        {
            Euler_387 e387 = new Euler_387();
            e387.BuildHarshads(13);
            e387.FindStrongHarshads();
            e387.SumHarshadPrimes(100000000000000);
        }
    }
}
