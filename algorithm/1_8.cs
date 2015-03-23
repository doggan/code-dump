using System;

namespace Hoge
{
    /*
        Assume you have a method isSubstring which checks if one word is a substring
        of another. Given two strings, s1 and s2, write code to check If s2 is a
        rotation of s1 using only one call
        to isSubstring (e.g., "waterbottLe" is a rotation of "erbottlewat").
     */
    class MainClass
    {
        static bool check(string s1, string s2)
        {
            if (string.IsNullOrEmpty(s1) ||
                string.IsNullOrEmpty(s2) ||
                s1.Length != s2.Length) {
                return false;
            }

            return (s1 + s1).Contains(s2);
        }

        private static int _count = 1;
        static void runCase(string s1, string s2, bool expected)
        {
            bool actual = check(s1, s2);
            if (expected == actual) {
                Console.WriteLine("  {0}: Success!", _count);
            } else {
                Console.WriteLine("  {0}: Failure! {1} / {2} ({3})", _count, s1, s2, actual);
            }
            ++_count;
        }

        public static void Main (string[] args)
        {
            runCase(null, null, false);
            runCase("ab", "a", false);
            runCase("waterbottle", "erbottlewat", true);
            runCase("waterbottle", "erbottlewatt", false);
            runCase("abba", "baab", true);
        }
    }
}
