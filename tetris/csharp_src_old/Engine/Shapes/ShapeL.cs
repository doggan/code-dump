using System;
using System.Collections.Generic;
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
    //public class ShapeL : IShape
    //{
    //    #region Shape configurations: http://tetris.wikia.com/wiki/SRS
    //    private static List<bool[,]> _configs = new List<bool[,]>
    //    {
    //        // 0
    //        new bool[3,3]
    //        {
    //            { false,  false, true },
    //            { true,  true,  true  },
    //            { false, false, false }
    //        },
    //        // 1
    //        new bool[3,3]
    //        {
    //            { false, true, false },
    //            { false, true, false },
    //            { false, true, true  }
    //        },
    //        // 2
    //        new bool[3,3]
    //        {
    //            { false, false, false },
    //            { true,  true,  true  },
    //            { true,  false, false }
    //        },
    //        // 3
    //        new bool[3,3]
    //        {
    //            { true,  true, false },
    //            { false, true, false },
    //            { false, true, false }
    //        }
    //    };
    //    #endregion

    //    public override bool[,] GetConfiguration(int index)
    //    {
    //        Core.Assert(index < _configs.Count);
    //        return _configs[index];
    //    }

    //    public ShapeL(Board board)
    //        : base(board, 4)
    //    {

    //    }
    //}

//     public class TestShapeComponent : IComponent
//     {
//         private string _name;
// 
//         public string Name { get { return _name; } }
// 
//         public TestShapeComponent(string name)
//         {
//             _name = name;
//         }
// 
//         public void ReceiveMessage(ref IMessage message)
//         {
// 
//         }
//     }
}
