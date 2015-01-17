using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Engine.Shapes;

namespace Engine
{
    /*
    public partial class Board
    {
        public int NumColumns { get; set; }
        public int NumRows { get; set; }

        public bool IsOccupied(int row, int column)
        {
            Core.Assert(row < NumRows && column < NumColumns);
            return _occupancyMap[row, column];
        }

        private bool[,] _occupancyMap;
 //       private IShape _activeShape;
        private GameObject _activeShapeObj;

        public Board()
        {
            InitializeComponent();

            Loaded += new RoutedEventHandler(OnLoaded);
        }

        private void OnLoaded(object sender, RoutedEventArgs e)
        {
            //LayoutRoot.Columns = BoardWidth;
            //LayoutRoot.Rows = BoardHeight;
            for (int i = 0; i < NumColumns; ++i)
            {
                ColumnDefinition col = new ColumnDefinition();
                LayoutRoot.ColumnDefinitions.Add(col);
            }

            for (int i = 0; i < NumRows; ++i)
            {
                RowDefinition row = new RowDefinition();
                LayoutRoot.RowDefinitions.Add(row);
            }

            _occupancyMap = new bool[NumRows, NumColumns];

            //_activeShape = new ShapeO(this);
            //_activeShape = new ShapeL(this);
            _activeShapeObj = new GameObject("ShapeL");
            _activeShapeObj.AddComponent(new ShapeComponent("Body"));
            _activeShapeObj.AddComponent(new ShapeMovementComponent("Movement"));
            _activeShapeObj.AddComponent(new ShapeRotationComponent("Rotation"));

            AddToBoardMessage m = new AddToBoardMessage();
            m.Board = this;
            _activeShapeObj.SendMessage(m);
        }

        // TEMP:
        public void Draw()
        {
//            if (_activeShape != null)
//                _activeShape.Draw();
            // temp:
            _activeShapeObj.Update(1);
        }

        public void Occupy(int row, int column, Block b)
        {
//            b.SetValue(UniformGrid.ColumnsProperty, i);
  //          b.SetValue(UniformGrid.RowsProperty, j);
//            LayoutRoot.SetValue(UniformGrid.RowsProperty, i);
  //          LayoutRoot.SetValue(UniformGrid.ColumnsProperty, j);
    //        LayoutRoot.Children.Add(b);

            if (LayoutRoot.Children.Contains(b))
            {
                int oldRow = Grid.GetRow(b);
                int oldColumn = Grid.GetColumn(b);
//                System.Diagnostics.Debug.Assert(IsOccupied(oldRow, oldColumn) == true);
     //           _occupancyMap[oldRow, oldColumn] = false;

                LayoutRoot.Children.Remove(b);
            }

            LayoutRoot.Children.Add(b);
            Grid.SetRow(b, row);
            Grid.SetColumn(b, column);
     //       _occupancyMap[row, column] = true;
        }

        public void MoveLeft()
        {
            MoveMessage m = new MoveMessage();
            m.Direction = MoveMessage.MoveDirection.LEFT;
            _activeShapeObj.SendMessage(m);
        }

        public void MoveRight()
        {
            MoveMessage m = new MoveMessage();
            m.Direction = MoveMessage.MoveDirection.RIGHT;
            _activeShapeObj.SendMessage(m);
        }

        public void MoveDown()
        {
            MoveMessage m = new MoveMessage();
            m.Direction = MoveMessage.MoveDirection.DOWN;
            _activeShapeObj.SendMessage(m);
        }

        public void Rotate()
        {
            RotateMessage m = new RotateMessage();
            _activeShapeObj.SendMessage(m);
//             MoveMessage m = new MoveMessage();
//             m.Direction = MoveMessage.MoveDirection.UP;
//             _activeShapeObj.SendMessage(m);
        }
    }*/

    public sealed class GameObjectManager
    {
        private List<GameObject> _gameObjects = new List<GameObject>();

        private static GameObjectManager _instance;
//        public static GameObjectManager Instance { get { return _instance; } }

        public GameObjectManager()
        {
            Core.Assert(_instance == null);
            _instance = this;
        }

        public static void AddGameObject(GameObject gameObject)
        {
            _instance._gameObjects.Add(gameObject);
        }

        public static void Update()
        {
            foreach (GameObject go in _instance._gameObjects)
            {
                go.Update(1);
            }
        }
    }

    public class BoardInputComponent : Component
    {
        public BoardInputComponent(string name) :
            base(name)
        {

        }

        public override void ReceiveMessage(ref IMessage message)
        {
            if (message is KeyDownMessage)
            {
                KeyDownMessage m = (KeyDownMessage)message;

                BoardComponent board = Owner.GetComponent<BoardComponent>();
                GameObject activeShape = board.ActiveShape;

                if (activeShape != null)
                {
                    switch (m.KeyEventArgs.Key)
                    {
                        case Key.Left:
                            {
                                MoveMessage m2 = new MoveMessage();
                                m2.Direction = MoveMessage.MoveDirection.LEFT;
                                activeShape.SendMessage(m2);
                            }
                            break;
                        case Key.Right:
                            {
                                MoveMessage m2 = new MoveMessage();
                                m2.Direction = MoveMessage.MoveDirection.RIGHT;
                                activeShape.SendMessage(m2);
                            }
                            break;
                        case Key.Down:
                            {
                                MoveMessage m2 = new MoveMessage();
                                m2.Direction = MoveMessage.MoveDirection.DOWN;
                                activeShape.SendMessage(m2);
                            }
                            break;
                        case Key.Up:
                            {
                                RotateMessage m2 = new RotateMessage();
                                activeShape.SendMessage(m2);
                            }
                            break;
                    }
                }
            }
        }
    }

    public class BoardComponent : Component
    {
        public int NumColumns { get; set; }
        public int NumRows { get; set; }

        public GameObject ActiveShape { get { return _activeShape; } }

        // TEMP: should be private
        public BoardControl _control;

        private bool[,] _occupancyMap;
        private GameObject _activeShape;

        public BoardComponent(string name) :
            base(name)
        {
            _control = new BoardControl(this);

            NumRows = 21;
            NumColumns = 10;

            _occupancyMap = new bool[NumRows, NumColumns];

            _activeShape = new GameObject("ShapeL");
            _activeShape.AddComponent(new ShapeComponent("Body"));
            _activeShape.AddComponent(new ShapeMovementComponent("Movement"));
            _activeShape.AddComponent(new ShapeRotationComponent("Rotation"));
            GameObjectManager.AddGameObject(_activeShape);

            AddToBoardMessage m = new AddToBoardMessage();
            m.Board = this;
            _activeShape.SendMessage(m);
        }

        public bool IsOccupied(int row, int column)
        {
            Core.Assert(row < NumRows && column < NumColumns);
            return _occupancyMap[row, column];
        }

        public void Occupy(int row, int column, Block b)
        {
            //            b.SetValue(UniformGrid.ColumnsProperty, i);
            //          b.SetValue(UniformGrid.RowsProperty, j);
            //            LayoutRoot.SetValue(UniformGrid.RowsProperty, i);
            //          LayoutRoot.SetValue(UniformGrid.ColumnsProperty, j);
            //        LayoutRoot.Children.Add(b);

            if (_control.LayoutRoot.Children.Contains(b))
            {
                int oldRow = Grid.GetRow(b);
                int oldColumn = Grid.GetColumn(b);
                //                System.Diagnostics.Debug.Assert(IsOccupied(oldRow, oldColumn) == true);
                //           _occupancyMap[oldRow, oldColumn] = false;

                _control.LayoutRoot.Children.Remove(b);
            }

            _control.LayoutRoot.Children.Add(b);
            Grid.SetRow(b, row);
            Grid.SetColumn(b, column);
            //       _occupancyMap[row, column] = true;
        }
    }

    // TODO: don't want to expose this class outside of BoardComponent.
    public partial class BoardControl : UserControl
    {
        private BoardComponent _component;

        public BoardControl(BoardComponent component)
        {
            _component = component;

            InitializeComponent();

            Loaded += new RoutedEventHandler(OnLoaded);
        }

        private void OnLoaded(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < _component.NumColumns; ++i)
            {
                ColumnDefinition col = new ColumnDefinition();
                LayoutRoot.ColumnDefinitions.Add(col);
            }

            for (int i = 0; i < _component.NumRows; ++i)
            {
                RowDefinition row = new RowDefinition();
                LayoutRoot.RowDefinitions.Add(row);
            }



            //AddToBoardMessage m = new AddToBoardMessage();
            //m.Board = this;
            //_activeShapeObj.SendMessage(m);
        }
    }
}
