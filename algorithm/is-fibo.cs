using System;

namespace Solution
{
    /*
        https://www.hackerrank.com/challenges/is-fibo

        mcs is-fibo.cs && mono is-fibo.exe < is-fibo.txt
     */
    class Solution
    {
        static bool IsFibo(ulong val)
        {
            if (val == 0 ||
                val == 1) {
                return true;
            } else if (val < 0) {
                return false;
            }

            ulong prevPrev = 0;
            ulong prev = 1;
            ulong current;
            while (true) {
                current = prevPrev + prev;

                if (current == val) {
                    return true;
                } else if (current > val) {
                    return false;
                }

                prevPrev = prev;
                prev = current;
            }
        }

        public static void Main (string[] args)
        {
            // Parse input...
            int testCount = Convert.ToInt32(Console.ReadLine());
            for (int i = 0; i < testCount; i++) {
                ulong val = Convert.ToUInt64(Console.ReadLine());
                Console.WriteLine(IsFibo(val) ? "IsFibo" : "IsNotFibo");
            }
        }
    }
}
