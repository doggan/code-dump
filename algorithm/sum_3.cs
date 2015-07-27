using System;
using System.Linq;

namespace Hoge
{
    class MainClass
    {
        /*
            Sum 3 values to zero.
         */
        static int[] findSum(int[] list)
        {
            Array.Sort(list);
            
            for (int i = 0; i < list.Length - 2; i++) {
                int l = i + 1;
                int r = list.Length - 1;
                while (l < r) {
                    int sum = list[i] + list[l] + list[r];
                    if (sum < 0) {
                        l++;
                    }
                    else if (sum > 0) {
                        r--;
                    }
                    else {
                        return new int[] { list[i], list[l], list[r] };
                    }
                }
            }
            
            return new int[] {};
        }
        
        private static int _count = 1;
        static void runCase(int[] list, int[] expected)
        {
            int[] actual = findSum(list);
            if (Enumerable.SequenceEqual(expected, actual)) {
                Console.WriteLine("{0}: Success!", _count);
            } else {
                Console.WriteLine("{0}: Failure! [{1}] != [{2}]", _count, string.Join(",", expected), string.Join(",", actual));
            }
            ++_count;
        }

        public static void Main (string[] args)
        {
            runCase(new int[]{ -1, 0, 1 }, new int[]{ -1, 0, 1 });
            runCase(new int[]{ 1, 0, -1 }, new int[]{ -1, 0, 1 });
            runCase(new int[]{ 5, 100, 22, -27, 0 }, new int[]{ -27, 5, 22 });
            runCase(new int[]{ 4, 5, 4, -8 }, new int[]{ -8, 4, 4 });
        }
    }
}
