package tet;

import java.awt.Point;

import javax.vecmath.Vector2f;

import hope.Component;
import hope.IUpdateable;

public class EnemyLogicComponent extends Component implements IUpdateable {
	
	private float cnt = 0;
	public float mDelay = 1;
	
	// per delay
	public int mRotation = 0;
	
	private int mRotAmount = 0;

	public EnemyLogicComponent(String name) {
		super(name);
		// TODO Auto-generated constructor stub
	}

	@Override
	public void preUpdate(float timeStep) {
		EnemyBodyComponent body = (EnemyBodyComponent)getGameObject().getComponent("Body");
		
		cnt += timeStep;
		
		if (cnt > mDelay) {
			// shoot
			Bullet b = new Bullet();
			Point p = body.getCenter();
			p.translate(-body.getCenterOffset().x, -body.getCenterOffset().y);
			b.setPosition( p );
			
			float speed = 30;
			mRotAmount += mRotation;
			
			double theta = Math.toRadians(mRotAmount);
			Vector2f vel = new Vector2f();
			vel.x = (float) (speed * Math.sin(theta));
			vel.y = -(float) (speed * Math.cos(theta));
			b.setVelocity(vel);
			
			cnt = 0;
		}
		
		// fire 1 sprite every mFireRate seconds
			// time is met?
				// create bullet, shoot it.. bullet updates itself

	}

	@Override
	public void update(float timeStep) {
		// TODO Auto-generated method stub

	}

	@Override
	public void postUpdate(float timeStep) {
		// TODO Auto-generated method stub

	}

}
