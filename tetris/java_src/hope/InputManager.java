package hope;

import java.awt.MouseInfo;
import java.awt.Point;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

public class InputManager {
	private static InputManager mSingleton;
	public static InputManager getInstance() {
		assert mSingleton != null;
		return mSingleton;
	}
	
	private enum KeyState {
		KEY_DOWN,
		KEY_UP,
		KEY_DOWN_FIRST
	}
	
	private enum MouseState {
		BUTTON_DOWN,
		BUTTON_UP,
		BUTTON_DOWN_FIRST
	}
	
	public enum MouseButton {
		LEFT_BUTTON,
		MIDDLE_BUTTON,
		RIGHT_BUTTON
	}
	
	private final int NUM_KEY_STATES = 256;
	private KeyState[] mKeyStates = new KeyState[NUM_KEY_STATES];
	private final int NUM_MOUSE_STATES = 3;
	private MouseState[] mMouseStates = new MouseState[NUM_MOUSE_STATES];
	
	private Window mWindow;
	
	private InputManager(Window window) {
		assert mSingleton == null;
		
		mSingleton = this;
		
		// Initialize to up.
		for (int i = 0; i < mKeyStates.length; i++) {
			mKeyStates[i] = KeyState.KEY_UP;
		}
		for (int i = 0; i < mMouseStates.length; i++) {
			mMouseStates[i] = MouseState.BUTTON_UP;
		}
		
		mWindow = window;
		KeyEventListener kel = new KeyEventListener(this);
		mWindow.getFrame().addKeyListener(kel);
		mWindow.getFrame().addMouseListener(kel);
	}
	
	public static void initialize() {
		new InputManager(Application.getInstance().getWindow());
	}
	
	public static void shutdown() {
		mSingleton = null;
	}
	
	public static boolean isKeyDown(int keyCode) {
		InputManager instance = getInstance();
		if (keyCode >= 0 && keyCode < instance.NUM_KEY_STATES) {
			if (instance.mKeyStates[keyCode].equals(KeyState.KEY_DOWN) ||
				instance.mKeyStates[keyCode].equals(KeyState.KEY_DOWN_FIRST)) {
				return true;
			}
		}
		
		return false;
	}
	
	public static boolean isKeyDownFirst(int keyCode) {
		InputManager instance = getInstance();
		if (keyCode >= 0 && keyCode < instance.NUM_KEY_STATES) {
			if (instance.mKeyStates[keyCode].equals(KeyState.KEY_DOWN_FIRST)) {
				return true;
			}
		}
		
		return false;
	}
	
	public static boolean isKeyUp(int keyCode) {
		InputManager instance = getInstance();
		if (keyCode >= 0 && keyCode < instance.NUM_KEY_STATES) {
			if (instance.mKeyStates[keyCode].equals(KeyState.KEY_UP)) {
				return true;
			}
		}
		
		return false;
	}
	
	public static boolean isButtonDown(MouseButton button) {
		InputManager instance = getInstance();
		int omg = 0;
		if (button.equals(MouseButton.MIDDLE_BUTTON)) {
			omg = 1;
		}
		else if (button.equals(MouseButton.RIGHT_BUTTON)) {
			omg = 2;
		}
		if (instance.mMouseStates[omg].equals(MouseState.BUTTON_DOWN)) {
			return true;
		}
		
		return false;
	}
	
	public static boolean isButtonDownFirst(MouseButton button) {
		InputManager instance = getInstance();
		int omg = 0;
		if (button.equals(MouseButton.MIDDLE_BUTTON)) {
			omg = 1;
		}
		else if (button.equals(MouseButton.RIGHT_BUTTON)) {
			omg = 2;
		}
		if (instance.mMouseStates[omg].equals(MouseState.BUTTON_DOWN_FIRST)) {
			return true;
		}
		
		return false;
	}
	
	public static boolean isButtonUp(MouseButton button) {
		InputManager instance = getInstance();
		int omg = 0;
		if (button.equals(MouseButton.MIDDLE_BUTTON)) {
			omg = 1;
		}
		else if (button.equals(MouseButton.RIGHT_BUTTON)) {
			omg = 2;
		}
		if (instance.mMouseStates[omg].equals(MouseState.BUTTON_UP)) {
			return true;
		}
		
		return false;
	}
	
	public static Point getMouseLocation() {
		Point p = MouseInfo.getPointerInfo().getLocation();
		p.translate(-1, -46);
		return p;
	}
	
	private class KeyEventListener extends Actor implements KeyListener, MouseListener {
		InputManager mInputManager;
		
		public KeyEventListener(InputManager inputManager) {
			super("Input");
			
			mInputManager = inputManager;
		}

		protected void update(float timeStep) {
			// Update key states.
			for (int i = 0; i < NUM_KEY_STATES; i++) {
				if (mInputManager.mKeyStates[i].equals(KeyState.KEY_DOWN_FIRST)) {
					mInputManager.mKeyStates[i] = KeyState.KEY_DOWN;
				}
			}
			
			// Update mouse states.
			for (int i = 0; i < NUM_MOUSE_STATES; i++) {
				if (mInputManager.mMouseStates[i].equals(MouseState.BUTTON_DOWN_FIRST)) {
					mInputManager.mMouseStates[i] = MouseState.BUTTON_DOWN;
				}
			}
		}
		
		public void keyTyped(KeyEvent e) {
//			displayInfo(e, "KEY TYPED: ");
		}
		
		public void keyPressed(KeyEvent e) {
			int keyCode = e.getKeyCode();
			if (keyCode >= 0 && keyCode < NUM_KEY_STATES) {
				mKeyStates[keyCode] = KeyState.KEY_DOWN_FIRST;
			}
		}
		
		public void keyReleased(KeyEvent e) {
			int keyCode = e.getKeyCode();
			if (keyCode >= 0 && keyCode < NUM_KEY_STATES) {
				mKeyStates[keyCode] = KeyState.KEY_UP;
			}
		}

		@Override
		public void mouseClicked(MouseEvent arg0) {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void mouseEntered(MouseEvent arg0) {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void mouseExited(MouseEvent arg0) {
			// TODO Auto-generated method stub
			
		}

		@Override
		public void mousePressed(MouseEvent arg0) {
			int button = arg0.getButton() - 1;
			if (button >= 0 && button < NUM_MOUSE_STATES) {
				mMouseStates[button] = MouseState.BUTTON_DOWN_FIRST;
			}
		}

		@Override
		public void mouseReleased(MouseEvent arg0) {
			int button = arg0.getButton() - 1;
			if (button >= 0 && button < NUM_MOUSE_STATES) {
				mMouseStates[button] = MouseState.BUTTON_UP;
			}
		}
	}
}
