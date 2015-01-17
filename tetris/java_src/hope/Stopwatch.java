package hope;

public class Stopwatch {
	private long mStartTime;
	private long mStopTime;
	
	private boolean mIsRunning = false;
	public boolean isRunning() {
		return mIsRunning;
	}
	
	public Stopwatch() {
		reset();
	}
	
	/**
	 * Starts, or resumes, measuring elapsed time.
	 */
	public void start() {
		mStartTime = System.currentTimeMillis();
		mIsRunning = true;
	}
	
	/**
	 * Stops measuring elapsed time.
	 */
	public void stop() {
		mStopTime = System.currentTimeMillis();
		mIsRunning = false;
	}
	
	/**
	 * Stops time interval measurement and resets the elapsed time to zero.
	 */
	public void reset() {
		stop();
		
		mStartTime = 0;
		mStopTime = 0;
	}
	
	/**
	 * Stops time interval measurement, resets the elapsed time to zero,
	 * and starts measuring elapsed time.
	 */
	public void restart() {
		reset();
		start();
	}
	
	/**
	 * Get the elapsed time in milliseconds.
	 * @return elapsed time in ms
	 */
	public long getElapsedTime() {
		if (mIsRunning) {
			return System.currentTimeMillis() - mStartTime;
		}
		else {
			return mStopTime - mStartTime;
		}
	}

	/**
	 * Get the elapsed time in seconds.
	 * @return elapsed time in s
	 */
	public long getElapsedTimeInSeconds() {
		return getElapsedTime() / 1000;
	}
}
