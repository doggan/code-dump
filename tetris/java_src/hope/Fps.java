package hope;

public class Fps {
	private int mFrameCount = 0;
	
	private int mFramesPerSecond = 0;
	public int getFramesPerSecond() {
		return mFramesPerSecond;
	}
	
	private Stopwatch mStopwatch = new Stopwatch();
	
	public Fps() {
		mStopwatch.start();
	}
	
	/**
	 * Call once per-frame to record a single tick.
	 */
	public void tick() {
		mFrameCount++;
		
		if (mStopwatch.getElapsedTime() > 1000) {
			mFramesPerSecond = mFrameCount;
			mFrameCount = 0;
			
			mStopwatch.restart();
		}
	}
}
