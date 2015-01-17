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
    public abstract class Module : IModule, IUpdateable, IDisposable
    {
        public string Name { get { return _name; } }
        public bool IsEnabled { get; set; }
        public int UpdateOrder { get; set; }

        private string _name;

        public Module(string name)
        {
            _name = name;
        }

        public virtual void Init() { }
        public virtual void End() { }
        
        public virtual void Dispose() { }

        public virtual void Update(float time) { }
    }
}
