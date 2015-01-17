using System;
using System.Collections.Generic;

namespace Engine
{
    public static class ShapeHelper
    {
        public static int GetNextConfigurationIndex(int currentIndex, int numConfigurations)
        {
            Core.Assert(currentIndex >= 0 && currentIndex < numConfigurations);
            return (currentIndex + 1) % numConfigurations;
//             currentIndex++;
//             if (currentIndex == numConfigurations)
//             {
//                 currentIndex = 0;
//             }
//             return currentIndex;
        }

        public static int GetMostLeftPositionAbsolute(ref bool[,] config, int column)
        {
            return GetMostLeftPositionRelative(ref config) + column;
        }

        public static int GetMostLeftPositionRelative(ref bool[,] config)
        {
            for (int col = 0; col < config.GetLength(1); ++col)
            {
                for (int row = 0; row < config.GetLength(0); ++row)
                {
                    if (config[row, col] == true)
                    {
                        return col;
                    }
                }
            }

            Core.Assert("Configuration not set.");
            return -1;
        }

        public static int GetMostRightPositionAbsolute(ref bool[,] config, int column)
        {
            return GetMostRightPositionRelative(ref config) + column;
        }

        public static int GetMostRightPositionRelative(ref bool[,] config)
        {
            for (int col = config.GetLength(1) - 1; col >= 0; --col)
            {
                for (int row = 0; row < config.GetLength(0); ++row)
                {
                    if (config[row, col] == true)
                    {
                        return col;
                    }
                }
            }

            Core.Assert("Configuration not set.");
            return -1;
        }

        public static int GetMostTopPositionAbsolute(ref bool[,] config, int row)
        {
            return GetMostTopPositionRelative(ref config) + row;
        }

        public static int GetMostTopPositionRelative(ref bool[,] config)
        {
            for (int row = 0; row < config.GetLength(0); ++row)
            {
                for (int col = 0; col < config.GetLength(1); ++col)
                {
                    if (config[row, col] == true)
                    {
                        return row;
                    }
                }
            }

            Core.Assert("Configuration not set.");
            return -1;
        }

        public static int GetMostBottomPositionAbsolute(ref bool[,] config, int row)
        {
            return GetMostBottomPositionRelative(ref config) + row;
        }

        public static int GetMostBottomPositionRelative(ref bool[,] config)
        {
            for (int row = config.GetLength(0) - 1; row >= 0; --row)
            {
                for (int col = 0; col < config.GetLength(1); ++col)
                {
                    if (config[row, col] == true)
                    {
                        return row;
                    }
                }
            }

            Core.Assert("Configuration not set.");
            return -1;
        }
    }

    public class ShapeComponent : Component
    {
        private BoardComponent _board;
        private Block[] _blocks;

        private int _currentRow = 0;
        private int _currentColumn = 0;
        private int _currentConfigurationIndex = 0;
        private const int _numConfigurations = 4;

        public BoardComponent Board
        {
            get
            {
                return _board;
            }
        }

        public int CurrentRow
        {
            get
            {
                return _currentRow;
            }
            set
            {
                _currentRow = value;
            }
        }
        public int CurrentColumn
        {
            get
            {
                return _currentColumn;
            }
            set
            {
                _currentColumn = value;
            }
        }

        public int NumConfigurations
        {
            get
            {
                return _numConfigurations;
            }
        }
        public bool[,] GetConfiguration(int index)
        {
            Core.Assert(index >= 0 && index < NumConfigurations);
            return _configs[index];
        }
        public int CurrentConfigurationIndex
        {
            get
            {
                return _currentConfigurationIndex;
            }
            set
            {
                Core.Assert(value >= 0 && value < NumConfigurations);
                _currentConfigurationIndex = value;
            }
        }
        public bool[,] CurrentConfiguration
        {
            get
            {
                return _configs[_currentConfigurationIndex];
            }
        }

        #region Shape configurations: http://tetris.wikia.com/wiki/SRS
        private readonly List<bool[,]> _configs = new List<bool[,]>
        {
            // 0
            new bool[3,3]
            {
                { false,  false, true },
                { true,  true,  true  },
                { false, false, false }
            },
            // 1
            new bool[3,3]
            {
                { false, true, false },
                { false, true, false },
                { false, true, true  }
            },
            // 2
            new bool[3,3]
            {
                { false, false, false },
                { true,  true,  true  },
                { true,  false, false }
            },
            // 3
            new bool[3,3]
            {
                { true,  true, false },
                { false, true, false },
                { false, true, false }
            }
        };
        #endregion
        
        public ShapeComponent(string name) :
            base(name)
        {
            CurrentColumn = 0;
            CurrentRow = 0;

            _blocks = new Block[4];
            for (uint i = 0; i < 4; ++i)
            {
                _blocks[i] = new Block();
            }
        }

        internal override void OnInitCompleted()
        {
            Owner.AddTicked(new TickHandler(Update));
        }

        private void Update(float dt)
        {
            bool[,] config = _configs[_currentConfigurationIndex];
            int blockNum = 0;

            for (int row = 0; row < config.GetLength(0); ++row)
            {
                for (int col = 0; col < config.GetLength(1); ++col)
                {
                    if (config[row, col] == true)
                    {
                        _board.Occupy(CurrentRow + row, CurrentColumn + col, _blocks[blockNum]);
                        blockNum++;
                    }
                }
            }
        }

        public override void ReceiveMessage(ref IMessage message)
        {
            if (message is AddToBoardMessage)
            {
                AddToBoardMessage m = (AddToBoardMessage)message;
                _board = m.Board;
            }
        }
    }

    public class ShapeMovementComponent : Component
    {
        public ShapeMovementComponent(string name) :
            base(name)
        {

        }

        public override void ReceiveMessage(ref IMessage message)
        {
            if (message is MoveMessage)
            {
                GameObject owner = Owner;
                if (owner != null)
                {
                    ShapeComponent shapeComponent = owner.GetComponent("Body") as ShapeComponent;
          //          ShapeComponent shapeComponent = owner.GetComponent<IBody>();
                    if (shapeComponent != null)
                    {
                        MoveMessage m = (MoveMessage)message;
                        
                        bool[,] config = shapeComponent.CurrentConfiguration;
                        int row = shapeComponent.CurrentRow;
                        int col = shapeComponent.CurrentColumn;
                        
                        switch (m.Direction)
                        {
                            case MoveMessage.MoveDirection.LEFT:
                                if (ShapeHelper.GetMostLeftPositionAbsolute(ref config, col ) > 0)
                                {
                                    // test collisions
                                    shapeComponent.CurrentColumn--;
                                }
                                break;
                            case MoveMessage.MoveDirection.RIGHT:
                                if (ShapeHelper.GetMostRightPositionAbsolute(ref config, col) < shapeComponent.Board.NumColumns - 1)
                                {
                                    // test collisions
                                    shapeComponent.CurrentColumn++;
                                }
                                break;
                            case MoveMessage.MoveDirection.DOWN:
                                if (ShapeHelper.GetMostBottomPositionAbsolute(ref config, row) < shapeComponent.Board.NumRows - 1)
                                {
                                    // test collisions
                                    shapeComponent.CurrentRow++;
                                }
                                break;
//                             case MoveMessage.MoveDirection.UP:
//                                 if (ShapeHelper.GetMostTopPositionAbsolute(ref config, row) > 0)
//                                 {
//                                     // test collisions
//                                     shapeComponent.CurrentRow--;
//                                 }
//                                 break;
                        }
                    }
                }
            }
        }
    }

    public class ShapeRotationComponent : Component
    {
//        private bool _isWallKickEnabled;
        
        public ShapeRotationComponent(string name) :
            base(name)
        {

        }

        public override void ReceiveMessage(ref IMessage message)
        {
            if (message is RotateMessage)
            {
                GameObject owner = Owner;
                if (owner != null)
                {
                    ShapeComponent shapeComponent = owner.GetComponent("Body") as ShapeComponent;
                    if (shapeComponent != null)
                    {
                        int nextIndex = ShapeHelper.GetNextConfigurationIndex(shapeComponent.CurrentConfigurationIndex, shapeComponent.NumConfigurations);
                        bool[,] nextConfig = shapeComponent.GetConfiguration(nextIndex);

                        if (IsValidConfiguration(ref nextConfig, shapeComponent))
                        {
                            // Rotate.
                            shapeComponent.CurrentConfigurationIndex = nextIndex;
                        }
                    }
                }
            }
        }

        private struct Coord2D
        {
            int row;
            int col;
        }

//         private void Omg(ShapeComponent shapeComponent)
//         {
//             bool[,] config = shapeComponent.CurrentConfiguration;
//             List<Coord2D> occupiedList = new List<Coord2D>();
// 
//             for (int row = 0; row < config.GetLength(0); ++row)
//             {
//                 for (int col = 0; col < config.GetLength(1); ++col)
//                 {
//                     if (config[row, col] == true)
//                     {
//                         int[] coord = {row, col};
//                         occupiedList.Add(coord);
//                     }
//                 }
//             }
// 
//             return occupiedList;
//         }

        private bool IsValidConfiguration(ref bool[,] config, ShapeComponent shapeComponent)
        {
            BoardComponent board = shapeComponent.Board;

            for (int row = 0; row < config.GetLength(0); ++row)
            {
                for (int col = 0; col < config.GetLength(1); ++col)
                {
                    if (config[row, col] == true)
                    {
                        int boardRow = row + shapeComponent.CurrentRow;
                        int boardCol = col + shapeComponent.CurrentColumn;

                        // Invalid board position or board is occupied.
                        if (!(boardRow >= 0 && boardRow < board.NumRows) ||
                            !(boardCol >= 0 && boardCol < board.NumColumns) ||
                            board.IsOccupied(boardRow, boardCol))
                        {
                            // Invalid.
                            return false;
                        }
                    }
                }
            }

            return true;
        }
    }

//     public class GhostShape : Component
//     {
// 
//     }
}