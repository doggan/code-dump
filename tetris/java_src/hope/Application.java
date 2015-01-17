package hope;

import java.awt.Color;
import java.util.ArrayList;

import javax.vecmath.Vector2f;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import com.google.common.collect.Lists;

public abstract class Application {
	private ArrayList<Module> modules = new ArrayList<Module>();
	
	private static Application mSingleton;
	public static Application getInstance() {
		assert mSingleton != null;
		return mSingleton;
	}
	
	private Window mWindow;
	public Window getWindow() {
		return mWindow;
	}
	
	private ActorCollector actorCollector = new ActorCollector();
	ActorCollector getActorCollector() {
		return actorCollector;
	}
	
	protected void registerActorLevel(String actorLevel) {
		mLogger.info("Register Actor Level: {}", actorLevel);
		this.actorCollector.registerActorLevel(actorLevel);
	}
	
	private float timeStep;
	public float getTimeStep() {
		return timeStep;
	}
	
	private Fps mFps = new Fps();
	private Text2D mFpsText;
	
	public Application() {
		assert mSingleton == null;
		mSingleton = this;
	}
	
	private final Logger mLogger = LoggerFactory.getLogger(Application.class);
	
	protected void onInitialize() { }
	protected void onInitializeFinished() { }
	protected void onShutdown() { }
	
	private void initialize() {
		mLogger.info("Initialize...");
		
		// Register engine modules.
		addModule(new CoreModule("Core"));
		addModule(new GraphicsModule("Graphics"));
	//	addModule(new GameObjectModule("GameObject"));
		
		// Register actor levels (pre-User).
		registerActorLevel("GameObject");
		
		// Initialize application.
		onInitialize();
		
		// Register actor levels (post-User).
		registerActorLevel("Input");
		registerActorLevel("Renderer");
		
		// Freeze actor levels.
		this.actorCollector.freeze();
		
		// Window creation.
		mWindow = new Window();

		mLogger.info("Initialize modules...");
		
		// Module initialization.
		for (Module m : modules) {
			mLogger.info("{} - initialize()", m.getName());
			m.initialize();
		}
		
		// Fps display setup.
		mFpsText = new Text2D();
		mFpsText.setPosition(new Vector2f(10, 40));
		mFpsText.setColor(Color.red);
	}
	
	private void shutdown() {
		mLogger.info("Shutdown...");
		
		onShutdown();
		
		mLogger.info("Shutdown modules...");
		
		// Module shutdown.	
		for (Module m : Lists.reverse(modules)) {
			mLogger.info("{} - shutdown()", m.getName());
			m.shutdown();
		}
		
		// TODO: better way to do this?
		mWindow.cleanup();
	}
	
	public final void run() {
		initialize();
		
		onInitializeFinished();
		
		long lastLoopTime = System.currentTimeMillis();
		
		// Game loop.
		while (true) {
			// Check for exit requests.
			if (isExitRequested()) {
				break;
			}
			
			long currentLoopTime = System.currentTimeMillis();
			timeStep = (currentLoopTime - lastLoopTime) / 1000.0f;
			lastLoopTime = currentLoopTime;
			
			// Step.
			executeStep(timeStep);
			
			// Sleep.
			try { Thread.sleep(10); }
			catch (Exception e) { }
		}
		
		shutdown();
	}
	
	// TODO: better way to do this
	private boolean isRequestingExit = false;
	public void requestExit() {
		isRequestingExit = true;
	}
	
	private boolean isExitRequested() {
//		if (window.isRequestingClose()) {
//			return true;
//		}
		
//		return false;
		
		return isRequestingExit;
	}
	
	private void executeStep(float timeStep) {
		mFps.tick();
		mFpsText.setText(String.format("Fps: %d", mFps.getFramesPerSecond()));
		
		// Actor update.
		actorCollector.update(timeStep);
	}
	
	protected void addModule(Module module) {
		assert doesModuleExist(module) == false : "Module already added.";
		modules.add(module);
	}
	
	private boolean doesModuleExist(Module module) {
		return modules.contains(module);
	}
}
