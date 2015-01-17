package hope;

import java.awt.Dimension;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.image.BufferStrategy;

import javax.swing.JFrame;

public class Window {
	private boolean isRequestingClose = false;
	public boolean isRequestingClose() {
		return isRequestingClose;
	}
	
	private BufferStrategy bufferStrategy;
	public BufferStrategy getBufferStrategy() {
		return bufferStrategy;
	}
	
	private JFrame mFrame;
	public JFrame getFrame() {
		return mFrame;
	}
	
	public Window() {
		mFrame = new JFrame("Main Window");
		
		mFrame.setPreferredSize(new Dimension(840,840));
		mFrame.setIgnoreRepaint(true);
		mFrame.pack();
		mFrame.setResizable(false);
		mFrame.setVisible(true);
		
		mFrame.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				// Window shutdown.
				java.awt.Window w = e.getWindow();
				w.setVisible(false);
				w.dispose();
				
				isRequestingClose = true;
				Application.getInstance().requestExit();
				System.out.println("Window Close");
			}
		});
		
		mFrame.requestFocus();
		mFrame.createBufferStrategy(5);
		this.bufferStrategy = mFrame.getBufferStrategy();
	}
	
	public void cleanup() {
		// Window shutdown.
//		java.awt.Window w = mFrame.getOwner();
		//java.awt.Window w = e.getWindow();
//		w.setVisible(false);
//		w.dispose();

//		isRequestingClose = true;
//		Application.getInstance().requestExit();
//		System.out.println("Window Close");
	}
}

/*
import org.lwjgl.LWJGLException;
import org.lwjgl.input.Keyboard;
import org.lwjgl.input.Mouse;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;
//import org.newdawn.spaceinvaders.GameWindow;
//import org.newdawn.spaceinvaders.GameWindowCallback;

public class ... {
	private void setTitle(String title) {
		mTitle = title;
		if (Display.isCreated()) {
			Display.setTitle(mTitle);
		}
	}

	private init() {
		// setDisplayMode()
		Display.create();

		Mouse.setGrabbed(true);

		GL11.glEnable(GL11.GL_TEXTURE_2D);

		GL11.glDisable(GL11.GL_DEPTH_TEST);

		GL11.glMatrixMode(GL11.GL_PROJECTION);
		GL11.glLoadIdentity();

		GL11.glOrtho(0. width, height, 0, -1, 1);


	}
}
*/