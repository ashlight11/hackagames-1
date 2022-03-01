#!env python3
#!env python3
import game421 as game
import player421 as player

gameEngine= game.Engine()
player= player.Human( game.Engine() )

nb= 4
rewards= [0.0 for i in range(nb)]
for i in range(nb):
    gameEngine.initialize()
    rewards[i]= 0.0
    print("Info: new game")
    player.perceive( gameEngine.stateDico(), rewards[i] )
    while not gameEngine.isEnd() :
        rewards[i]= gameEngine.step( player.action() )
        player.perceive( gameEngine.stateDico(), rewards[i] )
print("rewards: "+ ', '.join(str(x) for x in rewards) )
