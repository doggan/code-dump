namespace Engine
{
    public interface IModule
    {
        string Name { get; }

        void Init();
        void End();
    }
}
