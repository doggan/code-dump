using System;

namespace Hoge
{
	class MainClass
	{
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

			int m = midpoint(min, max);
			int mVal = list[m];
			if (val < mVal) {
				return searchImpl(list, val, min, m - 1);
			}
			else if (val > mVal) {
				return searchImpl(list, val, m + 1, max);
			}
			else {
				return m;
			}
		}

		static int midpoint(int min, int max)
		{
			return min + (max - min) / 2;
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
			runCase(new int[]{1, 2, 3}, 3, 2);
			runCase(new int[]{1, 2, 3}, 6, -1);
			runCase(new int[]{1, 2, 3, 8, 9}, 8, 3);
			runCase(new int[]{1, 2, 3, 8, 9}, 1, 0);
		}
	}
}
