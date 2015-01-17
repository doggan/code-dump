package hope;

import java.awt.Graphics;

public abstract class Primitive {
	public Primitive() {
		// Register with renderer.
		SpriteManager spriteManager = SpriteManager.getInstance();
		SpriteRenderer renderer = spriteManager.getRenderer();
		renderer.addToRenderQueue(this);
	}
	
	public void kill() {
		// Unregister with renderer.
		SpriteManager spriteManager = SpriteManager.getInstance();
		SpriteRenderer renderer = spriteManager.getRenderer();
		renderer.removeFromRenderQueue(this);
	}
	
	abstract void draw(Graphics g);
}
