using System;
using System.Collections.Generic;

namespace Euler_387
{
    public class Euler_387
    {
        private List<List<long>> m_Harshads;
        private List<long> m_StrongHarshads;

        public void BuildHarshads(int maxDigits)
        {
            m_Harshads = new List<List<long>>();
            List<long> singleDigits = new List<long>();
            for(long i=1; i<=9; ++i)
            {
                singleDigits.Add(i);
            }
            m_Harshads.Add(singleDigits);

            for(int d=2; d<=maxDigits; ++d)
            {
                List<long> harshads = new List<long>();
                int digitsCount = m_Harshads[d - 2].Count;
                for (int i=0; i<digitsCount; ++i)
                {
                    // Try each digit on the end of the previous harshad
                    for(long nd=0; nd<=9; ++nd)
                    {
                        long newNo = m_Harshads[d - 2][i] * 10 + nd;
                        
                        if (newNo % DigitSum(newNo) == 0)
                        {
                            harshads.Add(newNo);
                        }
                    }
                }
                m_Harshads.Add(harshads);
            }
        }

        private long DigitSum(long num)
        {
            long sum = 0;
            do
            {
                sum += num % 10;
                num /= 10;
            }
            while (num  != 0);
            return sum;
        }

        public void FindStrongHarshads()
        {
            m_StrongHarshads = new List<long>();

            for (int i = 0; i < m_Harshads.Count; ++i)
            {
                for (int j = 0; j < m_Harshads[i].Count; ++j)
                {
                    long harshad = m_Harshads[i][j];
                    long digitSum = DigitSum(harshad);
                    if (harshad % digitSum == 0 && IsPrime(harshad / digitSum))
                    {
                        m_StrongHarshads.Add(harshad);
                    }
                }
            }
        }

        public void SumHarshadPrimes(long limit)
        {
            long sum = 0;
            for (int i = 0; i < m_StrongHarshads.Count; ++i)
            {
                // Try each digit on the end of the harshad
                // (it won't end in 0,2,4,6,8)
                for (long nd = 1; nd <= 9; nd+=2)
                {
                    long newNo = m_StrongHarshads[i] * 10 + nd;
                    if (newNo < limit && IsPrime(newNo))
                    {
                        sum += newNo;
                    }
                }   
                
            }

            Console.WriteLine(sum.ToString());
        }

        private bool IsPrime(long n)
        {
            if (n <= 3)
            {
                return n > 1;
            }
            else if (n % 2 == 0 || n % 3 == 0)
            {
                return false;
            }

            long i = 5;
            while (i * i <= n)
            {
                if ((n % i == 0) || (n % (i + 2) == 0))
                {
                    return false;
                }
                i = i + 6;
            }
            return true;
        }
    }
}