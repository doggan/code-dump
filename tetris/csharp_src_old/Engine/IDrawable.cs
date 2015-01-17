namespace Engine
{
    public interface IDrawable
    {
        bool IsVisible { get; }
        int DrawOrder { get; }

        void Draw(float time);
    }
}