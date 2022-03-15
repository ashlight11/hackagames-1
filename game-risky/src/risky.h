/*******************************************************************************************
*
*   Risky, a Toghap game
*   Copyright (c) 2020-2021 Guillaume Lozenguez
*
********************************************************************************************/
#ifndef RISKY_H
#define RISKY_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "hackagames.h"

// Game attributes
//-----------------
enum Actions
{
    ACTION_MOVE= 0,
    ACTION_GROW,
    ACTION_SLEEP,
    ACTION_SIZE
};

enum Piece_Attributes
{
    PIECE_STRENGH= 0,
    PIECE_ACTIVED,
    PIECE_SIZE
};

enum Piece_Type
{
    TYPE_UNDEFINE= 0,
    TYPE_SOLDIER,
    TYPE_HORSEMAN,
    TYPE_CANON,
    TYPE_SIZE
};

// Game managment
//-----------------------
Game* initializeGame();
void resetGame(Game* game);
void initializePlayers(Game* game, int extrem);
int updateScore( Game* game, int playerID );
Organism * generateRandomTabletop( Organism* tabletop );
Organism * generateClassicalTabletop( Organism* tabletop );
void riskyLoop(Game * game, int num_turn, int num_action_per_trun);

Organism* Tabletop_addSoldierOn_ownedBy(Organism* tabletop, int position, int owner, int strengh);

// Game actions
//-----------------------
int resolveAction( Game* game, int playerID, Organism* action, int oneAction );
void actionSleep( Game* game, int playerID );
void actionMove( Game* game, int playerID, int from,  int to, int strengh);
void actionGrow( Game* game, int playerID, int target );

// modul to fight
//-----------------------
int fight( Organism* target, int strengh ); // function to implement.
int fightDeterminist( Organism* target, int strengh );
int fightStochastic( Organism* target, int strengh );

#endif