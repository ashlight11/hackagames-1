//-----------------------------------//
//--            GAME               --//
//-----------------------------------//
#ifndef HACKAGAMES_GAME_H
#define HACKAGAMES_GAME_H

enum GameStatus {
    GAME_END= 0,
    GAME_INITIALIZING,
    GAME_RUNNING
};

struct Str_Game {
    enum GameStatus status;
    Organism * tabletop;
    int nbPlayer;
    float * scores;
    int turn;
    
    // Tellnet players
    int* sockets; // 0 the master socket, 1 to nbPlayer for players
    int port;
    struct sockaddr_in address;
};
typedef struct Str_Game Game;

// Constructor / Destructor
Game* Game_new(Organism * tabletop, int nbPlayer);
void Game_delete(Game * self);

//Initialization:
void Game_resetTabletop(Game * self, Organism * tabletop);
void game_switchPlayers(Game * self);

// Game Engine : 
void Game_start(Game* self );
void Game_stop(Game* self );

// Interactions with player :
void Game_sendMsgTo( Game* self, char* msg, int playerID );
void Game_sendNetworkTo( Game* self, int playerID );
void Game_sendGameTo( Game* self, int playerID );
void Game_sendEndTo( Game* self, int playerID );
void Game_requestPlayer( Game* self, int playerID, Organism* anAction );

// Miniatures managment :
Organism* Game_addPiece( Game* self, int nodeID, Organism* aPiece );
Organism* Game_popPieceAs( Game* self, int nodeID, Organism* aModel );

#endif