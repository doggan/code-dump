package hope;

public abstract class Actor {
	private String actorLevel;
	
	public String getActorLevel() {
		return actorLevel;
	}
	
	public Actor(String actorLevel) {
		this.actorLevel = actorLevel;
		
		// Register.
		Application.getInstance().getActorCollector().registerActor(this);
	}
	
	private boolean mIsKilled = false;
	public boolean IsKilled() {
		return mIsKilled;
	}
	public void Kill() {
		mIsKilled = true;
	}
	
	protected void finalize() throws Throwable {
		try {
			// Unregister.
//			Application.getInstance().getActorCollector().unregisterActor(this);
		}
		finally {
			super.finalize();
		}
	}
	
	protected void update(float timeStep) { }
}
