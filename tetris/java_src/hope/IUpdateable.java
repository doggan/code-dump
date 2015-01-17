package hope;

public interface IUpdateable {
	public void preUpdate(float timeStep);
	public void update(float timeStep);
	public void postUpdate(float timeStep);
}
