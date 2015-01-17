package tet;

import java.awt.Point;

import javax.vecmath.Vector2f;

import hope.Actor;
import hope.Sprite;

public class Bullet extends Actor {

	private Vector2f mPos;
	private Vector2f mVel;
	
	public void setPosition(Point pos) {
		setPosition(new Vector2f(pos.x, pos.y));
	}
	public Vector2f getPosition() {
		return mPos;
	}
	
	private void setPosition(Vector2f pos) {
		mPos = pos;
		
		mSprite.setPosition((int)mPos.x, (int)mPos.y);
	}
	
	public void setVelocity(Vector2f vel) {
		mVel = vel;
	}
	
	private Sprite mSprite;
	
	private boolean mIsDead = false;
	public boolean IsDead() {
		return mIsDead;
	}
	
	public Bullet() {
		super("Bullet");
		
		mSprite = new Sprite("contents/blockGreen.png");
		mSprite.load();
		
		BulletManager.getInstance().add(this);
	}
	
	// todo: ugh...
	public void onCleanup() {
		mSprite.unload();
	}

	protected void update(float timeStep) {
		
		// pos += vel * time;
		
		Vector2f pos = (Vector2f) mPos.clone();
		Vector2f vel = (Vector2f) mVel.clone();	
		vel.scale(timeStep);
		pos.add(vel);
		
		setPosition(pos);
	}
	
}
