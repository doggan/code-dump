using System;

namespace Hoge
{
    /*
        Calculate the # of ways we can make change with a given set of coins.
     */
    class MainClass
    {
        static int calculate(int[] coins, int total)
        {
            if (coins == null) {
                return 0;
            }

            int[] counts = new int[coins.Length];
            return calculateImpl(coins, counts, 0, total);
        }

        static int calculateImpl(int[] coins, int[] counts, int coinIndex, int total)
        {
            if (total < 0 ||
                coinIndex == coins.Length) {
                return 0;
            }
            if (total == 0) {
                string str = string.Empty;
                for (int i = 0; i <= coinIndex; i++) {
                    str += counts[i] + "*" + coins[i] + (i == coinIndex ? "" : " + ");
                }
                Console.WriteLine(str);
                return 1;
            }

            // The # of combinations WITHOUT this coin.
            int without = calculateImpl(coins, counts, coinIndex + 1, total);

            // The # of combinations with AT LEAST ONE of this coin.
            int[] withCounts = counts.Clone() as int[];
            withCounts[coinIndex] += 1;
            int with = calculateImpl(coins, withCounts, coinIndex, total - coins[coinIndex]);

            return without + with;
        }

        static void runCase(int[] coins, int total)
        {
            int actual = calculate(coins, total);
            Console.WriteLine("Total Combinations: " + actual);
            Console.WriteLine("------");
        }

        public static void Main (string[] args)
        {
            runCase(new[]{2, 1}, 3);
            runCase(new[]{25, 10, 5, 1}, 100);
        }
    }
}
