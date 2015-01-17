package hope;

import java.awt.Graphics;
import java.awt.Image;
import javax.vecmath.*;

public class Sprite {
	Image mImage;
	int mX = 0, mY = 0;
	String mFileName;
	
	public Sprite(String fileName) {
		mFileName = fileName;
	}
	
	public boolean load() {
		SpriteManager spriteManager = SpriteManager.getInstance();
		
		try {
			mImage = spriteManager.loadSprite(mFileName);
			// verify image is not null
		}
		catch (Exception e) {
			return false;
		}
		
		// Get active renderer reference.
		SpriteRenderer renderer = spriteManager.getRenderer();
		if (renderer == null) {
			return false;
		}
		
		// Add to active renderer.
		renderer.addToRenderQueue(this);
		
		return true;
	}
	
	public void unload() {
		SpriteManager spriteManager = SpriteManager.getInstance();
		
		// Get active renderer reference.
		SpriteRenderer renderer = spriteManager.getRenderer();
		if (renderer == null) {
			return;
		}
		
		// Remove from active renderer.
		renderer.removeFromRenderQueue(this);
	}
	
//	public Vector2f getPosition() {
//		return mPosition;
//	}
	public void setPosition(int x, int y) {
		mX = x;
		mY = y;
	}
	
	public void draw(Graphics g) {
		g.drawImage(mImage, mX, mY, null);
	}
}
