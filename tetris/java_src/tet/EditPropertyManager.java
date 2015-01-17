package tet;

import java.awt.Color;
import java.awt.Point;
import java.awt.event.KeyEvent;

import javax.vecmath.Vector2f;

import hope.Actor;
import hope.GameObject;
import hope.InputManager;
import hope.Text2D;

public class EditPropertyManager extends Actor {
	private static EditPropertyManager mSingleton;
	public static EditPropertyManager getInstance() {
		assert mSingleton != null;
		return mSingleton;
	}
	
	private GameObject mEditingEnemy;
	public void setEditingEnemy(GameObject e) {
		mEditingEnemy = e;
		
		mDelay = new Text2D();
		mDelayVal = new Text2D();
		mRotation = new Text2D();
		mRotationVal = new Text2D();
	}
	public void unsetEditingEnemy(GameObject e) {
		if (mEditingEnemy == e) {
			mEditingEnemy = null;
			
			mDelay.kill();
			mDelayVal.kill();
			mRotation.kill();
			mRotationVal.kill();
		}
	}
	
	public EditPropertyManager(String actorLevel) {
		super(actorLevel);
		
		assert mSingleton == null;
		mSingleton = this;
	}
	
	private int mCurrentIndex = 0;

	private Text2D mDelay;
	private Text2D mDelayVal;
	private Text2D mRotation;
	private Text2D mRotationVal;
	
	private Color getColor(int index) {
		if (index == mCurrentIndex) {
			return Color.red;
		}
		else {
			return Color.black;
		}
	}
	
	protected void update(float timeStep) {
		
		// up/down arrows to change index
		// left/right to increase/decrease
		
		if (mEditingEnemy == null) {
			return;
		}
		
		EnemyLogicComponent logic = (EnemyLogicComponent)mEditingEnemy.getComponent("Logic");
		
		// Input
		if (InputManager.isKeyDownFirst(KeyEvent.VK_UP)) {
			mCurrentIndex--;
		}
		else if (InputManager.isKeyDownFirst(KeyEvent.VK_DOWN)) {
			mCurrentIndex++;
		}
		
		int maxIndex = 1;
		if (mCurrentIndex < 0) {
			mCurrentIndex = maxIndex;
		}
		else if (mCurrentIndex > maxIndex) {
			mCurrentIndex = 0;
		}
		
		// Value input
		if (InputManager.isKeyDownFirst(KeyEvent.VK_LEFT)) {
			if (mCurrentIndex == 0) {
				logic.mDelay -= .1f;
				if (logic.mDelay < 0) {
					logic.mDelay = 0;
				}
			}
			else if (mCurrentIndex == 1) {
				logic.mRotation--;
				if (logic.mRotation < 0) {
					logic.mRotation += 360;
				}
			}
		}
		if (InputManager.isKeyDownFirst(KeyEvent.VK_RIGHT)) {
			if (mCurrentIndex == 0 ) {
				logic.mDelay += .1f;
			}
			else if (mCurrentIndex == 1) {
				logic.mRotation++;
				if (logic.mRotation > 360) {
					logic.mRotation -= 360;
				}
			}
		}	
		
		// Display
		mDelay.setText("Delay:");
		mDelay.setPosition(new Point(5, 100));
		mDelay.setColor(getColor(0));
		mDelayVal.setText(Float.toString(logic.mDelay));
		mDelayVal.setPosition(new Point(5, 115));
		mDelayVal.setColor(getColor(0));
		
		mRotation.setText("Rotation:");
		mRotation.setPosition(new Point(5, 145));
		mRotation.setColor(getColor(1));
		mRotationVal.setText(Integer.toString(logic.mRotation));
		mRotationVal.setPosition(new Point(5, 160));
		mRotationVal.setColor(getColor(1));
	}
}
