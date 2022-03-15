#!env python3
#!env python3
import sys, os, random
sys.path.insert(1, os.path.join(sys.path[0], 'game-421'))

import game421 as game
import player421 as player

gameEngine= game.Engine()
player= player.PlayerHuman()

numberOfGames= 2
rewards= gameEngine.start( player, numberOfGames )
print( rewards )
