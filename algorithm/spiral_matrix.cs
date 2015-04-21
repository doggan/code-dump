using System;
using System.Linq;

namespace Hoge
{
    class MainClass
    {
        static int[] spiralMatrix(int[,] matrix)
        {
            int m = matrix.GetLength(0);
            int n = matrix.GetLength(1);
            
            int[] result = new int[m * n];
            int cnt = 0;
            
            int top = 0;
            int bottom = m - 1;
            int left = 0;
            int right = n - 1;
            
            while (true) {
                for (int i = left; i <= right; i++) {
                    result[cnt++] = matrix[top, i];
                }
                ++top;
                
                if (top > bottom) { break; }
                
                for (int i = top; i <= bottom; i++) {
                    result[cnt++] = matrix[i, right];
                }
                --right;
                
                if (left > right) { break; }
                
                for (int i = right; i >= left; --i) {
                    result[cnt++] = matrix[bottom, i];
                }
                --bottom;
                
                if (top > bottom) { break; }
                
                for (int i = bottom; i >= top; --i) {
                    result[cnt++] = matrix[i, left];
                }
                ++left;
                
                if (left > right) { break; }
            }
            
            return result;
        }

        private static int _count = 1;
        static void runCase(int[,] matrix, int[] expected)
        {
            int[] actual = spiralMatrix(matrix);

            bool success = Enumerable.SequenceEqual(actual, expected);

            if (success) {
                Console.WriteLine("{0}: Success! {1}", _count, string.Join(",", actual));
            }
            else {
                Console.WriteLine("{0}: Failure! {1} != {2}", _count, string.Join(",", actual), string.Join(",", expected));
            }
            ++_count;
        }

        public static void Main (string[] args)
        {
            runCase(new int[,] {
                {1},
                {2},
                {3}
                }, new int[] {1, 2, 3});
            runCase(new int[,] {
                {1, 2, 3}
                }, new int[] {1, 2, 3});
            runCase(new int[,] {
                {1, 2, 3},
                {4, 5, 6},
                {7, 8, 9}
                }, new int[] {1, 2, 3, 6, 9, 8, 7, 4, 5});
            runCase(new int[,] {
                {1, 2, 3},
                {4, 5, 6}
                }, new int[] {1, 2, 3, 6, 5, 4});
            runCase(new int[,] {
                {1, 2},
                {4, 5}
                }, new int[] {1, 2, 5, 4});
        }
    }
}
