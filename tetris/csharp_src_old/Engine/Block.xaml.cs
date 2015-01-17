using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace Engine
{
    public partial class Block : UserControl
    {
        public Block()
        {
            InitializeComponent();

            //border1.Background = new SolidColorBrush(Colors.Blue);
            //border1.Margin = new Thickness(1);
            //border1.BorderThickness = new Thickness(1);
            //border1.BorderBrush = new SolidColorBrush(Colors.Red);
            //border1.Width = 50;
            //border1.Height = 50;
            //border1.CornerRadius = new CornerRadius(5);
        }
    }

    /*
     * X Board is a grid with rows and columns
     * - Shapes can be created, which contain Blocks. Shapes know how to orient their blocks, and assign colors
     *      and textures to the blocks.
     * - Shapes have a position on the grid.
     * - As the shape moves, it updates it's blocks.
     *      - shapes make sure they don't move into an invalid region or off the board
     * - Board keeps track of grid cells (isOccupied)
     * - when a shape becomes inactive (hits the ground), it no longer responds to control.
     * - it's blocks are managed by the Board until the Board is cleared.
     * - for standard tetris, there should be BoardLogic classes that can be plugged in and are
     *      updated whenever the board changes. They can detect filled lines, etc and clear the board
     *      and update scores.
     */
}
