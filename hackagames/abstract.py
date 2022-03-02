

class Player() :
    # AI interface :
    def wakeUp(self, numberOfPlayers, playerId, tabletop):
        pass

    def perceive(self, turn, scores, pieces, deltaTabletop):
        pass

    def decide(self):
        pass # must return the action to perfom as a string.

    def sleep(self, result):
        pass


class Game() :
    # Game engine interface :
    def initialize(self):
        pass
    
    def turn():
        pass

    def pieces():
        pass
    
    def step(self, playerAction):
        pass

    def step(self):
        pass
    
    def start(self, player, numberOfGames=1 ):
        rewards= [0.0 for i in range(numberOfGames)]
        for i in range(numberOfGames):
            self.initialize()
            rewards[i]= 0.0
            player.perceive( self.turn(), [rewards[i]], 0, self.state() )
            while not self.isEnd() :
                rewards[i]= self.step( player.decide() )
                player.perceive( self.stateDico(), rewards[i] )
        return rewards

class GameOnePlayer(Game) :
    def start(self, player, numberOfGames=1 ):
        rewards= [0.0 for i in range(numberOfGames)]
        for i in range(numberOfGames):
            self.initialize()
            rewards[i]= 0.0
            player.wakeUp(1, 0, [])
            player.perceive( self.turn(), [rewards[i]], 0, self.pieces(0) )
            while not self.isEnd() :
                rewards[i]= self.step( player.decide() )
                player.perceive( self.turn(), [rewards[i]], 0, self.pieces(0) )
        return rewards
