using System.Windows.Input;

namespace Engine
{
    public interface IMessage
    {

    }

    public struct KeyDownMessage : IMessage
    {
        public object Sender { get; set; }
        public KeyEventArgs KeyEventArgs { get; set; }
    }

    public struct MoveMessage : IMessage
    {
        public enum MoveDirection
        {
            LEFT,
            RIGHT,
            DOWN,
            UP
        }

        public MoveDirection Direction { get; set; }
    }

    public struct RotateMessage : IMessage
    {

    }

    public struct AddToBoardMessage : IMessage
    {
        public BoardComponent Board { get; set; }
    }
}