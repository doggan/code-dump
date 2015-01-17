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

namespace Engine
{
    public abstract class DrawableModule : Module, IDrawable
    {
        public bool IsVisible { get; set; }
        public int DrawOrder { get; set; }

        public DrawableModule(string name) :
            base(name) { }

        public virtual void Draw(float time) { }
    }
}
