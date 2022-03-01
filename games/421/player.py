#!env python3
"""
Script MDP 421 
"""
import random

class Abstarct :
    def __init__(self, model):
        self.model= model

    def perceive(self, state, reward):
        self.model.setOnStateDico( state )

    def action(self):
        pass

class Random(Abstarct) :
    def __init__(self, model):
        self.model= model

    def perceive(self, perception, reward):
        self.model.setOnStateDico( perception )
        print( "Perception: "+ self.model.stateStr() +" with reward : " + str(reward) )

    def action(self):
        action= random.choice( self.model.allActions() )
        print( self.model.actionToStr(action) )
        return action

class Human(Abstarct) :

    def perceive(self, perception, reward):
        self.model.setOnStateDico( perception )
        print( "Perception: "+ self.model.stateStr() +" with reward : " + str(reward) )

    def action(self) :
        print( "Action ?")
        actionStr= ""
        while not self.model.isActionStr( actionStr ) :
                actionStr= input()
        return self.model.actionFromStr(actionStr)
