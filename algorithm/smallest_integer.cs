using System;

namespace Hoge
{
    /* Finds the smallest integer value not in a list.
    */
    class MainClass
    {
        static void swap(int[] list, int i, int j)
        {
            int tmp = list[i];
            list[i] = list[j];
            list[j] = tmp;
        }
        
        static int smallestInteger(int[] list)
        {
            // Runtime: O(n)
            // Memory: O(1) - overwrites the input list
            
            int maxVal = list.Length + 1;
            
            for (int i = 0; i < list.Length; /*i++*/) {
                // If the entry is less than the max possible value AND
                // the entry is not at the correct index AND
                // the entry is not equal to the entry we need to swap it with... (this last case prevents infinite swapping in the case of duplicates)
                if (list[i] < maxVal &&
                    list[i] != (i + 1) &&
                    list[i] != list[list[i] - 1]) {
                    swap(list, i, list[i] - 1);
                    continue;
                }
                
                i++;
            }
            
            // Find the first entry that doesn't match it's index - this is the smallest value not in the list.
            for (int i = 0; i < list.Length; i++) {
                if (list[i] != (i + 1)) {
                    return i + 1;
                }
            }
            
            // No open entries in the list, so we have to return the next highest value.
            return maxVal;
        }

        private static int _count = 1;
        static void runCase(int[] list, int expected)
        {
            int actual = smallestInteger(list);
            
            if (actual == expected) {
                Console.WriteLine("{0}: Success!", _count);
            }
            else {
                Console.WriteLine("{0}: Failure! {1} != {2} ({3})", _count, expected, actual, string.Join(",", list));
            }
            ++_count;
        }

        public static void Main (string[] args)
        {
            runCase(new int[]{3, 2, 1}, 4);
            runCase(new int[]{1, 3, 2}, 4);
            runCase(new int[]{1, 4, 2}, 3);
            runCase(new int[]{1, 4, 2, 4}, 3);
            runCase(new int[]{2, 3}, 1);
            runCase(new int[]{1, 3}, 2);
        }
    }
}
