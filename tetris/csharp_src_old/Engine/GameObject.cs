using System;
using System.Collections.Generic;

namespace Engine
{
    /// <summary>
    /// A delegate used for updating.
    /// </summary>
    /// <param name="dt">Time in seconds since last tick.</param>
    public delegate void TickHandler(float dt);

    public sealed class GameObject
    {
        private string _name;
        private List<Component> _components = new List<Component>();

        private event TickHandler _ticked;

        public string Name { get { return _name; } }

        public GameObject(string name)
        {
            _name = name;
        }

        public void AddComponent(Component component)
        {
            if (GetComponent(component.Name) != null)
            {
                Core.Assert("Component named [%s] already exists.", component.Name);
                return;
            }

            _components.Add(component);

            // Set owner game object.
            component.Owner = this;
            component.OnInitCompleted();
        }

        public Component GetComponent(string name)
        {
            foreach (Component c in _components)
            {
                if (c.Name == name)
                {
                    return c;
                }
            }

            return null;
        }

        public T GetComponent<T>() where T : Component
        {
            foreach (Component c in _components)
            {
                if (c is T)
                {
                    return (T)c;
                }
            }

            return null;
        }

        public void SendMessage(IMessage message)
        {
            foreach (Component c in _components)
            {
                c.ReceiveMessage(ref message);
            }
        }

        public void AddTicked(TickHandler tick)
        {
            _ticked += tick;
        }

        public void Update(float dt)
        {
            if (_ticked != null)
            {
                _ticked(dt);
            }
        }

        // Tick 
        // Update
    }
}
