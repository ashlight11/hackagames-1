#!env python3
import socket
from . import abstract, client

def takeASeat(host, port, player ):
    with socket.socket( socket.AF_INET, socket.SOCK_STREAM) as sock :
        sock.connect((host, port))
        itf= client.Interface( sock, player )
        itf.go()

class Piece() :
    def __init__(self, position, type, name, owner=0, attributs=[]):
        self.type= int(type)
        self.name= name
        self.owner= int(owner)
        self.position= int(position)
        self.attributs= [int(x) for x in attributs]

    def copie(self):
        return Piece( self.position, self.type, self.name, self.owner, self.attributs )
    
    def __str__(self):
        return f'player-{self.owner}: {self.type} {self.name} on {self.position} {self.attributs}'

class Player(abstract.Player) :
    def __init__(self):
        self.results= []

    def wakeUp(self, numberOfPlayers, playerId, tabletop):
        self.turn= 0
        self.id= 0
        self.pieces= []
        self.scores= [0]
        self.numberOfPlayers= numberOfPlayers
        self.id= playerId
        self.tabletop= tabletop
        
    def perceive(self, turn, scores, pieces, deltaTabletop):
        self.turn= turn
        self.pieces= [ Piece(tab[0], tab[1], tab[2], tab[3], tab[6:] ) for tab in pieces ]
        self.scores= scores

    def decide(self):
        return "sleep"

    def sleep(self, result):
        self.results.append(result)

class PlayerVerbose(Player):
    def wakeUp(self, numberOfPlayers, playerId, tabletop):
        super().wakeUp(numberOfPlayers, playerId, tabletop)
        print( "Tabletop:"),
        for i in range( len(self.tabletop) ):
            print( '  ', str(i), ':\t', str(self.tabletop[i]) )
    
    def perceive(self, turn, scores, pieces, deltaTabletop=[]):
        super().perceive(turn, scores, pieces, deltaTabletop)
        print( f'player-{self.id}" turn: {turn}' )
        print( 'Pieces:', ',\n\t'.join([ str(p) for p in self.pieces ]) )
        print( 'score:', scores)

    def decide(self):
        a= super().decide()
        print( 'action:', a)
        return a

    def sleep(self, result):
        super().sleep(result)
        print( "Final: ", result)
