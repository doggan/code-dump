//package tet;
//
//import java.lang.ref.WeakReference;
//
//import hope.Component;
//import hope.GameObject;
//import hope.IUpdateable;
//
//public class BasicBoardInputComponent extends Component implements IUpdateable {
//	private WeakReference<BasicBoardLogicComponent> mBoardLogic;
//	
//	public BasicBoardInputComponent(String name) {
//		super(name);
//		
//	}
//	
//	protected void onLoad() {
//		GameObject go = getGameObject();
//		mBoardLogic = new WeakReference<BasicBoardLogicComponent>((BasicBoardLogicComponent) go.getComponent("Board"));
//	}
//
//	@Override
//	public void preUpdate(float timeStep) {
//		// TODO Auto-generated method stub
//		
//	}
//
//	@Override
//	public void update(float timeStep) {
//		// TODO Auto-generated method stub
//		
//	}
//
//	@Override
//	public void postUpdate(float timeStep) {
//		// TODO Auto-generated method stub
//		
//	}
//
//}
