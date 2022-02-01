/*******************************************************************************************
*
*   Tabletop basic viewer
*   Copyright (c) 2020-2020 Guillaume Lozenguez
*
********************************************************************************************/

#include "raylib.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "toghap.h"

// Program attributes
//-------------------
const int screenWidth = 800;
const int screenHeight = 600;

void game_update(Tabletop * Tabletop);

// Game attributes
//-----------------
bool game_end;
char buffer[2048]= "";

int main(int nbArg, char ** arg)
{
    // Game Initialization
    //--------------------
    game_end= false;
    Tabletop * Tabletop= Tabletop_new(8);
    Game* game= Game_new(Tabletop, 0);
    Viewer * board= Viewer_new(game);

    Viewer_initialize(board, screenWidth, screenHeight);

    Tabletop_randomNodes(Tabletop, -30, -20, 30, 20);

/*
    Node_set( Tabletop->nodes, (Vector2){2.4f, 2.28f}, RED );
    Node_set( Tabletop->nodes+1, (Vector2){4.0f, 4.8f}, MAGENTA );
    Node_set( Tabletop->nodes+2, (Vector2){3.4f, -1.2f}, BLUE );
    Node_set( Tabletop->nodes+3, (Vector2){-1.4f, 1.2f}, GREEN );
*/

/*
    Node_set( Tabletop->nodes, (Vector2){-4.669313f, -12.282730f}, RED );
    Node_set( Tabletop->nodes+1, (Vector2){10.976391f, -16.651062f}, MAGENTA );
    Node_set( Tabletop->nodes+2, (Vector2){-16.125477f, 12.798130f}, BLUE );
    Node_set( Tabletop->nodes+3, (Vector2){22.237793f, -14.889148f}, GREEN );
*/

    Tabletop_minDistance(Tabletop, 12.0f);
    Tabletop_gabrielGraph(Tabletop);
/*
    Tabletop_connect(Tabletop, 1, 2);
    Tabletop_connect(Tabletop, 2, 0);
    Tabletop_connect(Tabletop, 0, 1);
    Tabletop_connect(Tabletop, 0, 3);
    Tabletop_connect(Tabletop, 3, 1);
    Tabletop_connect(Tabletop, 0, 2);
    Tabletop_connect(Tabletop, 1, 3);
*/

    strcpy( Tabletop->nodes[0].name, "Node 0" );
    strcpy( Tabletop->nodes[1].name, "Node 1" );
    strcpy( Tabletop->nodes[2].name, "Node 2" );
    strcpy( Tabletop->nodes[3].name, "Node 3" );

    // Some verificcations
    //--------------------
    puts( Tabletop_str(Tabletop, buffer) );
    puts( Viewer_str(board, buffer) );

    Vector2 position= {1.f, 1.f};
    position= Viewer_pixelFromPosition( board, position );
 //   printf("[%.2f,%.2f]\n", position.x, position.y);

    int calldown= 1200;
    // Main game loop
    while (!game_end && !WindowShouldClose())    // Detect window close button or ESC key
    {
        Viewer_control(board);
        game_update(Tabletop);
        Viewer_draw(board);

        if( calldown < 1 )
            game_end= true;
        calldown-= 1;
    }

    // proper closing
    //---------------
    Viewer_close(board);
    Tabletop_delete(Tabletop);
    Game_delete(game);
 
    return 0;
}

void game_update(Tabletop * Tabletop)
{

}
