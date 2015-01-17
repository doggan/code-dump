//package tet;
//
//import java.awt.event.KeyEvent;
//import java.lang.ref.WeakReference;
//import java.util.ArrayList;
//
//import javax.vecmath.Vector2f;
//
//import com.google.common.collect.Lists;
//
//import hope.*;
//
//public class PieceComponent extends Component implements IUpdateable {
////	private WeakReference<BasicBoardLogicComponent> mBoard;
//	private int mBoardX, mBoardY;
//	
//	private ArrayList<Sprite> mSprites = Lists.newArrayList();
//	
//	private PieceConfiguration mConfiguration;
//	
//	public PieceComponent(String name, PieceConfiguration config) {
//		super(name);
//		
//		mConfiguration = config;
//		
////		onPositionChanged();
//	}
//	
//	// isValidPosition( config, absCoord)
//	
//	// create piece and rotate it dynamically as rotation messages are received
//	// when all blocks of a piece are destroyed, piece can destroy itself
//	// 
//	
//	public void preUpdate(float timeStep) {
//		// TODO: get weak ref to board
//		BasicBoardLogicComponent mBoard;
//		
//		// Update the configuration.
//		mConfiguration.setAbsoluteCellPosition(mBoardX, mBoardY, mBoard.getCellWidth(), mBoard.getCellHeight());
//
//		/*
//		if (mBoard == null || mBoard.getActivePiece() != this.getGameObject()) {
//			return;
//		}
//		
////		Vector2f movement = new Vector2f(0, 0);
//		
////		float moveAmount = 32;
//		
//		if (InputManager.isKeyDownFirst(KeyEvent.VK_LEFT)) {
//			if (isValidPosition(mConfiguration, mBoardX - 1, mBoardY)) {
//				mBoardX -= 1;
////				movement.add(new Vector2f(-moveAmount, 0));
//			}
//		}
//		if (InputManager.isKeyDownFirst(KeyEvent.VK_RIGHT)) {
//			if (isValidPosition(mConfiguration, mBoardX + 1, mBoardY)) {
//				mBoardX += 1;
//			}
////			movement.add(new Vector2f(moveAmount, 0));
//		}
//		if (InputManager.isKeyDownFirst(KeyEvent.VK_UP)) {
////			movement.add(new Vector2f(0, -moveAmount));
//			rotate();
//		}
//		if (InputManager.isKeyDownFirst(KeyEvent.VK_DOWN)) {
//			if (isValidPosition(mConfiguration, mBoardX, mBoardY + 1)) {
//				mBoardY += 1;
//			}
////			movement.add(new Vector2f(0, moveAmount));
//		}
//		
//	//	movement.scale(timeStep);
//		
////		PieceComponent p = (PieceComponent)getGameObject().getComponent("Piece");
////		movement.add(p.getPosition());
////		p.setPosition(movement);
////		setPosition(new Vector2f(10 + mBoardX * 32, mBoardY * 32));
//		this.onPositionChanged();
//		*/
//	}
//	
//	private void rotate() {
//		if (this.mConfiguration.length % 2 == 0) {
//			// even
//		}
//		else {
//			// odd
//			
//			// coordinates
//			// swap x and y
//			// negate y
//			
//			BlockType newConfig[][] = new BlockType[3][3];//mConfiguration.clone();
//			newConfig[1][1] = mConfiguration[1][1];
//			
//			newConfig[2][0] = mConfiguration[0][0];
//			newConfig[2][1] = mConfiguration[1][0];
//			newConfig[2][2] = mConfiguration[2][0];
//			newConfig[1][2] = mConfiguration[2][1];
//			newConfig[0][2] = mConfiguration[2][2];
//			newConfig[0][1] = mConfiguration[1][2];
//			newConfig[0][0] = mConfiguration[0][2];
//			newConfig[1][0] = mConfiguration[0][1];
//			
//			if (isValidPosition(newConfig, mBoardX, mBoardY)) {
//				mConfiguration = newConfig;
//			}
//		}
//	}
//	
//	private boolean isValidPosition(BlockType config[][], int x, int y) {
//		for (int col = 0; col < config.length; col++) {
//			for (int row = 0; row < config[col].length; row++) {
//				int boardX = x + col;
//				int boardY = y + row;
//				
//				// if cell is out of board range, it must be empty
//				if (boardX < 0 || boardX > 9 || boardY > 21) {
//					if (!config[col][row].equals(BlockType.NoBlock)) {
//						return false;
//					}
//				}
//				else {
//					// if cell is in board range and not empty, board cell must be empty
//					if (!config[col][row].equals(BlockType.NoBlock)) {
//						if (mBoard.isOccupied(boardX, boardY)) {
//							return false;
//						}
//					}
//				}
//			}
//		}
//		
//		return true;
//	}
//	
//	public void clear(int col, int row) {
//		int x = col - mBoardX;
//		int y = row - mBoardY;
//		this.mConfiguration[x][y] = BlockType.NoBlock;
//		this.onPositionChanged();
//	}
//	
//	private void onPositionChanged() {
//		// Update positions of all sprites...
//		
//// pos is world offset to top left corner
//		
//		int spriteNum = 0;
//		for (int col = 0; col < mConfiguration.length; col++) {
//			for (int row = 0; row < mConfiguration[col].length; row++) {
//				if (mConfiguration[col][row].equals(BlockType.StandardBlock)) {
//					//Vector2f newPosition = (Vector2f)mPosition.clone();
//					//newPosition.add(new Vector2f(col * 32, row * 32));
//					Vector2f pos = new Vector2f(10 + (mBoardX + col) * 32, (mBoardY + row) * 32);
//					mSprites.get(spriteNum).setPosition(pos);
//					spriteNum++;
//				}
//			}
//		}
//		
//		// TODO: when cell is cleared (during line clear) force excess sprites to delete
//		for (int i = spriteNum; i < this.mSprites.size(); i++) {
//			this.mSprites.get(i).unload();
//		}
//	}
//	
//	// how does gravity/game update work?
//	
//	// game board is an actor
//	// keeps track of all active pieces on board
//	// 
//	
//	protected void onReceiveMessage(Message message) {
//		// handle rotation, positioning, sprite
//		if (message instanceof SetAsActivePieceMessage) {
//			SetAsActivePieceMessage m = (SetAsActivePieceMessage)message;
//			mBoard = m.mBoard;
//			mBoardX = m.mX;
//			mBoardY = m.mY;
//			this.onPositionChanged();
//		}
//	}
//	@Override
//	public void update(float timeStep) {
//		// TODO Auto-generated method stub
//		
//	}
//	@Override
//	public void postUpdate(float timeStep) {
//		// TODO Auto-generated method stub
//		
//	}
//}
