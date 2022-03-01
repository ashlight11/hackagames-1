#!env python3
#!env python3
import player
import game421 as game

gameEngine= game.Engine()
player= player.Random( game.Engine() )

gameEngine.initialize()
reward= 0.0
player.perceive( gameEngine.stateDico(), reward )
while not gameEngine.isEnd() :
    reward= gameEngine.step( player.action() )
    player.perceive( gameEngine.stateDico(), reward )
