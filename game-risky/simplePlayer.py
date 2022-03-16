#!env python3
import sys, os, random
sys.path.insert(1, os.path.join(sys.path[0], '..'))

import hackagames as hg

STRENGTH= 0
ACTIVATED= 1

def main():
    hg.takeASeat('localhost', 14001, Player() )

class Player(hg.PlayerVerbose) :

    # AI Interface :
    def decide(self):
        actions= [ ['sleep'] ]
        for piece in self.pieces :
            actions+= self.actionsFrom(self.id, piece)
        action= random.choice( actions )
        if( action[0] == 'move' ): #then get a random strength:
            action[3]= random.randrange( action[3] )
        actstr= ' '.join([str(x) for x in action])
        print( 'action:', actstr )
        return actstr

    # Generate possible actions :
    def actionsFrom( self, playerid, aPiece ):
        actions= []
        if aPiece.owner == playerid and aPiece.attributs[ACTIVATED] == 0 :
            actions.append( ['grow', aPiece.position] )
            for edge in self.tabletop[ aPiece.position ] :
                actions.append( ['move', aPiece.position, edge, aPiece.attributs[STRENGTH] ] )
        return actions

# Activate default interface :
if __name__ == '__main__':
    main()
