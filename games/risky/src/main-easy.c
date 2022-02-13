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

#define NUM_PLAYER 2
#define NUM_TURN   30
#define NUM_ACTION_PER_TURN 1

int fight( Organism* target, int strengh ){ return fightDeterminist(target, strengh); }

int main(int nbArg, char ** arg)
{
    int nbTurn= NUM_TURN;
    if( nbArg > 1 )
        nbTurn= atoi( arg[1] );

    int nbGame= 1;
    if( nbArg > 2 )
        nbGame= atoi( arg[2] );

    int mapSeed= time(NULL);
    if( nbArg > 3 )
        mapSeed= atoi( arg[3] );

    int fightSeed= time(NULL);
    if( nbArg > 4 )
        fightSeed= atoi( arg[4] );
    
    printf("\n------------------\nHackaGames Risky (game seed: %d - %d)\n------------------\n", mapSeed, fightSeed);

    // Game Initialization
    //--------------------
    Game* game= initializeGame();

    // Launch the Game
    //----------------
    Game_start( game );
    Interface* view= Interface_new( game->tabletop, 1200, 800, 10.f );
    Interface_startIHM( view );

    // Main game loop
    for( int iGame= 0 ;  iGame < nbGame ; ++iGame )
    {
        srand( mapSeed );
        resetGame(game);
        srand( fightSeed );
        riskyLoop(game, nbTurn, NUM_ACTION_PER_TURN );
        
        printf("score: %d(%f), %d(%f)\n",
            game->sockets[1], game->scores[1],
            game->sockets[2], game->scores[2] );
        
        if( iGame % 2 == 1)
        {
            mapSeed+= 1;
            fightSeed+= 1;
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
