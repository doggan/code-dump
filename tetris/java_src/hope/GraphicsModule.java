package hope;

public class GraphicsModule extends Module {
	private SpriteRenderer mRenderer;
	public SpriteRenderer getRenderer() {
		return mRenderer;
	}
	
	public GraphicsModule(String moduleName) {
		super(moduleName);
	}
	
	protected void initialize() {
		mRenderer = new SpriteRenderer(Application.getInstance().getWindow());
		
		SpriteManager.initialize(this);
	}
	
	protected void shutdown() {
		SpriteManager.shutdown();
	}
}
