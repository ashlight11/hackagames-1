#!env python3
import socket

def takeASeat(host, port, player ):
    with socket.socket( socket.AF_INET, socket.SOCK_STREAM) as sock :
        sock.connect((host, port))
        itf= Interface( sock, player )
        itf.go()

class Interface():
    def __init__(self, sock, player ):
        self.sock= sock
        self.player= player
        self.playerId= -1
        self.score= []
        self.horizon= -1
        self.tabletop= []
        self.pieces= []

    def go(self):
        self.state= self.stWakeUp
        while True :
            try:
                data= self.sock.recv(1024)
            except:
                break
            if not data :
                break
            data= data.decode('utf-8').split('\n')
            for line in data :
                if line != "" :
                    self.state= self.state( line.split(' ') )

    def stWakeUp(self, input):
        if( input[0] == "Info:" ):
            print( "- ", " ".join( input ) )
        elif( input[0] == "Tabletop:" ):
            self.tabletop= [ [] for i in range( int(input[1]) ) ]
            self.size= 0
        elif( input[0] == "Node:" ):
            assert( self.size == int(input[1]) )
            for edge in input[4:] :
                self.tabletop[self.size].append( int(edge) )
            self.size+= 1
            if self.size == len(self.tabletop) :
                self.player.wakeUp( self.tabletop )
                return self.stLoop
        return self.stWakeUp

    def stLoop(self, input):
        if( input[0] == "Info:" ):
            print( "- ", " ".join( input ) )
            return self.stLoop
        elif( input[0] == "End:" ):
            self.player.sleep( int(input[1]) )
            return self.stWakeUp
        elif( input[0] == "Player:" ):
            self.playerId= int(input[1])
            self.score= [ float(val) for val in input[3:] ]
            return self.stLoop
        elif( input[0] == "Game:" ):
            self.horizon= int(input[1])
            self.pieces= []
            self.size= int(input[2])
            return self.stLoop
        elif( input[0] == "Piece:" ):
            self.pieces.append( input[1:] )
            if self.size == len(self.pieces) :
                self.player.perceive(self.horizon, self.playerId, self.pieces, self.score )
                return self.stLoop
        elif input[0] == "Your-turn:" :
            action= self.player.decide()
            self.sock.send( str.encode(action) )
        return self.stLoop

    def sleep(self):
        print("sleep")

class Piece() :
    def __init__(self, position, type, name, owner=0, attributs=[]):
        self.type= int(type)
        self.name= name
        self.owner= int(owner)
        self.position= int(position)
        self.attributs= [int(x) for x in attributs]

    def copie(self):
        return Piece( self.position, self.type, self.name, self.owner. self.attributs )
    
    def __str__(self):
        return f'player-{self.owner}: {self.type} {self.name} on {self.position} {self.attributs}'

class AI() :
    # AI interface :
    def wakeUp(self, tabletop):
        self.turn= 0
        self.id= 0
        self.pieces= []
        self.scores= [0]
        self.tabletop= tabletop
        
    def perceive(self, turn, playerId, pieces, scores):
        self.turn= turn
        self.id= playerId
        self.pieces= [ Piece(tab[0], tab[1], tab[2], tab[3], tab[6:] ) for tab in pieces ]
        self.scores= scores

    def decide(self):
        return "sleep"

    def sleep(self, win):
        pass

class VerboseAI(AI) :
    # AI interface :
    def wakeUp(self, tabletop):
        super().wakeUp(tabletop)
        print( "Tabletop:"),
        for i in range( len(self.tabletop) ):
            print( '  ', str(i), ':\t', str(self.tabletop[i]) )
    
    def perceive(self, turn, playerId, pieces, scores):
        super().perceive(turn, playerId, pieces, scores)
        print( f'player-{self.id}" turn: {turn}' )
        print( 'Pieces:', ',\n\t'.join([ str(p) for p in self.pieces ]) )
        print( 'score:', scores)

    def decide(self):
        print( 'action:', "sleep")
        return "sleep"

    def sleep(self, win):
        print( "Final: ", win)