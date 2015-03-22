using System;

namespace Hoge
{
    class MainClass
    {
        static void sort(int[] list)
        {
            if (list == null || list.Length == 0) {
                return;
            }

            int[] buffer = new int[list.Length];
            sortImpl(list, buffer, 0, list.Length - 1);
        }

        static void sortImpl(int[] list, int[] buffer, int min, int max)
        {
            if (min == max) {
                return;
            }

            int p = min + (max - min) / 2;
            sortImpl(list, buffer, min, p);
            sortImpl(list, buffer, p + 1, max);
            merge(list, buffer, min, p, max);
        }

        static void merge(int[] list, int[] buffer, int min, int p, int max)
        {
            int cnt = 0;
            int i0 = min; int end0 = p;
            int i1 = end0 + 1; int end1 = max;
            while ((i0 <= end0) && (i1 <= end1)) {
                if (list[i0] < list[i1]) {
                    buffer[cnt++] = list[i0++];
                } else {
                    buffer[cnt++] = list[i1++];
                }
            }
            while (i0 <= end0) {
                buffer[cnt++] = list[i0++];
            }
            while (i1 <= end1) {
                buffer[cnt++] = list[i1++];
            }
            for (int i = 0; i < cnt; i++) {
                list[min + i] = buffer[min + i];
            }
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
