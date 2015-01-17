package tet;

import java.awt.Point;
import java.util.ArrayList;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.common.collect.Lists;

import hope.Actor;
import hope.Component;
import hope.GameObject;
import hope.InputManager;

public class EnemyManager extends Actor {
	private static EnemyManager mSingleton;
	public static EnemyManager getInstance() {
		assert mSingleton != null;
		return mSingleton;
	}
	
	private final Logger mLogger = LoggerFactory.getLogger(EnemyManager.class);
	
	private ArrayList<GameObject> mEnemies = Lists.newArrayList();
	
	private GameObject mEnemyForEditing;
	private GameObject mEnemyForMoving;
	
	public EnemyManager(String actorLevel) {
		super(actorLevel);
		
		assert mSingleton == null;
		mSingleton = this;
	}
	
	private int selectEnemy(Point point) {
		for (int i = 0; i < mEnemies.size(); i++) {
			EnemyBodyComponent body = (EnemyBodyComponent)mEnemies.get(i).getComponent("Body");
			double dist = body.getCenter().distanceSq(point);
			
			if (dist < body.getRadiusSq()) {
				mLogger.info("dist {}", dist);				
				return i;
			}
		}
		
		return -1;
	}
	
	protected void update(float timeStep) {		
		if (InputManager.isButtonDownFirst(InputManager.MouseButton.RIGHT_BUTTON)) {
			Point clickPoint = InputManager.getMouseLocation();
			
			int clickedOnEnemy = selectEnemy(clickPoint);
			
			// if click on enemy, delete
			if (clickedOnEnemy != -1) {
				mLogger.info("delete enemy");
				
				GameObject deletedEnemy = mEnemies.get(clickedOnEnemy);
				EnemyBodyComponent body = (EnemyBodyComponent)deletedEnemy.getComponent("Body");

				// delete enemy
				if (mEnemyForMoving != null &&
					mEnemyForMoving == deletedEnemy) {
					mEnemyForMoving = null;
				}
				if (mEnemyForEditing != null &&
					mEnemyForEditing == deletedEnemy) {
					body.setEditing(false);
					mEnemyForEditing = null;
				}
				body.kill();
				mEnemies.remove(clickedOnEnemy);
				deletedEnemy.Kill();
			}
			else {
				mLogger.info("spawn enemy");
				
				// spawn enemy
				GameObject enemy = new GameObject("Enemy", "Enemy");
				enemy.addComponent(new EnemyBodyComponent("Body"));
				enemy.addComponent(new EnemyLogicComponent("Logic"));
				mEnemies.add(enemy);
				
				EnemyBodyComponent body = (EnemyBodyComponent)enemy.getComponent("Body");
				body.setPosition(clickPoint);
			}				
		}
		
		if (InputManager.isButtonDownFirst(InputManager.MouseButton.LEFT_BUTTON)) {
			Point clickPoint = InputManager.getMouseLocation();
			
			int clickedEnemy = selectEnemy(clickPoint);
			if ( clickedEnemy != -1 ) {
				mLogger.info("set enemy as active");
								
				// set enemy moving
				mEnemyForMoving = mEnemies.get(clickedEnemy);
				
				// set enemy for editing
				if (mEnemyForEditing != null) {
					// stop editing previous enemy
					EnemyBodyComponent body = (EnemyBodyComponent)mEnemyForEditing.getComponent("Body");
					body.setEditing(false);
				}
				mEnemyForEditing = mEnemies.get(clickedEnemy);
				EnemyBodyComponent body = (EnemyBodyComponent)mEnemyForEditing.getComponent("Body");
				body.setEditing(true);
			}
			else {
				// stop editing
				if (mEnemyForEditing != null) {
					EnemyBodyComponent body = (EnemyBodyComponent)mEnemyForEditing.getComponent("Body");
					body.setEditing(false);
					mEnemyForEditing = null;
				}
			}
		}
		else if (InputManager.isButtonDown(InputManager.MouseButton.LEFT_BUTTON)) {
			// moving an enemy
			if (mEnemyForMoving != null) {
				mLogger.info("moving enemy...");
				
				Point clickPoint = InputManager.getMouseLocation();
				
				EnemyBodyComponent body = (EnemyBodyComponent)mEnemyForMoving.getComponent("Body");
				body.setPosition(clickPoint);
			}
		}
		else if (InputManager.isButtonUp(InputManager.MouseButton.LEFT_BUTTON)) {
			// inactivate any moving enemy
			if (mEnemyForMoving != null) {
				mLogger.info("stop moving enemy");
				mEnemyForMoving = null;
			}
		}
	}

}
