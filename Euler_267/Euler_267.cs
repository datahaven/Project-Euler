// Project Euler 267 - Billionaire
//
// Adrian Dale 13/07/2019
//
// Congratulations, the answer you gave to problem 267 is correct.
//
// You are the 2898th person to have solved this problem.
//
// This problem had a difficulty rating of 50%. The highest difficulty rating you have solved so far is 100%

using System;

namespace Euler_267
{
    public class Euler_267
    {
        // Store two rows so we can copy from one to the next, but the final calculation only needs m_PascalRows[1, xxx] for N is even
        public double[,] m_PascalRows;

        public void BuildPascalsTriangle(long N)
        {
            // This makes a pascal's triangle and stores the entries for row N
            // ie 1 4 6 4 1 for n == 4
            // 1000 -> 1 1000 499500 166167000 41417124750 8250291250200 1368173298991500 194280608456793000 5668336450989879509
            // This gets way too big for a 64-bit int by the 14th entry, so I've used doubles instead.
            
            m_PascalRows = new double[2,N+2];
            long fromRow = 0;
            long toRow = 1 - fromRow;
            m_PascalRows[fromRow, 0] = 1;
            for(long row=1; row<=N+2; ++row)
            {
                m_PascalRows[toRow, 0] = 1.0f;
                for(long col=1; col<row; ++col)
                {
                    // Each column entry is the sum of the two columns in the row above it
                    m_PascalRows[toRow, col] = m_PascalRows[fromRow, col - 1] + m_PascalRows[fromRow, col];
                }
                // swap to and from rows for next iteration.
                long temp = fromRow;
                fromRow = toRow;
                toRow = temp;
            }
        }

        public void Solve()
        {
            int stepCount = 100000;
            double bestWinCount = 0;
            double bestf = 0.0f;

            // Try stepcount different values of f between 0 and 1
            for (int j = 0; j < stepCount; ++j)
            {
                double f = j / (double)stepCount;

                int N = 1000;
                double winCount = 0;
                // You will always win x times and lose y = N-x times.
                for (int i = 0; i <= N; ++i)
                {
                    // For each of the x, y pairs, calculate the final bankroll
                    double x = N - i;
                    double y = i;
                    double finalBankRoll = Math.Pow(2.0f * f + 1.0f, x) * Math.Pow(1.0f - f, y);
                    if (finalBankRoll >= 1000000000.0f)
                    {
                        // Each x,y pair can happen the binomial coefficient number of times
                        winCount += m_PascalRows[1, i];
                    }
                }
                
                if (winCount >= bestWinCount)
                {
                    bestWinCount = winCount;
                    bestf = f;
                }
            }
            // Seems like this can happen for a wide range of different values for f
            // 0.12952 <= f <= 0.16656
            double answer = bestWinCount / Math.Pow(2.0f, 1000.0f);
            Console.WriteLine("Answer=" + answer.ToString());
        }
    }
}