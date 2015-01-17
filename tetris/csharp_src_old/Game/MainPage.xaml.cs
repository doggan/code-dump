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
using Engine;

namespace Game
{
    public partial class MainPage : UserControl
    {
        private Storyboard _gameLoop = new Storyboard();

//        private List<Module> _modules = new List<Module>();
  //      private List<DrawableModule> _drawableModules = new List<DrawableModule>();

        private GameObject _board;

        public MainPage()
        {
            InitializeComponent();

            // TODO: engine level
            new Engine.GameObjectManager();

            _board = new GameObject("Board");
            _board.AddComponent(new BoardComponent("Body"));
            _board.AddComponent(new BoardInputComponent("Input"));
            this.LayoutRoot.Children.Add(_board.GetComponent<BoardComponent>()._control);

            GameObjectManager.AddGameObject(_board);

            _gameLoop.Duration = TimeSpan.FromMilliseconds(0);
            _gameLoop.Completed += new EventHandler(Update);
            _gameLoop.Begin();

            

            //border1.Background = new SolidColorBrush(Colors.Blue);
            //border1.Margin = new Thickness(1);
            //border1.BorderThickness = new Thickness(1);
            //border1.BorderBrush = new SolidColorBrush(Colors.Red);
            //border1.Width = 50;
            //border1.Height = 50;
            //border1.CornerRadius = new CornerRadius(5);

            //Canvas.SetLeft(rectangle2, 32);

            this.KeyDown += new KeyEventHandler(OnKeyDown);
        }

        private void Update(object sender, EventArgs e)
        {
            //_board._activeShape.Draw();
//            _board.Draw();

            GameObjectManager.Update();

            _gameLoop.Begin();

            // sort modules, update
            
            // sort drawable modules, draw
        }
        
        public void AddModule(Module module)
        {
            Core.Assert(module != null);

            //foreach (Module m in _modules)
            //{
            //    if (m.Name == module.Name)
            //    {
            //        Core.Assert("Module with same name [" + m.Name + "] already exists.");
            //        return;
            //    }
            //}

            //if (module is DrawableModule)
            //{
            //    foreach (Module m in _drawableModules)
            //    {
            //        if (m.Name == module.Name)
            //        {
            //            Core.Assert("DrawableModule with same name [" + m.Name + "] already exists.");
            //            return;
            //        }
            //    }
            //}

 //           _modules.Add(module);
            
      //      if (module is DrawableModule)
      //      {
      //          _drawableModules.Add((DrawableModule)module);
        //    }
        }

        private void OnKeyDown(object sender, KeyEventArgs e)
        {
            KeyDownMessage message;
            message.Sender = sender;
            message.KeyEventArgs = e;
            _board.SendMessage(message);
            /*
            switch (e.Key)
            {
                case Key.Left:
                    {
                        MoveMessage m = new MoveMessage();
                        m.Direction = MoveMessage.MoveDirection.LEFT;
                        _board.SendMessage(m);
                    }
                    break;
                case Key.Right:
                    {
                        MoveMessage m = new MoveMessage();
                        m.Direction = MoveMessage.MoveDirection.RIGHT;
                        _board.SendMessage(m);
                    }
                    break;
                case Key.Down:
                    {
                        MoveMessage m = new MoveMessage();
                        m.Direction = MoveMessage.MoveDirection.DOWN;
                        _board.SendMessage(m);
                    }
                    break;
                case Key.Down:
                    _board.MoveDown();
                    break;
            }*/
        }
    }

    // input is detected, message sent to board, board component receives message and forwards movement to active shape object, active shape performs movement
}
