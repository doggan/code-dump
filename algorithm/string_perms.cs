using System;
using System.Collections.Generic;

namespace Hoge
{
    class MainClass
    {
        static void generatePermutations(string val, List<string> list)
        {
            if (string.IsNullOrEmpty(val)) {
                return;
            }

            generatePermutationsImpl(string.Empty, val, list);
        }

        static void generatePermutationsImpl(string val, string remaining, List<string> list)
        {
            if (remaining.Length == 1) {
                list.Add(val + remaining);
                return;
            }

            for (int i = 0; i < remaining.Length; i++) {
                generatePermutationsImpl(val + remaining[i], remaining.Substring(0, i) + remaining.Substring(i + 1), list);
            }
        }

        private static int _count = 1;
        static void runCase(string val, int expectedCount)
        {
            List<string> list = new List<string>(expectedCount);
            generatePermutations(val, list);

            bool success = true;

            // Verify no duplicates.
            for (int i = 0; i < list.Count; i++) {
                for (int j = i + 1; j < list.Count; j++) {
                    if (list[i] == list[j]) {
                        success = false;
                        break;
                    }
                }
                if (!success) {
                    break;
                }
            }
            // Verify correct counts.
            if (list.Count != expectedCount) {
                success = false;
            }

            if (success) {
                Console.WriteLine("{0}: Success! {1}", _count, string.Join(",", list.ToArray()));
            }
            else {
                Console.WriteLine("{0}: Failure! {1}", _count, string.Join(",", list.ToArray()));
            }
            ++_count;
        }

        public static void Main (string[] args)
        {
            // Driver program.
            runCase(null, 0);
            runCase("", 0);
            runCase("a", 1);
            runCase("ab", 2 * 1);
            runCase("abc", 3 * 2 * 1);
            runCase("abcde", 5 * 4 * 3 * 2 * 1);
        }
    }
}
