using System;
using System.Diagnostics;

namespace Engine
{
    public static partial class Core
    {
        /// <summary>
        /// Throws an assert.
        /// </summary>
        [Conditional("DEBUG")]
        public static void Assert()
        {
            Debug.Assert(false);
        }

        /// <summary>
        /// Throws an assert if the given condition is false.
        /// </summary>
        [Conditional("DEBUG")]
        public static void Assert(bool condition)
        {
            Debug.Assert(condition);
        }

        /// <summary>
        /// Throws an assert with the given message.
        /// </summary>
        /// <param name="message">A composite format string.</param>
        /// <param name="args">A System.Object array containing zero or more objects to format.</param>
        [Conditional("DEBUG")]
        public static void Assert(string message, params object[] args)
        {
            Debug.Assert(false, String.Format(message, args));
        }

        /// <summary>
        /// Throws an assert with the given message if the given condition is false.
        /// </summary>
        /// <param name="message">A composite format string.</param>
        /// <param name="args">A System.Object array containing zero or more objects to format.</param>
        [Conditional("DEBUG")]
        public static void Assert(bool condition, string message, params object[] args)
        {
            Debug.Assert(condition, String.Format(message, args));
        }
    }
}
