package hope;
import java.util.ArrayList;

import com.google.common.collect.Lists;

final public class GameObject extends Actor {
	/** This GameObject's name. */
	private String name;
	/** All Components belonging to this GameObject. */
	private ArrayList<Component> mComponents = Lists.newArrayList();
	/** Components added this frame which need to be loaded. */
	private ArrayList<Component> mAddedComponents = Lists.newArrayList();
	/** Components removed this frame which need to be unloaded. */
	private ArrayList<Component> mRemovedComponents = Lists.newArrayList();
	
	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public GameObject(String name, String actorLevel) {
		super(actorLevel);
		this.name = name;
	}
	
	public void addComponent(Component component) {
		if (getComponent(component.getName()) == null) {
			mComponents.add(component);
			mAddedComponents.add(component);
			return;
		}
		
		System.out.println(String.format("Component [%s] already exists", component.getName()));
	}
	
	public void removeComponent(String name) {
		Component c = getComponent(name);
		if (c != null) {
			mComponents.remove(c);
			mRemovedComponents.add(c);
			return;
		}
		
		System.out.println(String.format("Component [%s] not found", name));
	}
	
	public Component getComponent(String name) {
		for (Component c : this.mComponents) {
			if (c.getName().equals(name)) {
				return c;
			}
		}
		
		return null;
	}

	// TODO:
//	public <T> T Test() {
//		for (Component c : this.components) {
//			if (c.getClass().getn) {
//				
//			}
//		}
//		return null;
//	}
	
	public void sendMessage(Message m) {
		for (Component c : this.mComponents) {
			c.onReceiveMessage(m);
		}
	}
	
	protected void update(float timeStep) {
		// Process added components.
		if (!mAddedComponents.isEmpty()) {
			for (Component c : mAddedComponents) {
				c.setGameObject(this);
				c.load();
			}
			mAddedComponents.clear();
		}
		
		// Process removed components.
		if (!mRemovedComponents.isEmpty()) {
			for (Component c : mRemovedComponents) {
				c.unload();
				c.setGameObject(null);
			}
			mRemovedComponents.clear();
		}
		
		// Pre-Update.
		for (Component c : this.mComponents) {
			if (c instanceof IUpdateable) {
				((IUpdateable) c).preUpdate(timeStep);
			}
		}

		// Update.
		for (Component c : this.mComponents) {
			if (c instanceof IUpdateable) {
				((IUpdateable) c).update(timeStep);
			}
		}
		
		// Post-Update.
		for (Component c : this.mComponents) {
			if (c instanceof IUpdateable) {
				((IUpdateable) c).postUpdate(timeStep);
			}
		}
	}
}
