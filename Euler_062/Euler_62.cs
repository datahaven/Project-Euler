// Project Euler 62
// Cubic Permutations
// Adrian Dale
// 08/05/2019
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Euler_62
{
    class Program
    {
        static void Main(string[] args)
        {
            Dictionary < string, List < ulong>> numberCount = new Dictionary<string, List<ulong>>();

            // Roughly the cube root of 2^64 which is as big as our integer can be.
            // Hopefully we'll have the answer before then - and we do! :-)
            for(ulong i=0; i<2642245; ++i)
            {
                ulong i3 = i * i * i;
                string cubeAsStr = i3.ToString();
                char[] sortedStrArray = cubeAsStr.ToCharArray();
                Array.Sort(sortedStrArray);
                string sortedStr = new string(sortedStrArray);
                if (numberCount.ContainsKey(sortedStr))
                {
                    numberCount[sortedStr].Add(i3);
                }
                else
                {
                    numberCount[sortedStr] = new List<ulong> { i3 };
                }
            }

            foreach(KeyValuePair<string, List<ulong>> kvp in numberCount)
            {
                if (kvp.Value.Count == 5)
                {
                    Console.WriteLine(String.Format("{0}", kvp.Value[0]));
                    break;
                }
            }
            
        }
    }
}
