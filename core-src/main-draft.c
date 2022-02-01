/*******************************************************************************************
*
*   HACKAGAME
*   Copyright (c) 2021-2022 Guillaume Lozenguez - Institut Mines-Telecom
*
********************************************************************************************/
#include "raylib.h"
#include "raymath.h"

#include "hackagames.h"
#include "hackagames-interface.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define NUM_PLAYER 0

// Game attributes
//-----------------
enum Piece_Attributes
{
    PIECE_ACTIVED= 0,
    PIECE_STRENGH,
    PIECE_SIZE
};

// Intermediate Functions
//-----------------------
void generateFixedTabletop(Organism* tabletop);
void generateRandomTabletop(Organism* tabletop, int size);

int main(int nbArg, char ** arg)
{
    int seed= time(NULL);
    if( nbArg > 1 )
        seed= atoi( arg[1] );
    srand( seed );

    printf("\n------------------\nTOGHAP Draft App: Start (random seed: %d)\n------------------\n", seed);

    // Game Initialization
    //--------------------
    puts("Create the tabletop");
    Organism* tabletop= Organism_new("Tabletop", 0, 1);
    tabletop->shape= 40.f;

    //generateRandomTabletop(tabletop, 24);
    generateFixedTabletop(tabletop);

    puts("Prints tabletops");
    Organism_print(tabletop);

    puts("Add somme pieces");
    Organism* piece= Organism_addPieceOn(tabletop, 0, Organism_new("Humans", PIECE_SIZE, 0) );
    Organism_setPhisic(piece, -0.5f, 0.5f, 0.4f, 0xFF0000FF);
    piece= Organism_addPieceOn(tabletop, 1, Organism_new("Zombies", PIECE_SIZE, 0) );
    Organism_setPhisic(piece, -0.5f, 0.5f, 0.4f, 0x0000FFFF);

    puts("Start the interface");
    Interface* frame= Interface_new(tabletop, 1200, 800, 24.f);
    Interface_startIHM( frame );


    // Main game loop
    //---------------
    puts("Start the main loop");
    int count= 0;
    while ( Interface_IHMIsOpen(frame)  )
    {
        ++count;
    }

    // Clean Stop 
    //-----------
    puts("Stop everythings");
    Interface_stopIHM(frame);

    puts("Delete frame");
    Interface_delete(frame);

    puts("Delete tabletop");
    Organism_delete( tabletop );

    puts("Ok by...");
    return 0;
}

void generateRandomTabletop(Organism* tabletop, int size)
{
    Organism_destroy( tabletop );
    Organism_construct(tabletop, "Tabletop", 0, size);

    //puts("    Create a first piece    ");
    Organism* cell= Organism_addCell( tabletop, Organism_new("-", 0, 1) );
    cell->color= 0x767680FF;

    //puts("    generate random cells    ");
    Organism_cellsAtRandom(tabletop, size, tabletop->cells[0]);
    Organism_cellsAtMinDistance(tabletop, 4.f);
    Organism_nameCells(tabletop, "Cell");

    //puts("    generate the gabriel graph    ");
    Organism_linksGabrielGraph(tabletop);
}

void generateFixedTabletop(Organism* tabletop)
{
    Organism_destroy( tabletop );
    Organism_construct(tabletop, "Tabletop12", 0, 12);
    Organism_addCell( tabletop, Organism_newPosition("Cell-00", 0, 1, -18.f,  0.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-01", 0, 1,  18.f,  0.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-02", 0, 1, -12.f,  6.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-03", 0, 1,  12.f,  6.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-04", 0, 1, -12.f, -6.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-05", 0, 1,  12.f, -6.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-06", 0, 1, -6.f, 12.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-07", 0, 1,  6.f, 12.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-08", 0, 1, -6.f, -6.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-09", 0, 1,   6.f, -6.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-10", 0, 1,   0.f,  6.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-11", 0, 1,   0.f, 00.f) );

    Organism_addCell( tabletop, Organism_newPosition("Cell-12", 0, 1,   -20.f, -12.f) );
    Organism_addCell( tabletop, Organism_newPosition("Cell-13", 0, 1,   20.f, -12.f) );

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
}
