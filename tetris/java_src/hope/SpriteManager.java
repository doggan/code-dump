package hope;

import java.awt.GraphicsConfiguration;
import java.awt.GraphicsEnvironment;
import java.awt.Image;
import java.awt.Transparency;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;

import javax.imageio.ImageIO;

import hope.GraphicsModule;

public class SpriteManager {
	private GraphicsModule mGraphicsModule;
	
	// TODO: needs to use weak reference. override sprite::finalize to remove image from cache
	private HashMap<String, Image> mSprites = new HashMap<String, Image>();
	
	public SpriteRenderer getRenderer() {
		return mGraphicsModule.getRenderer();
	}
	
	private static SpriteManager mSingleton;
	public static SpriteManager getInstance() {
		assert mSingleton != null;
		return mSingleton;
	}
	
	private SpriteManager(GraphicsModule graphicsModule) {
		assert mSingleton == null;
		assert graphicsModule != null;
		
		mSingleton = this;
		mGraphicsModule = graphicsModule;
	}
	
	public static void initialize(GraphicsModule graphicsModule) {
		new SpriteManager(graphicsModule);
	}
	
	public static void shutdown() {
		mSingleton = null;
	}
	
	Image loadSprite(String fileName) {
		// Check cache.
		if (mSprites.get(fileName) != null) {
			return mSprites.get(fileName);
		}
		
		// Not in cache, so perform loading.
		BufferedImage sourceImage = null;

		try {
			// Load image.
		    sourceImage = ImageIO.read(new File(fileName));
		} catch (IOException e) {
			return null;
		}
		
		GraphicsConfiguration gc = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice().getDefaultConfiguration();
		Image image = gc.createCompatibleImage(sourceImage.getWidth(), sourceImage.getHeight(), Transparency.BITMASK);
		
		// Draw source image into accelerated image.
		image.getGraphics().drawImage(sourceImage,0,0,null);
		
		// Add to cache.
		mSprites.put(fileName, image);
		
		return image;
	}
}
