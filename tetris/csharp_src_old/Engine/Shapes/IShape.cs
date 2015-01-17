using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace Engine.Shapes
{
    public abstract class IShape
    {
        private BoardComponent _board;
        private Block[] _blocks;

        private const int _maxConfigurations = 4;
        private int _currentConfiguration = 0;

        public abstract bool[,] GetConfiguration(int index);

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

        private int _currentRow = 0;
        private int _currentColumn = 0;

        public IShape(GameObject board, uint numBlocks)
        {
            _board = board.GetComponent<BoardComponent>();
            //_board = board;

            _blocks = new Block[numBlocks];
            for (uint i = 0; i < numBlocks; ++i)
            {
                _blocks[i] = new Block();
            }
        }

        public void Rotate()
        {
            _currentConfiguration++;
            if (_currentConfiguration == _maxConfigurations)
            {
                _currentConfiguration = 0;
            }
        }

        public void Draw()
        {
            bool[,] config = GetConfiguration(_currentConfiguration);
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
    }
}
