package hope;

public class CoreModule extends Module {
	public CoreModule(String moduleName) {
		super(moduleName);
	}
	
	protected void initialize() {
		InputManager.initialize();
	}
	
	protected void shutdown() {
		
	}
}
