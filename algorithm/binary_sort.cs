using System;

namespace Hoge
{
	class MainClass
	{
        static void sort(int[] list)
        {
            if (list == null) {
                return;
            }

            sortImpl(list, 0, list.Length - 1);
        }

        static void sortImpl(int[] list, int min, int max)
        {
            if (min >= max) {
                return;
            }

            int p = getPivot(min, max);
            int pVal = list[p];
            swap(list, p, max);
            p = max;

            int swapIdx = min;
            for (int i = min; i < max; i++) {
                if (list[i] < pVal) {
                    swap(list, i, swapIdx);
                    swapIdx++;
                }
            }

            swap(list, swapIdx, p);
            p = swapIdx;
            sortImpl(list, min, p - 1);
            sortImpl(list, p + 1, max);
        }

        static void swap(int[] list, int i0, int i1)
        {
            int tmp = list[i0];
            list[i0] = list[i1];
            list[i1] = tmp;
        }

        static int getPivot(int min, int max)
        {
            return max;
        }

		private static int _count = 1;
		static void runCase(int[] list)
		{
			sort(list);

            bool success = true;
            if (list != null) {
                for (int i = 1; i < list.Length; i++) {
                    if (list[i - 1] > list[i]) {
                        success = false;
                        break;
                    }
                }
            }

            if (success) {
				Console.WriteLine("{0}: Success!", _count);
			}
            else {
				Console.WriteLine("{0}: Failure! {1}", _count, string.Join(",", list));
			}
			++_count;
		}

		public static void Main (string[] args)
		{
			// Driver program.
			runCase(null);
			runCase(new int[]{});
			runCase(new int[]{1, 2, 3});
			runCase(new int[]{-10, -20, 30});
			runCase(new int[]{2, 1});
			runCase(new int[]{3, 2, 4, 1});
			runCase(new int[]{3, 3, 1, 1, 2});
		}
	}
}
