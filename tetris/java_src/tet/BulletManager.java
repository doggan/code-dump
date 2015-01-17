package tet;

import java.util.ArrayList;

import javax.vecmath.Vector2f;

import com.google.common.collect.Lists;

import hope.Actor;
import hope.GameObject;

public class BulletManager extends Actor {
	private static BulletManager mSingleton;
	public static BulletManager getInstance() {
		assert mSingleton != null;
		return mSingleton;
	}
	
	private ArrayList<Bullet> mBullets = Lists.newArrayList();
	
	public void add(Bullet b) {
		if (!mBullets.contains(b)) {
			mBullets.add(b);
		}
	}
	
	public BulletManager(String actorLevel) {
		super(actorLevel);
		
		assert mSingleton == null;
		mSingleton = this;
	}
	
	protected void update(float timeStep) {
		for (int i = 0; i < mBullets.size(); /*++i*/) {
			Bullet b = mBullets.get(i);
			
			// prune dead bullets
			if (b.IsDead()) {
				b.onCleanup();
				mBullets.remove(i);
				continue;
			}
			
			Vector2f pos = b.getPosition();
			if (pos.x < 0 || pos.x > 800 ||
				pos.y < 0 || pos.y > 800) {
				b.onCleanup();
				mBullets.remove(i);
				continue;
			}
			
			// todo: other checks
			
			++i;
		}
	}
}
