# Your AI for CTF must inherit from the base Commander class.  See how this is
# implemented by looking at the commander.py in the ./api/ folder.
from api import Commander

# The commander can send 'Commands' to individual bots.  These are listed and
# documented in commands.py from the ./api/ folder also.
from api import commands

# The maps for CTF are layed out along the X and Z axis in space, but can be
# effectively be considered 2D.
from api import Vector2

from hfsm import StateMachineDesc, StateMachine
from operator import methodcaller

from inspect import stack

STATE_UNKNOWN   =  0
STATE_IDLE      =  1
STATE_DEFENDING =  2
STATE_MOVING    =  3
STATE_ATTACKING =  4
STATE_CHARGING  =  5
STATE_SHOOTING  =  6
STATE_TAKINGORDERS = 7

# Returns the calling function's name.
def getCurrentFunctionName():
    return stack()[1][3]

class StateMachineEventHandler(object):
    def __init__(self, commander, bot, bb):
        assert(commander)
        assert(bot)
        assert(bb)

        self.cmd = commander
        self.bot = bot
        self.bb = bb

    def captureFlag_onUpdate(self):
        if self.bb.botHasFlag:
            return "returnWithFlag"

 #       if self.bot.state == STATE_MOVING:
  #          print "movin"

    def returnWithFlag_onUpdate(self):
        if not self.bb.botHasFlag:
            return "captureFlag"

    def moveIntoPosition_onUpdate(self):
        if not self.bb.botIsAvailable:
            return

        # Move into position.
        currentRegion = self.cmd.getCurrentRegion(self.bot)
        if currentRegion == Placeholder.REGION_FRIENDLY or\
           currentRegion == Placeholder.REGION_NEUTRAL:
            self.cmd.issue( commands.Move, self.bot, self.cmd.leftFlankPath, description = getCurrentFunctionName() )
        # Go on the offensive.
        else:
            return "performFlagCapture"

    def performCaptureFlag_onUpdate(self):
        if not self.bb.botIsAvailable:
            return

        self.cmd.issue( commands.Attack, self.bot, self.cmd.game.enemyTeam.flagSpawnLocation, description = getCurrentFunctionName() )

    def moveHome_onUpdate(self):
        #todo:
#        if not self.bb.botIsAvailable or self.bot.state == STATE_MOVING):
        if self.bot.state == STATE_MOVING or\
           self.bot.state == STATE_TAKINGORDERS:
            return

        currentRegion = self.cmd.getCurrentRegion(self.bot)
        if currentRegion == Placeholder.REGION_ENEMY:
            path = [self.cmd.getFlankingPosition(self.bot.position, self.cmd.left),
                    self.cmd.getFlankingPosition(self.cmd.middle, self.cmd.left),
                    self.cmd.getFlankingPosition(self.cmd.game.team.flagScoreLocation, self.cmd.left),
                    self.cmd.game.team.flagScoreLocation]
        else:
            path = self.cmd.game.team.flagScoreLocation

        self.cmd.issue(commands.Move, self.bot, path, description = getCurrentFunctionName())

# Creates a state machine object and returns it.
def createStateMachine():
    smDesc = StateMachineDesc()

    # State definitions.
    smDesc.addState("captureFlag",\
        onUpdate = methodcaller("captureFlag_onUpdate"))
    smDesc.addState("returnWithFlag",\
        onUpdate = methodcaller("returnWithFlag_onUpdate"))

    smDesc.addState("moveIntoPosition", "captureFlag",\
        onUpdate = methodcaller("moveIntoPosition_onUpdate"))
    smDesc.addState("performFlagCapture", "captureFlag",\
        onUpdate = methodcaller("performCaptureFlag_onUpdate"))

    smDesc.addState("moveHome", "returnWithFlag",\
        onUpdate = methodcaller("moveHome_onUpdate"))

    # Transition definitions.
    smDesc.addTransition("captureFlag", "returnWithFlag")
    smDesc.addTransition("returnWithFlag", "captureFlag")

    smDesc.addTransition("moveIntoPosition", "performFlagCapture")

    return StateMachine(smDesc)

class Blackboard(object):
    def __init__(self):
        self.botIsAvailable = False
        self.botHasFlag = False

"""
Wraps the bot object and updates our bot logic.
This class is re-used when one bot dies and another bot spawns.
"""
class Entity(object):
    def __init__(self, commander, bot):
        assert(commander)
        assert(bot)

        self.cmd = commander
        self.bot = bot

        self.bb = Blackboard()
        self.stateMachine = createStateMachine()
        self.stateEventHandler = StateMachineEventHandler(self.cmd, self.bot, self.bb)

    def update(self):

        """
        # Move into position.
        if ( self.defendPos - self.bot.position ).length() > 3.0:
            movePos = self.cmdr.level.findNearestFreePosition( self.defendPos )
            self.cmdr.issue( commands.Move, self.bot, movePos, description = "Moving into position." )
        else:
            self.cmdr.issue( commands.Defend, self.bot, self.cmdr.middle, description = "Defending." )
        """

        # Update the blackboard for use this frame.
        self.bb.botIsAvailable = self.bot in self.cmd.game.bots_available
        self.bb.botHasFlag = self.bot == self.cmd.game.enemyTeam.flag.carrier

        # State machine update.
        self.stateMachine.update(self.stateEventHandler)
        #sm.dumpStates()

class Placeholder(Commander):
    """
    Rename and modify this class to create your own commander and add mycmd.Placeholder
    to the execution command you use to run the competition.
    """

    REGION_FRIENDLY = 0
    REGION_NEUTRAL = 1
    REGION_ENEMY = 2

    # Get the bot's current region.
    def getCurrentRegion(self, bot):
        currentRegion = Placeholder.REGION_FRIENDLY
        minVal = ( bot.position - self.game.team.flagSpawnLocation ).length()
        val = ( bot.position - self.middle ).length()
        if val < minVal:
            currentRegion = Placeholder.REGION_NEUTRAL
            minVal = val

        val = ( bot.position - self.game.enemyTeam.flagSpawnLocation ).length()
        if val < minVal:
            currentRegion = Placeholder.REGION_ENEMY

        return currentRegion

    def getFlankingPosition(self, target, flankDir):
        flank = target + flankDir * 26.0    #todo
        return self.level.findNearestFreePosition( flank )

    # Did we capture the enemy flag?
    def didCaptureFlag( self ):
        return self.game.enemyTeam.flag.carrier != None

    def initialize(self):
        self.verbose = True    # display the command descriptions next to the bot labels

        self.entityMap = {}

        # Calculate flag positions and store the middle.
        ours = self.game.team.flagSpawnLocation
        theirs = self.game.enemyTeam.flagSpawnLocation
        self.middle = (theirs + ours) / 2.0

        # Now figure out the flanking directions, assumed perpendicular.
        d = (ours - theirs)
        self.left = Vector2(-d.y, d.x).normalized()
        self.right = Vector2(d.y, -d.x).normalized()
        self.front = Vector2(d.x, d.y).normalized()

        self.leftFlankPath = [
            self.getFlankingPosition(self.middle, self.left),
            self.getFlankingPosition(theirs, self.left)]

        self.leftFlanker = None     #todo: turn into array later; always want bots to move in pairs

    def _onCreateNewEntity(self, bot):
        #todo: role selection logic goes here... maybe
        if len(self.entityMap) > 0:
            return None

        self.entityMap[bot] = Entity(self, bot)

    def tick(self):
        # Prune dead bots.
        for bot in self.entityMap.keys():
            if bot not in self.game.bots_alive:
                del self.entityMap[bot]
        # Create entities for unaccounted (new) bots.
        for bot in self.game.bots_alive:
            if bot not in self.entityMap:
                newEntity = self._onCreateNewEntity(bot)
                if newEntity:
                    self.entityMap[bot] = newEntity

        # Update all entities.
        for bot, entity in self.entityMap.items():
            entity.update()

    def shutdown(self):
        pass

# Bot that doesn't do anything (for testing).
class Dummy(Commander):
    def tick(self):
        pass