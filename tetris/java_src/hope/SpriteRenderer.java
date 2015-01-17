package hope;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferStrategy;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import com.google.common.collect.Lists;

public class SpriteRenderer extends Actor {
	private Window window;
	private ArrayList<Sprite> mSprites = new ArrayList<Sprite>();
//	private ArrayList<WeakReference<Primitive>> mPrimitives = Lists.newArrayList();
	private ArrayList<Primitive> mPrimitives = new ArrayList<Primitive>();
	
	public SpriteRenderer(Window window) {
		super("Renderer");
		
		// Grab reference to window.
		this.window = window;
		assert this.window != null;
	}
	
	void addToRenderQueue(Sprite sprite) {
		mSprites.add(sprite);
	}
	
	void removeFromRenderQueue(Sprite sprite) {
		mSprites.remove(sprite);
	}
	
	void addToRenderQueue(Primitive primitive) {
//		mPrimitives.add(new WeakReference<Primitive>(primitive));
		mPrimitives.add(primitive);
	}
	
	void removeFromRenderQueue(Primitive primitive) {
		mPrimitives.remove(primitive);
	}
	
	protected void update(float timeStep) {
		BufferStrategy buffer = window.getBufferStrategy();
		Graphics2D g = (Graphics2D) buffer.getDrawGraphics();
		g.setColor(Color.cyan);
		g.fillRect(0,0,800,600);
		
		// Draw all active sprites.
		for (Sprite s : mSprites) {
			s.draw(g);
		}
		
		// Draw all primitives.
		for (int i = 0; i < mPrimitives.size(); /*i++*/) {
			Primitive p = mPrimitives.get(i);//.get();
//			if (p == null) {
//				mPrimitives.remove(i);
//			}
//			else {
				p.draw(g);
				i++;
//			}
		}
		
		g.dispose();
		buffer.show();
	}
}
