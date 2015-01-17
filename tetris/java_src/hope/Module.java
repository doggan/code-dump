package hope;

public abstract class Module {
	private String name;
	
	public String getName() {
		return this.name;
	}

	public Module(String name) {
		this.name = name;
	}
	
	protected void initialize() { }
	protected void shutdown() { }
}
