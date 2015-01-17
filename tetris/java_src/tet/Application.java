package tet;

import hope.Actor;
import hope.GameObject;

public class Application extends hope.Application {
	/* TETRIS:
	GameObject mPlayer;
	GameObject mTestPiece;
	
	//Actor mBoard;
	GameObject mBoard;
	
	protected void onInitialize() {
		// Actor level registration.
		this.registerActorLevel("Board");
		this.registerActorLevel("Piece");
		
		// Module registration.
	}
	
	protected void onInitializeFinished() {
		/* TETRIS:
		mPlayer = new GameObject("Player", "Player");
		mPlayer.addComponent(new InputComponent("Input"));
		// mPlayer.addComponent( scoring? )
		
//		mBoard = new BasicBoard();
		mBoard = new GameObject("Board", "Board");
		mBoard.addComponent(new BasicBoardLogicComponent("Board"));
		mBoard.addComponent(new BasicBoardInputComponent("Input"));

	//	mTestPiece = new GameObject("Piece", "GameObject");
	//	mTestPiece.addComponent(PieceFactory.createLPiece());
		//mPlayer.addComponent(PieceFactory.createLPiece());
//		mPlayer.addComponent(PieceFactory.createTPiece());
		
//		GameObject go = new GameObject();
//		go.addComponent(new PieceComponent("test"));
//		go.sendMessage(new TestMessage());
		
		// TetrinominoFactory
	}
	*/
	
	private GameObject mPlayer;
	
	protected void onInitialize() {
		this.registerActorLevel("Editor");
		this.registerActorLevel("BulletManager");
		this.registerActorLevel("EnemyManager");
		this.registerActorLevel("Player");
		this.registerActorLevel("Enemy");
		this.registerActorLevel("Bullet");
	}
	
	protected void onInitializeFinished() {
		new EditPropertyManager("Editor");
		new BulletManager("BulletManager");
		new EnemyManager("EnemyManager");
		
		mPlayer = new GameObject("Player", "Player");
		mPlayer.addComponent(new PlayerBodyComponent("Body"));
		mPlayer.addComponent(new PlayerInputComponent("Input"));
	}
	
	protected void onShutdown() {
		
	}
}
