package hope;

import java.util.ArrayList;

final class ActorCollector {
	private ArrayList<String> actorLevels = new ArrayList<String>();
	private ArrayList<ArrayList<Actor>> actorLists = new ArrayList<ArrayList<Actor>>();
	
	private boolean isFrozen = false;
	void freeze() {
		isFrozen = true;
	}
	
	ActorCollector() {
		
	}
	
	void registerActorLevel(String actorLevel) {
		assert !isFrozen;
		assert !isActorLevelRegistered(actorLevel);
		
		// Register.
		this.actorLevels.add(actorLevel);
		this.actorLists.add(new ArrayList<Actor>());
		
		assert this.actorLevels.size() == this.actorLists.size();
	}
	
	private boolean isActorLevelRegistered(String actorLevel) {
		for (String al : actorLevels) {
			if (al.equals(actorLevel)) {
				return true;
			}
		}
		
		return false;
	}
	
	private int getActorLevelIndex(String actorLevel) {
		return actorLevels.indexOf(actorLevel);
	}
	
	void registerActor(Actor actor) {
		assert isFrozen;
		
		String actorLevel = actor.getActorLevel();
		assert isActorLevelRegistered(actorLevel);
		
		// Get existing list.
		int actorLevelIndex = getActorLevelIndex(actorLevel);
		ArrayList<Actor> actors = actorLists.get(actorLevelIndex);
		assert actors != null;
		
		// Register.
		actors.add(actor);
	}
	
	void unregisterActor(Actor actor) {
		assert isFrozen;
		
		String actorLevel = actor.getActorLevel();
		assert isActorLevelRegistered(actorLevel);
		
		// Get existing list.
		int actorLevelIndex = getActorLevelIndex(actorLevel);
		ArrayList<Actor> actors = actorLists.get(actorLevelIndex);
		assert actors != null;
		
		// Unregister.
		actors.remove(actor);
	}
	
	void update(float timeStep) {
		assert isFrozen;
		
		// Actor update.
		for (ArrayList<Actor> actorList : this.actorLists) {
			for (int i = 0; i < actorList.size(); /*i++*/) {
				Actor actor = actorList.get(i);
				
				// Cleanup.
				if (actor.IsKilled()) {
					actorList.remove(i);
					continue;
				}

				actor.update(timeStep);
				i++;
			}
		}
	}
}
