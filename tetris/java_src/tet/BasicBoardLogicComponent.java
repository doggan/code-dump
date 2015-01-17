//package tet;
//
//import java.awt.event.KeyEvent;
//import java.util.Random;
//
//import javax.vecmath.Vector2f;
//
//import tet.PieceComponent.BlockType;
//
//import hope.Actor;
//import hope.Component;
//import hope.GameObject;
//import hope.IUpdateable;
//import hope.InputManager;
//import hope.Sprite;
//
//public class BasicBoardLogicComponent extends Component implements IUpdateable {
//	private Sprite mBoardImg;
//	private BasicBoardSettings mSettings = new BasicBoardSettings();
//	
//	final private int mCellWidth = 32;
//	final private int mCellHeight = 32;
//	
//	/** Width of a single cell. */
//	public int getCellWidth() {
//		return mCellWidth;
//	}
//	
//	/** Height of a single cell. */
//	public int getCellHeight() {
//		return mCellHeight;
//	}
//	
//	private GameObject[][] mBoardState = new GameObject[10][22];
//	
//	private GameObject mActivePiece;
//	public GameObject getActivePiece() {
//		return mActivePiece;
//	}
//	
//	public BasicBoardLogicComponent(String name) {
//		super(name);
//		
//		mBoardImg = new Sprite("contents/board.png");
//		mBoardImg.load();
//		//mBoardImg.setPosition(new Vector2f(50,50));
//	}
//	
//	@Override
//	public void preUpdate(float timeStep) {
//		// TODO Auto-generated method stub
//		
//	}
//	
//	public void update(float timeStep) {
//		if (mActivePiece == null) {
//			spawnActivePiece();
//		}
//		
//		/*
//		
//		else {
//			
//			// for bottom most cell of each column of active piece
//				//if below row is occupied or out of bounds, stop the piece
//			
//			// Piece stoppage.
//			PieceComponent piece = (PieceComponent)mActivePiece.getComponent("Piece");
//			boolean isStopped = false;
//			for (int col = 0; col < piece.mConfiguration.length; col++) {
//				if (isStopped) {
//					break;
//				}
//				for (int row = piece.mConfiguration[col].length - 1; row >= 0; row--) {
//					if (piece.mConfiguration[col][row].equals(BlockType.StandardBlock)) {
//						
//						int x = piece.mBoardX + col;
//						int y = piece.mBoardY + row;
//						
//						if (isOccupied(x, y + 1)) {
//							stopActivePiece();
//							isStopped = true;
//						}
//						
//						break;	// check next column
//					}
//				}
//			}
//		}
//		
//		// line clear logic
//		
//		// start from last line and work up
//		// if line is completely filed, null it, shift all above rows down.. resume check on next line up
//		
//		for (int checkLine = 21; checkLine >= 0; ) {//checkLine--) {
//			boolean allFilled = true;
//			for (int col = 0; col < 10; col++) {
//				if (!this.isOccupied(col,checkLine)) {
//					allFilled = false;
//					break;
//				}
//			}
//			
//			if (allFilled) {
//				clearLine(checkLine);
//	//			System.out.println("Clear line: " + checkLine);
//				
//				// shift all down
//	//			for (int row = checkLine; row > 0; row--) {	// skip topmost row
//	//				for (int col = 0; col < 10; col++) {
//	//					this.mBoardState[col][row] = this.mBoardState[col][row - 1];
//	//				}
//	//			}
//			}
//			else {
//				checkLine--;
//			}
//		}
//		*/
//	}
//	
//	@Override
//	public void postUpdate(float timeStep) {
//		// TODO Auto-generated method stub
//		
//	}
//	
//	/*
//	private void clearLine(int line) {
//		for (int col = 0; col < 10; col++) {
//			GameObject go = this.mBoardState[col][line];
//			PieceComponent piece = (PieceComponent)go.getComponent("Piece");
//			piece.clear(col, line);
//			this.mBoardState[col][line] = null;
//		}
//	}
//	*/
//	
//	private void spawnActivePiece() {
//		// TODO: validate that the piece can actually be placed - if not, game over
//		
//		mActivePiece = PieceFactory.createRandomPiece();
//		
//		// pieces should be oriented properly alrdy... place it on board
//		
//		
//
//		
////		mActivePiece.sendMessage(new SetAsActivePieceMessage(this, 3, 0));
//	}
//	
//	private void stopActivePiece() {
//		// update board state
//		PieceComponent piece = (PieceComponent)mActivePiece.getComponent("Piece");
//		for (int col = 0; col < piece.mConfiguration.length; col++) {
//			for (int row = 0; row < piece.mConfiguration[col].length; row++) {
//				if (piece.mConfiguration[col][row] == BlockType.StandardBlock) {
//					int x = piece.mBoardX + col;
//					int y = piece.mBoardY + row;
//					mBoardState[x][y] = mActivePiece;					
//				}
//			}
//		}		
//		
//		// Clear
//		mActivePiece = null;
//	}
//	
//	public boolean isOccupied(int col, int row) {
//		return col < 0 || col > 9 || row > 21 || row < 0 || this.mBoardState[col][row] != null;
//	}
//}
