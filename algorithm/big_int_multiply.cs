using System;

namespace Hoge
{
    class MainClass
    {
        static int[] bigIntMultiply(int[] a, int[] b)
        {
            // Handle 0 * x.
            if ((a.Length == 1 && a[0] == 0) ||
                (b.Length == 1 && b[0] == 0)) {
                return new int[] { 0 };
            }
            
            int maxLength = a.Length + b.Length;
            int[] result = new int[maxLength];
            
            int startDigit = result.Length - 1;
            for (int i = a.Length - 1; i >= 0; i--) {
                int digit = startDigit--;
                int remainder = 0;
                for (int j = b.Length - 1; j >= 0; j--) {
                    int val = result[digit] + a[i] * b[j] + remainder;
                    
                    if (val >= 10) {
                        result[digit] = val % 10;
                        remainder = val / 10;
                    } else {
                        result[digit] = val;
                        remainder = 0;
                    }
                    digit--;
                }
                
                if (remainder != 0) {
                    result[digit] = remainder;
                }
            }
            
            // Prune leading 0's.
            {
                int firstDigit = 0;
                while (result[firstDigit] == 0) {
                    ++firstDigit;
                }
                if (firstDigit != 0) {
                    int[] newResult = new int[result.Length - firstDigit];
                    for (int i = 0; i < newResult.Length; i++) {
                        newResult[i] = result[firstDigit + i];
                    }
                    result = newResult;
                }
            }
            
            return result;
        }

        private static int _count = 1;
        static void runCase(int[] a, int[] b, int[] expected)
        {
            int[] actual = bigIntMultiply(a, b);
            
            bool success = true;
            if (actual.Length != expected.Length) {
                success = false;
            } else {
                for (int i = 0; i < expected.Length; i++) {
                    if (expected[i] != actual[i]) {
                        success = false;
                        break;
                    }
                }
            }
            
            if (success) {
                Console.WriteLine("{0}: Success! {1} x {2} = {3}", _count, string.Join("", a), string.Join("", b), string.Join("", actual));
            }
            else {
                Console.WriteLine("{0}: Failure! {1} x {2} != {3} ({4})", _count, string.Join("", a), string.Join("", b), string.Join("", actual), string.Join("", expected));
            }
            ++_count;
        }

        public static void Main (string[] args)
        {
            runCase(new int[]{0}, new int[]{4}, new int[]{0});
            runCase(new int[]{2}, new int[]{4}, new int[]{8});
            runCase(new int[]{5}, new int[]{1,0}, new int[]{5,0});
            runCase(new int[]{2}, new int[]{5,0}, new int[]{1,0,0});
            runCase(new int[]{5}, new int[]{2,2}, new int[]{1,1,0});
            runCase(new int[]{1,0}, new int[]{5,0}, new int[]{5,0,0});
            runCase(new int[]{3,7}, new int[]{5,8}, new int[]{2,1,4,6});
            runCase(new int[]{3,7}, new int[]{5,8,2,3}, new int[]{2,1,5,4,5,1});
            runCase(new int[]{5,8,2,3}, new int[]{3,7}, new int[]{2,1,5,4,5,1});
            runCase(new int[]{1,2,3,4,5,6,7,8,9,0}, new int[]{9,8,7,6,5,4,3,2,1}, new int[]{1,2,1,9,3,2,6,3,1,1,1,2,6,3,5,2,6,9,0});
        }
    }
}
