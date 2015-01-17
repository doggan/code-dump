package tet;

public class BasicBoardSettings {
	/** Amount of time for a piece to fall 1 row. */
	private float mFallRate;
	
	public void setFallRate(float fallRate) {
		mFallRate = fallRate;
	}
	
	public float getFallRate() {
		return mFallRate;
	}
	
	public BasicBoardSettings() {
		setFallRate(2);
	}
}
