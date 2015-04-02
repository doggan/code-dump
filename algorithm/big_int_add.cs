using System;

namespace Hoge
{
    /*
        Program to add 2 arrays as if they were big integers.
        
        Example: [9,0] + [1,2] = [1,0,2]
     */
    class MainClass
    {
        static int[] bigIntAdd(int[] a, int[] b)
        {
            int maxLength = Math.Max(a.Length, b.Length) + 1;
            int[] result = new int[maxLength];
            
            int placeOffset = 0;
            int carry = 0;
            for (int i = result.Length - 1; i >= 0; i--, placeOffset++) {
                int aPlace = a.Length - 1 - placeOffset;
                int bPlace = b.Length - 1 - placeOffset;
                int aVal = aPlace >= 0 ? a[aPlace] : 0;
                int bVal = bPlace >= 0 ? b[bPlace] : 0;
                int sum = aVal + bVal + carry;
                if (sum >= 10) {
                    result[i] = sum % 10;
                    carry = sum / 10;
                } else {
                    result[i] = sum;
                    carry = 0;
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
            int[] actual = bigIntAdd(a, b);
            
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
                Console.WriteLine("{0}: Success! {1} + {2} = {3}", _count, string.Join("", a), string.Join("", b), string.Join("", actual));
            }
            else {
                Console.WriteLine("{0}: Failure! {1} + {2} != {3} ({4})", _count, string.Join("", a), string.Join("", b), string.Join("", actual), string.Join("", expected));
            }
            ++_count;
        }

        public static void Main (string[] args)
        {
            // Driver program.
            runCase(new int[]{2}, new int[]{4}, new int[]{6});
            runCase(new int[]{5}, new int[]{1,0}, new int[]{1,5});
            runCase(new int[]{5}, new int[]{1,5}, new int[]{2,0});
            runCase(new int[]{9}, new int[]{9,1}, new int[]{1,0,0});
            runCase(new int[]{2,1,4,7,4,8,3,6,4,7}, new int[]{2,1,4,7,4,8,3,6,4,7}, new int[]{4,2,9,4,9,6,7,2,9,4});
        }
    }
}
