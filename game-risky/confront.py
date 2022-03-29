#!env python3
import sys, os, time, re
sys.path.insert(1, os.path.join(sys.path[0], '..'))

import hackagames as hg
from otherPlayerAI import Player

GAMES= 200
SEED= 123

gameDir= os.path.dirname( os.path.realpath(__file__) )
game_output= "hg-risky-"+str(GAMES)+"x"+str(SEED)+".log"

# Start the server:
cmd= gameDir+"/hg-risky-hidden "+ str(GAMES) +' '+ str(SEED) +"> "+ game_output +" &"
print("Start the game:", cmd)
os.system(cmd)
time.sleep(0.5)

# Player-1:
cmd = "python3 "+ gameDir + "/simplePlayer.py > oponent.log &"
print("seat the oponent:", cmd)
os.system(cmd)
time.sleep(0.5)

# Player-2:
# os.system( "python3 simplePlayer.py > player-2.log" )

AI_player = Player()

print("seat our own IA:")
hg.takeASeat( 'localhost', 14001, AI_player ) 

# Lets annalyse the games:

resultFile= open( game_output )

pattern= re.compile( 'score: ([\d]+)\(([\d]+\.[\d]+)\), ([\d]+)\(([\d]+\.[\d]+)\)' )

# First score:
score= {}
player1= None
player2= None

for line in resultFile :
    match= pattern.search( line.strip() )
    if match != None :
        print( line.strip() )
        player1= match[1]
        p1Score= match[2]
        player2= match[3]
        p2Score= match[4]
        score= {
            player1: [ float(p1Score) ],
            player2: [ float(p2Score) ],
        }
        break

nbGame=1
for line in resultFile :
    match= pattern.search( line.strip() )
    if match != None :
        print( line.strip() )
        p1ID= match[1]
        p1Score= match[2]
        p2ID= match[3]
        p2Score= match[4]
        score[p1ID].append( float(p1Score) )
        score[p2ID].append( float(p2Score) )
        nbGame+= 1
    
resultFile.close
wins= { p:0 for p in score }

for g in range(nbGame) :
    if score[player1][g] > score[player2][g] :
        wins[player1]+= 1
    if score[player2][g] > score[player1][g] :
        wins[player2]+= 1

winner= None
if wins[player1] > wins[player2] :
    winner= player1
if wins[player2] > wins[player1] :
    winner= player2

print( "Score" )
for p in score :
    print( p+":\t"+ ', '.join( [str(v) for v in score[p]] ) )

print( "Wins:" )
print( wins )

print( "Winner:", winner )

# print(AI_player.qvalues)


