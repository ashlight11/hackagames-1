#!env python3

def main():
    import client
    client.takeASeat('localhost', 2014, Player() )

class Player() :
    def wakeUp(self, tabletop):
        self.world= tabletop
        print( "Tabletop:"),
        for i in range( len(self.world) ):
            print( '  ', str(i), ':\t', str(self.world[i]) )
    
    def perceive(self, horizon, playerId, pieces, scores):
        print( "game state:", horizon, playerId)
        print( 'Pieces:', ',\n\t'.join([ str(p) for p in pieces ]) )
        print( 'score:', scores)
    
    def decide(self):
        print( 'action:', "sleep")
        return "sleep"

    def sleep(self, score):
        print( "Final: ", score)

# Activate default interface :
if __name__ == '__main__':
    main()
