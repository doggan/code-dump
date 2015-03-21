using System;

namespace Hoge
{
    class MainClass
    {
        /*
            Algorithm to find the minimum valued index in a rotated sorted array.
            Elements in the array must be distinct:
                {5, 5, 5, 4, 5} (expected = 3) would break it because the algorithm
                doesn't know if it should search the left half or the right half.
         */
        static int findMinIndex(int[] list)
        {
            if (list == null || list.Length == 0) {
                return -1;
            }

            return findMinIndexImpl(list, 0, list.Length - 1);
        }

        static int findMinIndexImpl(int[] list, int min, int max)
        {
            // Midpoint calculation.
            int m = min + (max - min) / 2;
            int mVal = list[m];

            // Is m the rotation point?
            if (m > 0 && list[m - 1] > mVal) {
                return m;
            }
            // Rotation point is to the left?
            else if (list[min] > mVal) {
                return findMinIndexImpl(list, min, m - 1);
            }
            // Rotation point is to the right?
            else if (list[max] < mVal) {
                return findMinIndexImpl(list, m + 1, max);
            }
            // No rotation in this part of the list.
            else {
                return min;
            }
        }

        private static  int _count = 1;
        static void runCase(int[] list, int expected)
        {
            int actual = findMinIndex(list);
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
            runCase(null, -1);
            runCase(new int[]{}, -1);
            runCase(new int[]{1, 2, 3}, 0);
            runCase(new int[]{3, 1, 2}, 1);
            runCase(new int[]{2, 3, 1}, 2);
            runCase(new int[]{3, 4, 5, 1, 2}, 3);
        }
    }
}
