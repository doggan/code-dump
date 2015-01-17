package hope;
public abstract class Component {

	private String name;
	private GameObject gameObject;
	
	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
	
	public GameObject getGameObject() {
		return gameObject;
	}

	public void setGameObject(GameObject gameObject) {
		this.gameObject = gameObject;
	}

	public Component(String name) {
		this.name = name;
	}
	
	void load() {
		// TODO: timer logic
		onLoad();
	}
	
	void unload() {
		// TODO: timer logic
		onUnload();
	}

	/**
	 * Called when a component is added to a game object.
	 */
	protected void onLoad() {
		
	}
	
	/**
	 * Called when a component is removed from a game object.
	 */
	protected void onUnload() {
		
	}
	 
	protected void onReceiveMessage(Message message) {
		
	}

}
