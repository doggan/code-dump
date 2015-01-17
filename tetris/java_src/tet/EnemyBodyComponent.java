package tet;

import java.awt.Color;
import java.awt.Point;

import javax.vecmath.Vector2f;

import hope.Component;
import hope.Sprite;
import hope.Text2D;

public class EnemyBodyComponent extends Component {

	private Sprite mSprite;
	private Point mPosition = new Point();
	private Text2D mEditingMode;
	
	private Point mCenterOffset = new Point(15, -5);
	private double mRadiusSq = 20 * 20;
	
	public Point getCenterOffset() {
		return mCenterOffset;
	}
	public Point getCenter() {
		Point center = (Point)mPosition.clone();
		center.translate(mCenterOffset.x, mCenterOffset.y);
		return center;
	}
	
	public double getRadiusSq() {
		return mRadiusSq;
	}

	public void setPosition(Point pos) {
		pos.translate(-mCenterOffset.x, -mCenterOffset.y);
		mPosition = pos;
		mSprite.setPosition(pos.x, pos.y);
		
		// update position
		if (mEditingMode != null) {
			mEditingMode.setPosition(getCenter());
		}
	}
	
	public void kill() {
		mSprite.unload();
	}
	
	public void setEditing(boolean state) {
		if (state) {
			mEditingMode = new Text2D();
			mEditingMode.setPosition(getCenter());
			mEditingMode.setColor(Color.red);
			mEditingMode.setText("E");
			
			EditPropertyManager.getInstance().setEditingEnemy(getGameObject());
		}
		else {
			mEditingMode.kill();
			mEditingMode = null;
			
			EditPropertyManager.getInstance().unsetEditingEnemy(getGameObject());
		}
	}
		
	public EnemyBodyComponent(String name) {
		super(name);
		
		mSprite = new Sprite("contents/blockRed.png");
		mSprite.load();
	}
	
}
