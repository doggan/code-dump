namespace Engine
{
    public interface IUpdateable
    {
        bool IsEnabled { get; }
        int UpdateOrder { get; }

        void Update(float time);
    }
}
