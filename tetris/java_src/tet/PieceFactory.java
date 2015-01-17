//package tet;
//
//import hope.GameObject;
//import hope.Sprite;
//
//import java.util.Random;
//
//public class PieceFactory {
//	private static Random mRandom = new Random();
//	
//	public static GameObject createRandomPiece() {
//		int val = 0;//mRandom.nextInt(6);
//		
//		GameObject go = new GameObject("Piece", "Piece");
//		
//		switch (val) {
//			case 0:
//				go.addComponent(PieceFactory.createJPiece());
//				break;
///*			case 1:
//				go.addComponent(PieceFactory.createLPiece());
//				break;
//			case 2:
//				go.addComponent(PieceFactory.createOPiece());
//				break;
//			case 3:
//				go.addComponent(PieceFactory.createSPiece());
//				break;
//			case 4:
//				go.addComponent(PieceFactory.createTPiece());
//				break;
//			case 5:
//				go.addComponent(PieceFactory.createZPiece());
//				break;
//				*/
////			case 6:
////				go.addComponent(PieceFactory.createIPiece());
////				break;
//		}
//		
//		return go;
//	}
//	
//	private static Sprite makeSprite(String fileName) {
//		Sprite s = new Sprite(fileName);
//		s.load();
//		return s;
//	}
//	
//	public static PieceComponent createJPiece() {
//		String fileName = "contents/blockBlue.png";
//		
//		PieceConfiguration config = new PieceConfiguration(3, 3);
//		config.setPivotPoint(1, 1);
//		
//		/* Initial configuration:
//		 *  1 0 0
//		 *  1 1 1
//		 *  0 0 0
//		 */
//		config.setCell(0, 0, makeSprite(fileName));
//		config.setCell(0, 1, makeSprite(fileName));
//		config.setCell(1, 1, makeSprite(fileName));
//		config.setCell(2, 1, makeSprite(fileName));
//		
//		return new PieceComponent("Piece", config);
//	}
//	
////	public static PieceComponent createLPiece() {
////		PieceComponentDesc desc = new PieceComponentDesc(3,3);
////		
////		/*
////		 *  0 0 1
////		 *  1 1 1
////		 *  0 0 0
////		 */
////		desc.mConfiguration[2][0] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[0][1] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[1][1] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[2][1] = PieceComponent.BlockType.StandardBlock;
////		
////		desc.mBlockFileName = "contents/blockOrange.png";
////		
////		return new PieceComponent("Piece", desc);
////	}
////	
////	public static PieceComponent createOPiece() {
////		PieceComponentDesc desc = new PieceComponentDesc(2,2);
////		
////		/*
////		 *  1 1
////		 *  1 1
////		 */
////		desc.mConfiguration[0][0] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[1][0] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[0][1] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[1][1] = PieceComponent.BlockType.StandardBlock;
////		
////		desc.mBlockFileName = "contents/blockYellow.png";
////		
////		return new PieceComponent("Piece", desc);
////	}
////	
////	public static PieceComponent createSPiece() {
////		PieceComponentDesc desc = new PieceComponentDesc(3,3);
////		
////		/*
////		 *  0 1 1
////		 *  1 1 0
////		 *  0 0 0
////		 */
////		desc.mConfiguration[1][0] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[2][0] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[0][1] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[1][1] = PieceComponent.BlockType.StandardBlock;
////		
////		desc.mBlockFileName = "contents/blockGreen.png";
////		
////		return new PieceComponent("Piece", desc);
////	}
////	
////	public static PieceComponent createTPiece() {
////		PieceComponentDesc desc = new PieceComponentDesc(3,3);
////		
////		/*
////		 *  0 1 0
////		 *  1 1 1
////		 *  0 0 0
////		 */
////		desc.mConfiguration[1][0] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[0][1] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[1][1] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[2][1] = PieceComponent.BlockType.StandardBlock;
////		
////		desc.mBlockFileName = "contents/blockPurple.png";
////		
////		return new PieceComponent("Piece", desc);
////	}
////	
////	public static PieceComponent createZPiece() {
////		PieceComponentDesc desc = new PieceComponentDesc(3,3);
////		
////		/*
////		 *  1 1 0
////		 *  0 1 1
////		 *  0 0 0
////		 */
////		desc.mConfiguration[0][0] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[1][0] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[1][1] = PieceComponent.BlockType.StandardBlock;
////		desc.mConfiguration[2][1] = PieceComponent.BlockType.StandardBlock;
////		
////		desc.mBlockFileName = "contents/blockRed.png";
////		
////		return new PieceComponent("Piece", desc);
////	}
////	
////	public static PieceComponent createIPiece() {
////		return null;
////	}
//}
