package tet;

import javax.vecmath.Vector2f;

import hope.Component;
import hope.Sprite;

public class PlayerBodyComponent extends Component {

	private Sprite mSprite;
	private Vector2f mPosition = new Vector2f();
	
	public Vector2f getPosition() {
		return mPosition;
	}
	
	public void setPosition(Vector2f pos) {
		mPosition = pos;
		mSprite.setPosition((int)mPosition.x, (int)mPosition.y);
	}
	
	public PlayerBodyComponent(String name) {
		super(name);
		
		mPosition = new Vector2f(100, 100);
		
		mSprite = new Sprite("contents/blockBlue.png");
		mSprite.load();
		mSprite.setPosition((int)mPosition.x, (int)mPosition.y);
	}

}
