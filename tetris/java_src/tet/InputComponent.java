//package tet;
//
//import javax.vecmath.Vector2f;
//import java.awt.event.KeyEvent;
//
//import hope.Component;
//import hope.IUpdateable;
//import hope.InputManager;
//import hope.Sprite;
//
//public class InputComponent extends Component implements IUpdateable {
//	private Sprite test;
//	
//	public InputComponent(String name) {
//		super(name);
//		
////		test = new Sprite("alien.gif");
////		test.load();
////		test.setPosition(new Vector2f(100, 100));
//	}
//	
//	public void preUpdate(float timeStep) {
//	/*	Vector2f movement = new Vector2f(0, 0);
//		
//		float moveAmount = 32;
//		
//		if (InputManager.isKeyDownFirst(KeyEvent.VK_LEFT)) {
//			movement.add(new Vector2f(-moveAmount, 0));
//		}
//		if (InputManager.isKeyDownFirst(KeyEvent.VK_RIGHT)) {
//			movement.add(new Vector2f(moveAmount, 0));
//		}
//		if (InputManager.isKeyDown(KeyEvent.VK_UP)) {
////			movement.add(new Vector2f(0, -moveAmount));
//			rotate();
//		}
//		if (InputManager.isKeyDownFirst(KeyEvent.VK_DOWN)) {
//			movement.add(new Vector2f(0, moveAmount));
//		}
//		
//	//	movement.scale(timeStep);
//		
//		PieceComponent p = (PieceComponent)getGameObject().getComponent("Piece");
//		movement.add(p.getPosition());
//		p.setPosition(movement);
//		*/
//	}
//
//	public void update(float timeStep) {
//	}
//
//	public void postUpdate(float timeStep) {
//	}
//
//}
