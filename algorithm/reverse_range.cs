using System;
using System.Text;

class Solution
{
    class ListNode {
        public ListNode(int v) {
            val = v;
        }

        public ListNode next;
        public int val;
    };

    static void PrintList(ListNode node) {
        StringBuilder sb = new StringBuilder();
        while (node != null) {
            sb.Append(node.val + " -> ");
            node = node.next;
        }
        sb.Append("NULL");
        Console.WriteLine(sb);
    }

    static ListNode BuildList(int[] values) {
        if (values == null || values.Length == 0) {
            return null;
        }

        ListNode head = new ListNode(values[0]);
        ListNode tail = head;
        for (int i = 1; i < values.Length; i++) {
            tail.next = new ListNode(values[i]);
            tail = tail.next;
        }
        return head;
    }

    static ListNode _nNodeNext = null;
    static ListNode ReverseK(ListNode node, int k) {
        if (node == null ||
            node.next == null ||
            k == 0) {
            _nNodeNext = node.next;
            return node;
        }

        ListNode head = ReverseK(node.next, k - 1);
        node.next.next = node;
        node.next = null;
        return head;
    }

    static ListNode ReverseRange(ListNode head, int m, int n)
    {
        _nNodeNext = null;

        if (m == n) {
            return head;
        }

        ListNode mNodePrev = null;
        ListNode mNode = head;
        for (int i = 1; i < m; i++) {
            mNodePrev = mNode;
            mNode = mNode.next;
        }

        int k = n - m;
        ListNode newHead = ReverseK(mNode, k);
        mNode.next = _nNodeNext;
        if (mNodePrev != null) {
            mNodePrev.next = newHead;
            return head;    // head stays same
        } else {
            return newHead;
        }
    }

    public static void Main (string[] args)
    {
        {
            ListNode n = BuildList(new int[]{1, 2, 3, 4, 5});
            PrintList(n);
            PrintList(ReverseRange(n, 4, 5));
        }
    }
}
