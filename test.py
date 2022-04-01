#!env python3
import matplotlib.pyplot as plt
import hackagames as hg
import sys
import os
import random
import json
from sklearn import tree
sys.path.insert(1, os.path.join(sys.path[0], '..'))

STRENGTH = 0
ACTIVATED = 1


def main():
    # Opening JSON file
    try : 
        with open('trained_qvalues_bis.json') as json_file:
            data = json.load(json_file)
    except FileNotFoundError :
        data = None
    except json.JSONDecodeError :
        data = None
    player = Player(qvalues=data)
    hg.takeASeat('localhost', 14001, player)


    # Dumping json into a file
    with open("trained_qvalues_bis.json", "w") as outfile:
        json.dump(player.qvalues, outfile)

    for feature in player.stats:
        del player.stats[feature][-1]
        print(player.stats[feature])
        plt.plot(player.stats[feature])
        plt.ylabel(feature)
        plt.show()


def diffScore(playerId, scores):
    playerScore = scores[playerId-1]
    oponentScore = 0
    for i in range(len(scores)):
        if i != playerId-1 and scores[i] > oponentScore:
            oponentScore = scores[i]
    diff = playerScore-oponentScore
    return diff

def append_new_line(file_name, text_to_append):
    """Append given text as a new line at the end of file"""
    # Open the file in append & read mode ('a+')
    with open(file_name, "a+") as file_object:
        # Move read cursor to the start of file.
        file_object.seek(0)
        # If file is not empty then append '\n'
        data = file_object.read(100)
        if len(data) > 0:
            file_object.write("\n")
        # Append text at the end of file
        file_object.write(text_to_append)

def state_str_to_array(state) : 
    
    a_list = state.split("|")
    map_object = map(str, a_list)
    list_of_strs = list(map_object)
    
    value = []
    for element in list_of_strs :
        list_b = element.split("-")
        map_b = map(int, list_b)
        value.extend(list(map_b))
    return value


class Player(hg.Player):
    def __init__(self, explorationRatio=0.1, discountFactor=0.99, learningRate=0.1, qvalues=None):
        super().__init__()
        
        self.epsilon = explorationRatio
        self.gamma = discountFactor
        self.alpha = learningRate
        if(qvalues == None ):
            self.qvalues = {}
        else :
            print("Using previous qvalues")
            self.qvalues = qvalues
        self.stats = {"exploration": [0],
                      "average end": [0], "average best Q": [0]}
        self.episod = 0
        self.heuristic_action = 'sleep'
        self.action = [['sleep']]
        self.string_labels = ["win", "defeat", "slight disadvantage", "slight advantage", "large disadvantage", "large advantage"]
        self.define_model()

    def define_model(self):
        samples = []

        with open("wins.txt") as file :
            wins = file.read().splitlines()

        with open("defeats.txt") as file : 
            defeats = file.read().splitlines()
        with open("slight_disadvantages.txt") as file : 
            slight_disadvantages = file.read().splitlines()
        with open("slight_advantages.txt") as file : 
            slight_advantages = file.read().splitlines()
        with open("large_disadvantages.txt") as file : 
            large_disadvantages = file.readlines()
        with open("large_advantages.txt") as file : 
            large_advantages = file.read().splitlines()

        samples.extend(wins)
        samples.extend(defeats)
        samples.extend(slight_disadvantages)
        samples.extend(slight_advantages)
        samples.extend(large_disadvantages)
        samples.extend(large_advantages)
        state_labels = [0 for i in range (len(wins))]
        state_labels.extend([1 for i in range (len(defeats))])
        state_labels.extend([2 for i in range (len(slight_disadvantages))])
        state_labels.extend([3 for i in range (len(slight_advantages))])
        state_labels.extend([4 for i in range (len(large_disadvantages))])
        state_labels.extend([5 for i in range (len(large_advantages))])

        
        for index, line in enumerate(samples) : 
            a_list = line.split("|")
            map_object = map(str, a_list)
            list_of_integers = list(map_object)
            
            value = []
            for element in list_of_integers :
                list_b = element.split("-")
                map_b = map(int, list_b)
                value.extend(list(map_b))
            samples[index] = value

        self.clf = tree.DecisionTreeClassifier()
        self.clf = self.clf.fit(samples, state_labels)

    # State Machine :
    def stateStr(self):
        return self.fullStateStr()

    def fullStateStr(self):
        self.current_states = ['0-0-0' for c in self.tabletop]
        for p in self.pieces:
            owner = '0'
            if p.owner == self.id:
                owner = '1'
            self.current_states[p.position] = owner + '-' + \
                str(p.attributs[STRENGTH]) + '-' + str(p.attributs[ACTIVATED])
        return '|'.join(self.current_states)

    def reward(self, playerId, newScores):
        diff = diffScore(playerId, newScores)
        return diff - diffScore(playerId, self.scores)

    # Q learning methods:
    def updateQ(self, aStateT0, anAction, aStateT1, aReward):

        oldValue = self.qvalues[aStateT0][anAction]
        futureGains = self.qvalues[aStateT1][self.bestAction(aStateT1)]
        self.qvalues[aStateT0][anAction] = (
            1 - self.alpha) * oldValue + self.alpha * (aReward + self.gamma * futureGains)

    def bestAction(self, aState):
        option = random.choice(list(self.qvalues[aState].keys()))
        for a in self.qvalues[aState]:
            if self.qvalues[aState][a] > self.qvalues[aState][option]:
                option = a
        return option

    # AI Interface :
    def wakeUp(self, numberOfPlayers, playerId, tabletop):
        super().wakeUp(numberOfPlayers, playerId, tabletop)
        self.scores = [0 for i in range(numberOfPlayers)]
        initialState = self.stateStr()
        initialState = [state_str_to_array(initialState)]
        initialState = self.string_labels[self.clf.predict(initialState)[0]]
        
        if initialState not in self.qvalues.keys():
            self.qvalues[initialState] = {'sleep': 0.0, 'expand': 0.0, 'attack': 0.0,
            'defend': 0.0, 'grow': 0.0}

    def perceive(self, turn, scores, pieces):
        last = self.stateStr()
        last_labeled = self.clf.predict([state_str_to_array(last)])[0]
        last_labeled = self.string_labels[last_labeled]
        reward = self.reward(self.id, scores)
        super().perceive(turn, scores, pieces)
        state = self.stateStr()
        state_labeled = self.clf.predict([state_str_to_array(state)])[0]
        state_labeled = self.string_labels[state_labeled]
        print("state : ", state)
        # Initialize qvalues structure if required:
        if state_labeled not in self.qvalues.keys():
            self.qvalues[state_labeled] = {'sleep': 0.0, 'expand': 0.0, 'attack': 0.0,
            'defend': 0.0, 'grow': 0.0}
    
        if last_labeled not in self.qvalues.keys():
            self.qvalues[last_labeled] = {'sleep': 0.0, 'expand': 0.0, 'attack': 0.0,
            'defend': 0.0, 'grow': 0.0}
        
        # If heuristic action == random, classify it, else : trust heuristic
        if self.heuristic_action == "random" :
            print("--------------------")
            #print("identifying random...")
            self.heuristic_action = self.identify_last_action(last, state)
        
        #print("action made : ", self.action, "; heuristic : ", self.heuristic_action)
        

        if self.heuristic_action not in self.qvalues[last_labeled].keys() :
            self.qvalues[last_labeled][self.heuristic_action]= 0.0


        # Apply Q equation:
        self.updateQ(last_labeled, self.heuristic_action, state_labeled, reward)

    def decide(self):
        state = self.stateStr()
        state_labeled = self.clf.predict([state_str_to_array(state)])[0]
        state_labeled = self.string_labels[state_labeled]
        if state_labeled not in self.qvalues.keys():
            self.qvalues[state_labeled] = {'sleep': 0.0, 'expand': 0.0, 'attack': 0.0,
            'defend': 0.0, 'grow': 0.0}
        
        if self.episod < 100 or random.random() < self.epsilon:
            action = self.get_random_action()
            self.heuristic_action = "random"
        else:
            best_action = self.bestAction(state_labeled)
            # find best option
            if(best_action == "expand"): 
                action = self.move_expand()
            elif (best_action =="grow"):
                action = self.grow()
            elif (best_action == "defend"):
                action = self.move_defend()
            elif (best_action == "attack"):
                action = self.attack()
            else :
                action=[ ['sleep'] ]
        self.action = action
        actstr = ' '.join([str(x) for x in action])
        print( 'action:', actstr)
        #print( f'state: { self.stateStr() }, action: { self.action }')
        return actstr

    def identify_last_action(self, last_state, current_state):
        # classify previous actions (random for examples)
        # print("identify : ", self.action)
        if self.action[0] == "sleep" :
            #print("last action was sleep")
            return "sleep"
        if self.action[0] == "grow" :
            #print("last action was grow")
            return "grow"
        if self.action[0] == "move" :
            target = self.action[2]
            previous = last_state.split("|")
            p_owner, p_forces, p_activation = previous[target].split('-')
            current = current_state.split("|")
            c_owner, c_forces, c_activation = current[target].split("-")
            #print("previous : ", p_owner, p_forces, p_activation)
            #print("current : ", c_owner, c_forces, c_activation)
            
            # if we are the current owner but the case was previously owned => attack
            if(int(c_owner) == 1 and int(p_owner) == 0 and int(p_forces) != 0) :
                return "attack"
            # if we are the current owner but we were also owner previously => defend
            elif(int(c_owner) == 1 and c_owner == p_owner):
                return "defend"
            # if we are the current owner but the case was previously not owned => expand
            elif (int(c_owner) == 1 and int(p_owner) == 0 and int(p_forces) == 0):
                return "expand"
            # error case <=> sleep
            else :
                return "sleep"
                
    def attack(self):
        # Find the way to move forces to a node occupied by opponent
        attack_action = []
        for index, piece in enumerate(self.current_states) :
            owner, forces, activation = piece.split('-')
            owner = int(owner)
            forces = int(forces)
            activation = int(activation)
            # if we own this case and it can be played
            if int(owner) == 1 and int(activation) == 0 and int(forces) > 1 : 
                # link = case we are linked to
                for link in self.tabletop[index] :
                    opponent_forces = int(self.current_states[link].split('-')[1])
                    if opponent_forces < forces - 2:
                        print("found weak adjacent")
                        attack_action.append(['move', index, link, opponent_forces + 2 ])
                        self.heuristic_action = 'attack'
                        return attack_action
            else :
                attack_action = self.get_random_action()
                self.heuristic_action = 'random'          
        return attack_action
    
    def grow(self):
        # Find the way to grow a node owned
        growing_action = []
        for index, piece in enumerate(self.current_states) :
            owner, forces, activation = piece.split('-')
            owner = int(owner)
            forces = int(forces)
            activation = int(activation)
            # if we own this case and it is interesting to grow
            if int(owner) == 1 and int(forces) < 8 : 
                print("found growable case")
                growing_action.append(['grow', index])
                self.heuristic_action = 'grow'
                return growing_action
            else :
                growing_action = self.get_random_action()
                self.heuristic_action = 'random'         
        return growing_action

    def move_expand(self):
        # Find the way to expand the territory of our AI
        # Look for first adjacent piece available
        expanding_action = []
        for index, piece in enumerate(self.current_states) :
            owner, forces, activation = piece.split('-')
            owner = int(owner)
            forces = int(forces)
            activation = int(activation)
            #print("OSA : ", owner, forces, activation)
            # if we own this case and it can be played
            if int(owner) == 1 and activation == 0 and forces > 1 : 
                # link = case we are linked to
                for link in self.tabletop[index] :
                    if int(self.current_states[link].split('-')[1]) == 0 :
                        print("found empty adjacent")
                        expanding_action.append(['move', index, link, forces - 1])
                        self.heuristic_action = 'expand'
                        return expanding_action
            else :
                expanding_action = self.get_random_action()
                self.heuristic_action = "random"       
        return expanding_action
    
    def move_defend(self):
        # Find the way to make internal moves to defend our territory
        defend_action = []
        for index, piece in enumerate(self.current_states) :
            owner, forces, activation = piece.split('-')
            owner = int(owner)
            forces = int(forces)
            activation = int(activation)
            # if we own this case and it can be played
            if int(owner) == 1 and activation == 0 and forces > 1 : 
                # link = case we are linked to
                for link in self.tabletop[index] :
                    o, f, a = self.current_states[link].split('-')
                    o = int(o)
                    f = int(f)
                    a = int(a)
                    if o == 1 and a == 1 and f < 4 :
                        print("found weak neighbour owned by us")
                        defend_action.append(['move', index, link, forces - 1])
                        self.heuristic_action = 'defend'
                        return defend_action
            else :
                #defend_action.append(['sleep']) 
                #self.heuristic_action = 'sleep'  
                defend_action = self.get_random_action()
                self.heuristic_action = 'random'
                           
        return defend_action

    def sleep(self, result):
        super().sleep(result)

        # Compute markers
        sizeQ = len(self.qvalues)
        avBestQ = 0.0
        for s in self.qvalues:
            aStar = self.bestAction(s)
            avBestQ += self.qvalues[s][aStar]
        avBestQ = (float)(avBestQ)/sizeQ
        print(f'exploration: {sizeQ} end: {result} average best Q: {avBestQ}')

        # Reccords them
        xpLen = 10
        xp = self.episod // xpLen
        self.stats["exploration"][xp] += len(self.qvalues)
        self.stats["average end"][xp] += result
        self.stats["average best Q"][xp] += avBestQ

        # Prepare for a new episod:
        self.episod += 1
        if self.episod % xpLen == 0:
            for feature in self.stats:
                self.stats[feature][xp] = self.stats[feature][xp] / xpLen
                self.stats[feature].append(0)

    # Generate possible actions :
    def randomAction(self):
        actions = [['sleep']]
        for piece in self.pieces:
            actions += self.actionsFrom(self.id, piece)
        action = random.choice(actions)
        if(action[0] == 'move'):  # then get a random strengh:
            action[3] = random.randrange(action[3])
        return ' '.join([str(x) for x in action])
    
    # Generate possible actions as an array : 
    def get_random_action(self):
        actions = [['sleep']]
        for piece in self.pieces:
            actions += self.actionsFrom(self.id, piece)
        action = random.choice(actions)
        if(action[0] == 'move'):  # then get a random strengh:
            if(action[3] > 1 ):
                action[3] = random.randrange(1, action[3])
        return action

    def actionsFrom(self, playerid, aPiece):
        actions = []
        if aPiece.owner == playerid and aPiece.attributs[ACTIVATED] == 0:
            actions.append(['grow', aPiece.position])
            for edge in self.tabletop[aPiece.position]:
                actions.append(['move', aPiece.position, edge,
                               aPiece.attributs[STRENGTH]])
        return actions


# Activate default interface :
if __name__ == '__main__':
    main()
