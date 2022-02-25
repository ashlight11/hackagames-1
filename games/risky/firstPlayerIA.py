#!env python3
import pyhackagames.player as hgplayer
import pyhackagames.client as hgclient
import random

def main():
    hgclient.takeASeat('localhost', 14001, Player() )

class Piece(hgplayer.Piece) :

    def strengh(self):
        return self.attributs[0]

    def activated(self):
        return self.attributs[1]

    def __str__(self):
        return 'n-%d o-%d a-%d s-%d' % (self.node, self.owner, self.action, self.strengh)

class Player(hgplayer.Player) :

    # Actor interface :    
    def perceive(self, horizon, playerId, pieces, scores):
        super().perceive(horizon, playerId, pieces, scores)
        self.actions= [ ['sleep'] ]
        for piece in self.pieces :
            self.actions+= self.actionsFrom(self.id, piece)
    
    def decide(self):
        print( 'action:', "sleep")
        return "sleep"

    def sleep(self, score):
        print( "Final: ", score)

    # Generate possible actions :
    def actionsFrom( self, playerid, aPiece ):
        actions= []
        if aPiece.owner == playerid and aPiece.action == 0 :
            actions.append( ['grow', aPiece.position] )
            for edge in self.world[ aPiece.position ] :
                acts= [ ['move', aPiece.position, edge, s]
                            for s in range( 1, aPiece.strengh()+1 ) ]
                actions+= acts
        return actions

        

    '''def decide(self):
        print( 'state-%d:' % self.id, '\n\t'.join( [str(p) for p in self.pieces ] ) )
        options= [ random.choice( self.actions ) for x in range( min(len(self.actions), self.explo)) ]
        action= ['sleep']
        value= self.evaluate( self.simulate( self.id, action ) )
        for opt in options :
            modif= self.simulate( self.id, opt )
            result= self.evaluate(modif)
            print( 'option:', opt, result )
            print( 'reach:', '\n\t'.join( [str(p) for p in modif] ) )
            if result > value :
                action= opt
                value= result
        action= ' '.join( [str(w) for w in action ] )
        print( 'action:', action, value )
        return action

    def sleep(self, score):
        print( "Final: ", score)



    def simulate( self, playerid, action ):
        pieces= [ p.copie() for p in self.pieces ]
        if action[0] == 'sleep' :
            pieces= self.actionSleep( playerid, pieces )
        if action[0] == 'grow' :
            pieces= self.actionGrow( playerid, pieces, action[1] )
        if action[0] == 'move' :
            pieces= self.actionMove( playerid, pieces, action[1], action[2], action[3] )
        return pieces

    def actionSleep(self, playerid, pieces):
        for p in pieces:
            if p.owner == playerid :
                p.action= 0
        return pieces 

    def actionGrow(self, playerid, pieces, node):
        count= 1
        for p in pieces:
            if p.owner == playerid and p.node in self.world[node] :
                count+= 1
        
        for p in pieces:
            if p.node == node and p.owner == playerid :
                p.strengh+= count

        return pieces

    def actionMove(self, playerid, pieces, start, target, strengh):
        pStrat= 0
        pTarget= 0
        for p in pieces:
            if p.node == start :
                pStart= p
        for p in pieces:
            if p.node == target :
                pTarget= p
        if pTarget == 0 :
            pieces.append( Piece( [target, playerid, 1, strengh ] ) )
            pStart.strengh-= strengh
        elif pTarget.owner == playerid :
            pTarget.strengh+= strengh
            pStart.strengh-= strengh
        else: #fight
            victorious= self.fightEasy( pTarget, strengh )
            if victorious > 0 :
                pTarget.owner= playerid
                pTarget.strengh= victorious
            pStart.strengh-= strengh
        return [ p for p in pieces if p.strengh > 0 ] 

    def evaluate( self, pieces ):
        value= 0
        for p in pieces :
            if p.owner == self.id :
                value+= 10
                value+= p.strengh
        return value

    def fightEasy(self, target, strengh ):
        defence= target.strengh
        while defence > 0 and strengh > 0 :
            s= strengh
            overNumber= 0 
            if strengh-defence > 0 :
                overNumber= strengh-defence
            strengh-= 1+((defence*4)/6)
            defence-= 1+((s+overNumber*2)*3)/6
        
        if defence < 0 :
            defence= 0
        if strengh < 0 :
            strengh= 0

        target.strengh= defence
        return strengh'''

# Activate default interface :
if __name__ == '__main__':
    main()
