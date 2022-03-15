/*******************************************************************************************
*
*   HackaGames
*   Copyright (c) 2020-2021 Guillaume Lozenguez - Institut Mines-Telecom
*
********************************************************************************************/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "risky.h"

#include "raylib.h"
#include "hackagames-interface.h"

#define NUM_PLAYER 2
#define NUM_TURN   30
#define NUM_ACTION_PER_TURN 1

int fight( Organism* target, int strengh )
    { return fightDeterminist(target, strengh); }

int main(int nbArg, char ** arg)
{
    int nbGame= 1;
    if( nbArg > 1 )
        nbGame= atoi( arg[1] );
    
    int nbTurn= NUM_TURN;
    if( nbArg > 2 )
        nbTurn= atoi( arg[2] );

    int seed= time(NULL);
    if( nbArg > 3 )
        seed= atoi( arg[3] );

    int mapSeed= time(NULL);
    if( nbArg > 4 )
        mapSeed= atoi( arg[4] );
    
    printf( "\n------------------\nHackaGames Risky (game seed: %d)\n------------------\n", seed);

    // Game Initialization
    //--------------------
    Game* game= initializeGame();
    puts("Game initialized");

    // Launch the Game
    //----------------
    Game_start( game );
    puts("Game started");

    Interface* view= Interface_new( game->tabletop, 1200, 800, 24.f );
    int ihmMask[2]= {PIECE_STRENGH, PIECE_ACTIVED};
    Interface_setAttributMask_ofSize(view, ihmMask, 2);
    
    Interface_startIHM( view );
    puts("IHM started");

    // Main game loop
    puts("Starts the games:");
    for( int iGame= 0 ;  iGame < nbGame ; ++iGame )
    {
        srand( mapSeed );
        resetGame(game);
        srand( seed );
        riskyLoop(game, nbTurn, NUM_ACTION_PER_TURN );
        
        printf("score: %d(%f), %d(%f)\n",
            game->sockets[1], game->scores[1],
            game->sockets[2], game->scores[2] );
        
        if( iGame % 2 == 1)
        {
            mapSeed+= 1;
            seed+= 1;
        }
        else
        {
            game_switchPlayers(game);
        }
    }
    
    // Stop IHM
    //-----------
    Interface_stopIHM( view );
    Game_stop( game );
    
    Game_delete( game );
    return 0;
}
