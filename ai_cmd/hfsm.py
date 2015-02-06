class State(object):
	def __init__(self, stateId, \
				 onEnter = None, onUpdate = None, onExit = None):
		self.stateId = stateId
		self.parent = None
		self.childStates = []
		self.currentChildState = None

		# Events.
		self.onEnter = onEnter
		self.onUpdate = onUpdate
		self.onExit = onExit

	def addChild(self, childState):
		assert(childState)
		assert(not childState.parent)

		if childState in self.childStates:
			raise Exception("Attempting to add duplicate child state [" + childState + "].")

		self.childStates.append(childState)
		childState.parent = self

	def doEnter(self, eventHandler):
		# Trigger callback.
		if eventHandler and self.onEnter:
			self.onEnter(eventHandler)

		# Activate top child state.
		if len(self.childStates) != 0:
			self.currentChildState = self.childStates[0]

		# Enter child state.
		if self.currentChildState:
			self.currentChildState.doEnter(eventHandler);

	def doUpdate(self, stateMachine, eventHandler):
		# Initialize on first call to update.
		if not self.currentChildState and len(self.childStates) != 0:
			self.doEnter(eventHandler)
			assert(self.currentChildState)

		# Trigger callback.
		if eventHandler and self.onUpdate:
			newStateId = self.onUpdate(eventHandler)

			# Change state?
			if newStateId and newStateId != self.stateId:
				assert(self.hasParent())	# all states that can request a state change will have a parent
				self.parent.changeChildState(stateMachine, eventHandler, newStateId)
				return

		# Update child.
		if self.currentChildState:
			self.currentChildState.doUpdate(stateMachine, eventHandler)

	def doExit(self, eventHandler):
		# Exit child state.
		if self.currentChildState:
			self.currentChildState.doExit(eventHandler);

		# Clear topic child state.
		self.currentChildState = None

		# Trigger callback.
		if eventHandler and self.onExit:
			self.onExit(eventHandler)

	def hasParent(self):
		return self.parent != None

	def hasChild(self):
		return len(self.childStates) != 0

	def changeChildState(self, stateMachine, eventHandler, newChildStateId):
		assert(self.currentChildState)
		currentChildStateId = self.currentChildState.stateId

		# Valid transition?
		if not stateMachine.isValidTransition(currentChildStateId, newChildStateId):
			raise Exception("Invalid state transition: [" + currentChildStateId + "] -> [" + newChildStateId + "].")

		# Leave previous state.
		self.currentChildState.doExit(eventHandler)

		# Enter new state.
		self.currentChildState = stateMachine.getState(newChildStateId)
		assert(self.currentChildState)
		self.currentChildState.doEnter(eventHandler)

	def __str__(self):
		return self.stateId

class StateMachineDesc(object):
	def __init__(self):
		self.stateMap = {}
		self.states = []
		self.transitionMap = {}

	def addState(self, stateId, parentId = None, \
				 onEnter = None, onUpdate = None, onExit = None):
		assert(stateId)

		if parentId and parentId not in self.stateMap:
			raise Exception("Invalid parent [" + parentId + "].")
		if stateId in self.stateMap:
			raise Exception("Attempting to add duplicate state[" + stateId + "].")

		# Create the new state.
		state = State(stateId,\
					  onEnter, onUpdate, onExit)
		self.stateMap[stateId] = state
		self.states.append(state)

		#todo: guard against circular dependencies~
		if parentId:
			parent = self.stateMap[parentId]
			parent.addChild(state)

		# Add empty transition entry for this state.
		assert(not self.transitionMap.get(stateId))
		self.transitionMap[stateId] = []

	def addTransition(self, fromStateId, toStateId):
		if fromStateId not in self.stateMap:
			raise Exception("Invalid fromStateId [" + fromStateId + "].")
		if toStateId not in self.stateMap:
			raise Exception("Invalid toStateId [" + toStateId + "].")

		# Get the "from" state entries.
		fromStateTransitions = self.transitionMap.get(fromStateId)
		assert(fromStateTransitions != None)

		if toStateId in fromStateTransitions:
			raise Exception("Attempting to add a duplicate transition: [" + fromStateId + "] -> [" + toStateId + "].")

		# Add the transition for this "to" state.
		fromStateTransitions.append(toStateId)

class StateMachine(object):
	def __init__(self, smDesc):
		assert(smDesc)

		self.smDesc = smDesc
		self.rootState = None

		self._build()

	def update(self, eventHandler):
		if self.rootState:
			self.rootState.doUpdate(self, eventHandler)

	def getState(self, stateId):
		return self.smDesc.stateMap.get(stateId)

	def isValidTransition(self, fromStateId, toStateId):
		fromStateTransitions = self.smDesc.transitionMap.get(fromStateId)
		return fromStateTransitions and toStateId in fromStateTransitions

	def dumpStates(self):
		if not self.rootState:
			print "<empty>"
			return

		stack = [(self.rootState, 0)]	# Tuple of the state and it's indent depth.
		while len(stack) > 0:
			currentState, currentDepth = stack.pop()

			# Don't print root state.
			if currentState is not self.rootState:
				# Is state currently active?
				if currentState.hasParent() and currentState.parent.currentChildState is currentState:
					print "  " * (currentDepth - 1) + "->[" + str(currentState) + "]"
				else:
					print "  " * currentDepth + "[" + str(currentState) + "]"		
			
			if len(currentState.childStates) > 0:
				newDepth = currentDepth + 1
				for state in reversed(currentState.childStates):
					stack.append((state,newDepth))

	def _build(self):
		if len(self.smDesc.states) == 0:
			print "Empty StateMachineDesc."
			return

		# Internal root state.
		self.rootState = State("root")

		# Add all states w/o parents to the root state.
		for state in self.smDesc.states:
			if not state.hasParent():
				self.rootState.addChild(state)