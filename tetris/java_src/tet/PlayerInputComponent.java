package tet;

import java.awt.event.KeyEvent;

import javax.vecmath.Vector2f;

import hope.Component;
import hope.IUpdateable;
import hope.InputManager;

public class PlayerInputComponent extends Component implements IUpdateable {

	public PlayerInputComponent(String name) {
		super(name);
	}

	@Override
	public void preUpdate(float timeStep) {
		Vector2f movement = new Vector2f(0, 0);
		float moveAmount = 100;
		
		if (InputManager.isKeyDown(KeyEvent.VK_A)) {
			movement.add(new Vector2f(-moveAmount, 0));
		}
		if (InputManager.isKeyDown(KeyEvent.VK_D)) {
			movement.add(new Vector2f(moveAmount, 0));
		}
		if (InputManager.isKeyDown(KeyEvent.VK_W)) {
			movement.add(new Vector2f(0, -moveAmount));
		}
		if (InputManager.isKeyDown(KeyEvent.VK_S)) {
			movement.add(new Vector2f(0, moveAmount));
		}
		
		movement.scale(timeStep);
		
		PlayerBodyComponent body = (PlayerBodyComponent)getGameObject().getComponent("Body");
		
		movement.add(body.getPosition());
		body.setPosition(movement);
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
