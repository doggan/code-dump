using System;
using System.Diagnostics;

namespace Hoge
{
    class MainClass
    {
        static ulong fibonacci_recursive(ulong n)
        {
            if (n == 0) {
                return 0;
            }
            if (n == 1) {
                return 1;
            }
            return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
        }

        static ulong fibonacci_iterative(ulong n)
        {
            if (n == 0) {
                return 0;
            }

            ulong prevPrev = 0;
            ulong prev = 1;
            ulong current = 1;

            for (ulong i = 1; i < n; i++) {
                current = prevPrev + prev;
                prevPrev = prev;
                prev = current;
            }

            return current;
        }

        const ulong DEFAULT_VALUE = 0xFFFFFFFFFFFFFFFF;

        static ulong fibonacci_dp_top_down(ulong n)
        {
            if (n == 0) {
                return 0;
            }

            ulong[] work = new ulong[n + 1];
            for (ulong i = 0; i < (ulong)work.Length; i++) { work[i] = DEFAULT_VALUE; }
            work[0] = 0;
            work[1] = 1;
            return fibonacci_dp_top_down_impl(n, work);
        }

        static ulong fibonacci_dp_top_down_impl(ulong n, ulong[] work)
        {
            if (work[n] == DEFAULT_VALUE) {
                work[n] = fibonacci_dp_top_down_impl(n - 1, work) + fibonacci_dp_top_down_impl(n - 2, work);
            }
            return work[n];
        }

        static ulong fibonacci_dp_bottom_up(ulong n)
        {
            if (n == 0) {
                return 0;
            }

            ulong[] work = new ulong[n + 1];
            work[0] = 0;
            work[1] = 1;
            for (ulong i = 2; i <= n; i++) {
                work[i] = work[i - 1] + work[i - 2];
            }

            return work[n];
        }

        static void runCase(Func<ulong, ulong> func, ulong n, ulong expected)
        {
            ulong actual = func(n);
            if (expected != actual) {
                Console.WriteLine("Failure! {0}({1}) = {2} != {3}", func.Method.Name, n, actual, expected);
            }
        }

        public static void Main (string[] args)
        {
            Func<ulong, ulong>[] funcs = {
                fibonacci_recursive,
                fibonacci_iterative,
                fibonacci_dp_top_down,
                fibonacci_dp_bottom_up
            };

            ulong n = 40;
            ulong expected = 102334155;
            foreach (var func in funcs) {
                Stopwatch timer = new Stopwatch();
                timer.Start();

                runCase(func, n, expected);

                timer.Stop();
                Console.WriteLine("{0}({1}) = {2}ms", func.Method.Name, n, timer.ElapsedMilliseconds);
            }
        }
    }
}
