#!env python3

def main():
    import client
    client.takeASeat('localhost', 14001, Player() )

class Piece() :
    def __init__(self, position, type, name, owner=0, attributs=[]):
        self.type= type
        self.name= name
        self.owner= owner
        self.position= int(position)
        self.attributs= [int(x) for x in attributs]

    def __str__(self):
        return f'player-{self.owner}: {self.type} {self.name} on {self.position} {self.attributs}'

class Player() :
    def wakeUp(self, tabletop):
        self.world= tabletop
        print( "Tabletop:"),
        for i in range( len(self.world) ):
            print( '  ', str(i), ':\t', str(self.world[i]) )
    
    def perceive(self, horizon, playerId, pieces, scores):
        print( "game state:", horizon, playerId)
        self.pieces= [ Piece(tab[0], tab[1], tab[2], tab[3], tab[6:] ) for tab in pieces ]
        print( 'Pieces:', ',\n\t'.join([ str(p) for p in self.pieces ]) )
        print( 'score:', scores)

    def decide(self):
        print( 'action:', "sleep")
        return "sleep"

    def sleep(self, score):
        print( "Final: ", score)

# Activate default interface :
if __name__ == '__main__':
    main()
