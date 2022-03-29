#!env python3
import sys, os, random
from turtle import st

from scipy import rand
sys.path.insert(1, os.path.join(sys.path[0], '..'))

import hackagames as hg

STRENGTH= 0
ACTIVATED= 1

def main():
    hg.takeASeat('localhost', 14001, Player() )

class Player(hg.PlayerVerbose) :

    def __init__(self, qvalues = None):
        super().__init__()
        print("!!!!!!!!!!!!!!!!!!!!!!!!!INIT")
        if(qvalues == None) :
            self.qvalues = {}
        else :
            self.qvalues = qvalues
        self.score = 0
        self.action = ["move", 0, 2, 1]
        self.stats = {"exploration": [0],
                      "average end": [0], "average best Q": [0]}

    def perceive(self, turn, scores, pieces, tabletop=...):
        last = self.stateStr()
        # self.reward= self.reward_calculation(self.id, scores)
        self.reward = scores[self.id -1] - self.score
        self.score = scores[self.id -1]
        self.pieces = pieces
        
        alpha = 0.1
        gamma  = 0.99
        epsilon = 0.1
        #print("last state", last)

         # Initialize qvalues structure if required:
        '''if state not in self.qvalues.keys():
            self.qvalues[state] = {self.randomAction(): 0.0}
        if self.action not in self.qvalues[last].keys():
            self.qvalues[last][self.action] = 0.0'''

        if last not in self.qvalues.keys():
            self.qvalues[last] = {"move" : 0, "sleep" : 0, "grow" : 0}

        action = self.action[0]
        self.qvalues[last][action] = (1- alpha) * self.qvalues[last][action] + alpha * (self.reward + gamma * self.findMax(last)[0])
        

    def findMax(self, state):
        max_value = 0.0
        best_action = self.get_random_action()
        for action in self.qvalues[state].keys():
            if(self.qvalues[state][action] > max_value):
                max_value = self.qvalues[state][action]
                best_action = action
        return max_value, best_action

    # AI Interface :
    def decide(self):
        init_state = "0-24-0|1-24-0|0-0-0|0-0-0|0-0-0|0-0-0|0-0-0|0-0-0|0-0-0|0-0-0|0-0-0|0-0-0|0-0-0|0-0-0"
        state = self.stateStr()

        if state not in self.qvalues.keys():
            self.qvalues[state] = {"move" : 0, "sleep" : 0, "grow" : 0}


        if(state == init_state):
            print("init state")
            actstr= ' '.join([str(x) for x in self.action])
            print( 'action after init :', actstr )
            return actstr
        
        else :
            mode = random.randrange(0, 10)
            if(mode < 1) :
                print("random action")
                return self.get_random_action()
            else :
                # find best option
                best_action = self.findMax(state)[1]
                if( best_action == 'move' ): #then get a random strength:
                    print("action" , self.action[3])
                    action = [best_action, self.action[2], random.randrange(0, 13), 1]
                elif (best_action =="grow"):
                    action = [best_action, self.action[2]]
                else :
                    action=[ ['sleep'] ]

                self.action = action
                actstr= ' '.join([str(x) for x in action])
                print( 'action:', actstr )
                return actstr
            

    def get_random_action(self):
        actions= [ ['sleep'] ]
        for piece in self.pieces :
            actions+= self.actionsFrom(self.id, piece)
        action= random.choice( actions )
        if( action[0] == 'move' ): #then get a random strength:
            action[3]= random.randrange( action[3] )
        actstr= ' '.join([str(x) for x in action])
        # print( 'action:', actstr )
        return actstr


    # Generate possible actions :
    def actionsFrom( self, playerid, aPiece ):
        actions= []
        if aPiece.owner == playerid and aPiece.attributs[ACTIVATED] == 0 :
            actions.append( ['grow', aPiece.position] )
            for edge in self.tabletop[ aPiece.position ] :
                actions.append( ['move', aPiece.position, edge, aPiece.attributs[STRENGTH] ] )
        return actions

    
    def stateStr(self):
        states= ['0-0-0' for c in self.tabletop]
        for p in self.pieces :
            owner= '0'
            if p.owner == self.id :
                owner= '1'
            states[p.position]= owner + '-' + str(p.attributs[STRENGTH]) +'-'+ str(p.attributs[ACTIVATED])
        return '|'.join(states)

    def reward(self, playerId, newScores):
        return diffScore(playerId, newScores) -  diffScore(playerId, self.scores)

    
def diffScore(playerId, scores):
        playerScore=  scores[playerId-1]
        oponentScore= 0
        for i in range( len(scores) ) :
            if i != playerId-1 and scores[i] > oponentScore :
                oponentScore= scores[i]
        return playerScore-oponentScore
        


# Activate default interface :
if __name__ == '__main__':
    main()
