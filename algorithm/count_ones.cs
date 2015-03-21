using System;

namespace Hoge
{
    class MainClass
    {
        /*
            Execution time depends on # of total bits.
         */
        static int countOnes_0(int val)
        {
            int cnt = 0;
            while (val > 0) {
                if ((val & 1) != 0) {
                    cnt++;
                }
                val >>= 1;
            }
            return cnt;
        }

        /*
            Execution time depends on # of 1 bits.
         */
        static int countOnes_1(int val)
        {
            int cnt = 0;
            while (val > 0) {
                cnt++;
                val &= (val - 1);
            }

            return cnt;
        }

        /*
            Use a pre-calculated LUT.
         */
        static int[] BIT_BUFFER = new int[256];
        static int countOnes_2(int val)
        {
            return
                BIT_BUFFER[val & 0xFF] +
                BIT_BUFFER[(val >> 8) & 0xFF] +
                BIT_BUFFER[(val >> 16) & 0xFF] +
                BIT_BUFFER[(val >> 24) & 0xFF];
        }

        static void init()
        {
            for (int i = 0; i < BIT_BUFFER.Length; i++) {
                BIT_BUFFER[i] = countOnes_0(i);
            }
        }

        private static  int _count = 1;
        static void runCase(Func<int, int> func, string valStr, int expected)
        {
            int val = Convert.ToInt32(valStr, 2);
            int actual = func(val);
            if (expected == actual) {
                Console.WriteLine("  {0}: Success!", _count);
            } else {
                Console.WriteLine("  {0}: Failure! {1} != {2} ({3})", _count, expected, actual, Convert.ToString(val, 2));
            }
            ++_count;
        }

        public static void Main (string[] args)
        {
            Func<int, int>[] funcs = {
                countOnes_0,
                countOnes_1,
                countOnes_2
            };

            init();

            foreach (var func in funcs) {
                Console.WriteLine("{0}:", func.Method.Name);

                runCase(func, "0", 0);
                runCase(func, "1", 1);
                runCase(func, "100", 1);
                runCase(func, "101", 2);
                runCase(func, "1111", 4);
            }
        }
    }
}
