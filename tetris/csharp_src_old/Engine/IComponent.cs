using System;

namespace Engine
{
    public abstract class Component
    {
        private string _name;
        private WeakReference _owner;

        public string Name { get { return _name; } }
        public GameObject Owner
        {
            get
            {
                return _owner.Target as GameObject;
            }
            internal set
            {
                Core.Assert(value is GameObject);
                _owner = new WeakReference(value);
            }
        }

        public Component(string name)
        {
            _name = name;
        }

        internal virtual void OnInitCompleted()
        {

        }

        public virtual void ReceiveMessage(ref IMessage message)
        {

        }
    }
}

// TODO: wrap bool[,]
    // in a configuration class?

/*
 * Game object is created (board)
 * Add 2D board component : UserControl
 *   - the canvas that is actually displayed
 * Add next piece logic component
 * Add line clearing logic component
 * Add scoring component
 * Add input logic component
 *   - if left arrow pressed, get the active shape on the board (game object),
 *          and send a MoveLeft message (better name?)
 * 
 * 
 * Game object (shape L)
 * Add body component
 * Add movement component
 *      - handles wall kicks and can get as complicated as necessary
 * 
 * 
 * 
 * - active shape receives the MoveLeft message
 * - movement component adjusts the body position of the shape
 *      - has reference to the board body component (which it received when
 *          added to the board), to use for boundary checks.
 * - body component re-draws itself when coordinates change
 *      by occupying a space in the board
 * - when occupy is called, board fires a message (BoardStateChange)
 * - line clearing logic checks to see if a line has been cleared, if so,
 *      send a LineCleared message
 * - scoring component increases the score
 * 
 * ...
 * - 
 * - next piece logic calculates the next piece and sets it in the next piece display
 * 
 * 
 * 
 * Ghost piece?
 *  - Add a ghost piece plugin the the shape. It is updated whenever the
 *      piece is active and the shape's body component coordinates change (CoordChangedMessage)
*/