/*******************************************************************************************
*
*   Risky, a Toghap game
*   Copyright (c) 2020-2021 Guillaume Lozenguez
*
********************************************************************************************/
#include "risky.h"

#define OVER_NUMBER_RATIO 2
#define DICE_FACE 6
#define DICE_ATTACK 3
#define DICE_DEFENCE 4

char* action_names[3]= {"move", "grow", "sleep"};
int player_colors[3]= {0x808080FF, 0xFF0000FF, 0x0000FFFF};

Game* initializeGame()
{
    Game* game= Game_new( generateClassicalTabletop( Organism_newBasic("Risky") ), 2 );
    initializePlayers(game);
    return game;
}

void resetGame(Game* game)
{
    Organism_destroy( game->tabletop );
    Organism_construct( game->tabletop, 0, "Risky", 0, 0, 0.f, 0.f, 12 );
    generateClassicalTabletop( game->tabletop );
    initializePlayers(game);
}

Organism* Tabletop_addSoldierOn_ownedBy(Organism* tabletop, int position, int owner, int strengh)
{
    Organism* piece= Organism_addPieceOn(tabletop, position,
        Organism_new(TYPE_SOLDIER, "Soldiers", owner, PIECE_SIZE, 0) );
    piece->attrs[PIECE_STRENGH]= strengh;
    Organism_setPhisic(piece, -0.5f, 0.5f, 0.6f, player_colors[owner]);
    return piece;
}

void initializePlayers(Game* game)
{
    int position_p1= Organism_extremCellIdTo( game->tabletop, 0 );
    int position_p2= Organism_extremCellIdTo( game->tabletop, position_p1 );

    Tabletop_addSoldierOn_ownedBy( game->tabletop, position_p1, 1, 24 );
    Tabletop_addSoldierOn_ownedBy( game->tabletop, position_p2, 2, 24 );

    //initialise all player data socket to 0, a color...
    for (int i = 0; i <= game->nbPlayer; i++)
    {
        game->scores[i] = 0;
    }
}

// Game managment
//-----------------------
int updateScore( Game* game, int playerID )
{
    float value= 0.f;
    for( int iNode= 0 ; iNode < game->tabletop->size ; ++iNode )
    {
        Organism* node= Organism_cell( game->tabletop, iNode );
        if( node->size > 0 && node->cells[0]->owner == playerID )
        {
            value+= 1.0f;
        }
    }
    game->scores[playerID]+= value;
    return value;
}

void riskyLoop(Game * game, int num_turn, int num_action_per_trun)
{
    game->turn= num_turn;

    // Initialize players:
    for( int player= 1; player <= game->nbPlayer ; ++player )
        Game_sendNetworkTo( game, player );

    // And lets go:
    int player= 1;
    while( game->turn > 0 )
    {
        Game_sendGameTo( game, player );
        Organism* action= Organism_newBasic("Action");
        Game_requestPlayer( game, player, action );
        int endTurn= resolveAction( game, player, action, num_action_per_trun );
        if( endTurn )
        {
            updateScore(game, player);
            player+= 1;
            if( player > game->nbPlayer )
            {
                player= 1;
                // next turn:
                game->turn -= 1;
            }
        }
    }
    for( int i = 1 ; i <= game->nbPlayer ; ++i )
    {
        Game_sendGameTo( game, i );
        Game_sendEndTo( game, i );
    }
}

// Game actions
//-----------------------
int resolveAction(Game* game, int playerID, Organism* action, int oneAction )
{
    // if oneAction= 0, the game wait for a sleep action to pass to another player.
    int endTurn= oneAction;
    char buffer[1024];
    Organism_attributsStr(action, buffer);

    // Get action from nam
    int iAction= 0;
    while( iAction < ACTION_SIZE && strcmp(action->name, action_names[iAction]) )
        ++iAction;
    
    // poll the appropriate triger
    switch(iAction)
    {
    case ACTION_MOVE:
        if ( action->size >= 3 && action->attrs[2] > 0 )
        {
            actionMove(game, playerID,
                action->attrs[0],
                action->attrs[1],
                action->attrs[2] );
        }
        else 
        { 
            if ( action->size == 2 )
            {
                actionMove( game, playerID,
                    action->attrs[0], action->attrs[1], 1 );
            }
            else 
            {
                actionSleep( game, playerID );
                endTurn= 1;
            }
        }
        break;
    case ACTION_GROW:
        if ( action->size >= 1 )
        {
            actionGrow( game, playerID, action->attrs[0] );
        }
        else 
        {
            actionSleep( game, playerID );
            endTurn= 1;
        }
        break;
    case ACTION_SLEEP:
    default:
        actionSleep( game, playerID );
        endTurn= 1;
        break;
    
    }
    return endTurn;
}

void actionSleep( Game* game, int playerID )
{
    for( int iNode= 0 ; iNode < game->tabletop->size ; ++iNode )
    {
        Organism* node= Organism_cell( game->tabletop, iNode );
        if( node->size > 0 && node->cells[0]->owner == playerID )
        {
            node->cells[0]->attrs[PIECE_ACTIVED]= 0;
        }
    }
}

void actionGrow( Game* game, int playerID, int iNode)
{
    Organism* node= Organism_cell( game->tabletop, iNode );

    // Exist minion to grow ?
    if( node->size < 1 )
    {
        Game_sendMsgTo(game, "Info: wrong action - no minion\n", playerID);
        actionSleep( game, playerID );
        return;
    }

    Organism* minion= node->cells[0];
    if( minion->owner != playerID )
    {
        Game_sendMsgTo(game, "Info: wrong action - not a player's minion\n", playerID);
        actionSleep( game, playerID );
        return;
    }

    if( minion->attrs[PIECE_ACTIVED] )
    {
        Game_sendMsgTo(game, "Info: wrong action - already activated minion\n", playerID);
        actionSleep( game, playerID );
        return;
    }

    // Compute a grow strengh proportional to the node connection
    int count= 1;
    int card=  Organism_cellCardinality(game->tabletop, iNode);
    for( int i= 0 ; i < card ; ++i )
    {
        Organism* neibourg=  Organism_linkTarget(game->tabletop, iNode, i);
        if( neibourg->size > 0 && neibourg->cells[0]->owner == playerID )
        {
            ++count;
        }
    }
    minion->attrs[PIECE_ACTIVED]= 1;
    minion->attrs[PIECE_STRENGH]+= count;
}

void actionMove(Game* game, int playerID, int from,  int to, int strengh )
{
    Organism* start= Organism_cell( game->tabletop, from );
    Organism* target= Organism_cell( game->tabletop, to );

    // Exist minion to move ?
    if( start->size < 1 )
    {
        Game_sendMsgTo(game, "Info: wrong movement - no minion\n", playerID);
        actionSleep( game, playerID );
        return;
    }

    // Availlable edges ?
    if( !Organism_isLink(game->tabletop, from, to) )
    {
        Game_sendMsgTo(game, "Info: wrong movement - no edge\n", playerID);
        actionSleep( game, playerID );
        return;
    }

    Organism* minion= start->cells[0];
    if( minion->owner != playerID )
    {
        Game_sendMsgTo(game, "Info: wrong movement - not a player's minion\n", playerID);
        actionSleep( game, playerID );
        return;
    }

    if( minion->attrs[PIECE_ACTIVED] )
    {
        Game_sendMsgTo(game, "Info: wrong movement - already activated minion\n", playerID);
        actionSleep( game, playerID );
        return;
    }

    // Bound the strengh
    if( minion->attrs[PIECE_STRENGH] < strengh )
        strengh= minion->attrs[PIECE_STRENGH];
    

    // Target
    if( target->size >= 1 )
    {
        Organism* host= target->cells[0];
        if( host->owner == playerID ) // if friends: merge
        {
            host->attrs[PIECE_STRENGH]+= strengh;
            host->attrs[PIECE_ACTIVED]= 1;
        }
        else
        {
            // Let fight...
            int defence= host->attrs[PIECE_STRENGH];
            int reminder= fight( host, strengh );

            game->scores[playerID]+= defence - host->attrs[PIECE_STRENGH];

            if( host->attrs[PIECE_STRENGH] <= 0 )
            {
                Organism_deleteCell( target, 0 );
            }

            if( reminder > 0 )
            {
                Organism* fork= Organism_popCellAs( target, minion );
                fork->attrs[PIECE_STRENGH]= reminder;
                fork->attrs[PIECE_ACTIVED]= 1;
            }
        }
    }
    else // if nobody: move
    {
        Organism* fork= Organism_popCellAs( target, minion );
        fork->attrs[PIECE_STRENGH]= strengh;
        fork->attrs[PIECE_ACTIVED]= 1;
    }

    // Release the start node:
    minion->attrs[PIECE_STRENGH]-= strengh;
    if( minion->attrs[PIECE_STRENGH] <= 0 )
    {
        Organism_deleteCell( start, 0 );
    }
}

int fightDeterminist( Organism* target, int strengh )
{
    int defence= target->attrs[PIECE_STRENGH];

    while( defence > 0 && strengh > 0 )
    {
        int s= strengh;
        int overNumber= 0;
        if( strengh-defence > 0)
            overNumber= strengh-defence;
        strengh-= 1+((defence*DICE_DEFENCE)/DICE_FACE);
        defence-= 1+((s+overNumber*OVER_NUMBER_RATIO)*DICE_ATTACK)/DICE_FACE;
    }

    if( defence < 0 )
        defence= 0;

    if( strengh < 0 )
        strengh= 0;

    target->attrs[PIECE_STRENGH]= defence;
    return strengh;
}

void playZombies( Game* game )
{
    printf("Gogogo zombies %d\n", (int)(game->status) );
}

// Configure Tabletop
//-------------------
Organism * generateRandomTabletop( Organism * tabletop )
{
//    Organism * tabletop= Organism_new("Risky-Random", 0, size);
    tabletop->shape= 120.f;

    //puts("    Create a first piece    ");
    Organism* cell= Organism_addCell(
        tabletop, Organism_newPosition("Cell", 0.f, 0.f) );
    cell->color= 0x767680FF;
    
    //puts("    generate random cells    ");
    Organism_cellsAtRandom(tabletop, tabletop->capacity-1, tabletop->cells[0]);
    Organism_cellsAtMinDistance(tabletop, 8.f);
    Organism_nameCells(tabletop, "Cell");

    //puts("    generate the gabriel graph    ");
    Organism_linksGabrielGraph(tabletop);

    return tabletop;
}


Organism * generateClassicalTabletop( Organism* tabletop )
{
    Organism_destroy( tabletop );
    Organism_construct(tabletop, 0, "Risky", 0, 0, 0.f,0.f, 14);
    Organism_addCell( tabletop, Organism_newPosition("Cell-00", -18.f,  0.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-01", 18.f,  0.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-02", -12.f,  6.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-03", 12.f,  6.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-04", -12.f, -6.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-05",  12.f, -6.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-06", -6.f, 12.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-07",  6.f, 12.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-08", -6.f, -6.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-09",   6.f, -6.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-10",   0.f,  6.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-11",   0.f, 00.f) );

    Organism_addCell( tabletop, Organism_newPosition("Cell-12",   -20.f, -12.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-13",   20.f, -12.f) );

    Organism_biconnect(tabletop, 0, 2);
    Organism_biconnect(tabletop, 0, 4);
    Organism_biconnect(tabletop, 2, 6);
    Organism_biconnect(tabletop, 2, 10);
    Organism_biconnect(tabletop, 2, 11);
    Organism_biconnect(tabletop, 2, 4);
    Organism_biconnect(tabletop, 4, 8);
    Organism_biconnect(tabletop, 6, 7);
    Organism_biconnect(tabletop, 8, 11);
    Organism_biconnect(tabletop, 8, 9);
    Organism_biconnect(tabletop, 10, 11);
    Organism_biconnect(tabletop, 1, 3);
    Organism_biconnect(tabletop, 1, 5);
    Organism_biconnect(tabletop, 3, 7);
    Organism_biconnect(tabletop, 3, 10);
    Organism_biconnect(tabletop, 3, 11);
    Organism_biconnect(tabletop, 3, 5);
    Organism_biconnect(tabletop, 5, 9);
    Organism_biconnect(tabletop, 9, 11);

    Organism_biconnect(tabletop, 0, 12);
    Organism_biconnect(tabletop, 1, 13);
    Organism_biconnect(tabletop, 12, 13);

    return tabletop;
}
