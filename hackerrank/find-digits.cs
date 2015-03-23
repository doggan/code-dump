using System;

namespace Solution
{
    /*
        https://www.hackerrank.com/challenges/find-digits

        mcs find-digits.cs && mono find-digits.exe < find-digits.txt
     */
    class Solution
    {
        static int FindDigits(int val)
        {
            int tmp = val;
            int cnt = 0;
            while (tmp != 0) {
                int digit = tmp % 10;

                if (digit != 0 &&
                    (val % digit) == 0) {
                    ++cnt;
                }

                tmp /= 10;
            }
            return cnt;
        }

        public static void Main (string[] args)
        {
            // Parse input...
            int testCount = Convert.ToInt32(Console.ReadLine());
            for (int i = 0; i < testCount; i++) {
                int val = Convert.ToInt32(Console.ReadLine());
                Console.WriteLine(FindDigits(val));
            }
        }
    }
}
