using System;

namespace Hoge
{
    class MainClass
    {
        /*
            Algorithm to search in a rotated sorted array.
         */
        static int search(int[] list, int val)
        {
            if (list == null) {
                return -1;
            }

            return searchImpl(list, val, 0, list.Length - 1);
        }

        static int searchImpl(int[] list, int val, int min, int max)
        {
            if (min > max) {
                return -1;
            }

            int m = min + (max - min) / 2;
            int mVal = list[m];

            // Found match?
            if (mVal == val) {
                return m;
            }
            // Is left side sorted?
            else if (list[min] < mVal) {
                // Is match within range of left side?
                if (val >= list[min] && val <= list[m]) {
                    return searchImpl(list, val, min, m - 1);
                }
                else {
                    return searchImpl(list, val, m + 1, max);
                }
            }
            // Right side must be sorted.
            else {
                // Is match within range of right side?
                if (val >= list[m] && val <= list[max]) {
                    return searchImpl(list, val, m + 1, max);
                }
                else {
                    return searchImpl(list, val, min, m - 1);
                }
            }
        }

        private static  int _count = 1;
        static void runCase(int[] list, int val, int expected)
        {
            int actual = search(list, val);
            if (expected == actual) {
                Console.WriteLine("{0}: Success!", _count);
            } else {
                Console.WriteLine("{0}: Failure! {1} != {2}", _count, expected, actual);
            }
            ++_count;
        }

        public static void Main (string[] args)
        {
            // Driver program.
            runCase(null, 1, -1);
            runCase(new int[]{}, 1, -1);
            runCase(new int[]{1, 2, 3}, 1, 0);
            runCase(new int[]{1, 2, 3}, 5, -1);
            runCase(new int[]{3, 1, 2}, 1, 1);
            runCase(new int[]{3, 1, 2}, 3, 0);
            runCase(new int[]{3, 1, 2}, 2, 2);
            runCase(new int[]{2, 3, 1}, 1, 2);
            runCase(new int[]{3, 4, 5, 1, 2}, 2, 4);
            runCase(new int[]{3, 4, 5, 1, 2}, 8, -1);
        }
    }
}
