/*******************************************************************************************
*
*   HACKAGAME
*   Copyright (c) 2021-2022 Guillaume Lozenguez - Institut Mines-Telecom
*
********************************************************************************************/


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "hackagames-geometry.h"
#include "hackagames-tabletop.h"

//-----------------------------------//
//--   Constructor / Destructor    --//
//-----------------------------------//

void Organism_construct(Organism* self, int type, char* name, int owner, int attrs_size, float x, float y, int capacity)
{
    self->type= type;
    self->name= malloc( sizeof(char)*(strlen(name)+2) );
    self->owner= owner;
    strcpy( self->name, name );
    self->color=  0x808080FF;;
    self->position.x= x;
    self->position.y= y;
    self->shape= 1.f;
    
    // Attributs:
    self->attrs_size= max( attrs_size, 0);
    self->attrs= malloc( sizeof(int) * max( self->attrs_size, 1) );
    for( int i = 0 ; i < self->attrs_size ; ++i )
    {
        self->attrs[i]= 0;
    }

    // Contents:
    self->capacity= max(capacity, 1);
    self->cells= malloc( sizeof(Organism*)*self->capacity );
    self->size= 0;

    // Connectivity:
    self->links= malloc( sizeof(int*) * self->capacity );
    for( int i = 0 ; i < self->capacity ; ++i )
    {
        self->links[i]= malloc( sizeof(int)*1 );
        self->links[i][0]= 0;
    }
}

void Organism_constructAs(Organism* self, Organism* model)
{
    self->type= model->type;
    self->name= malloc( sizeof(char)*(strlen(model->name)+2) );
    strcpy( self->name, model->name );
    self->owner= model->owner;
    self->color= model->color;
    self->position= model->position;
    self->shape= model->shape;
    
    // Attributs:
    self->attrs_size= model->attrs_size;
    self->attrs= malloc( sizeof(int) * max( self->attrs_size, 1) );
    for( int i = 0 ; i < self->attrs_size ; ++i )
    {
        self->attrs[i]= model->attrs[i];
    }

    // Contents:
    self->capacity= model->capacity;
    self->cells= malloc( sizeof(Organism*)*self->capacity );
    self->size= model->size;
    for( int i = 0 ; i < self->size ; ++i )
    {
        self->cells[i]= Organism_newAs( model->cells[i] );
    }

    // Connectivity:
    self->links= malloc( sizeof(int*) * self->capacity );
    for( int i = 0 ; i < self->capacity ; ++i )
    {
        self->links[i]= malloc( sizeof(int)*(model->links[i][0]+1) );
        for( int ii=0 ; ii <= model->links[i][0] ; ++ii )
            self->links[i][ii]= model->links[i][ii];
    }
}

Organism * Organism_new(int type, char* name, int owner, int attrs_size, int capacity)
{
    Organism * self= malloc( sizeof(Organism) );
    Organism_construct(self, type, name, owner, attrs_size, capacity, 0.f, 0.f);
    return self;
}

Organism * Organism_newBasic(char* name)
{
    return Organism_new(0, name, 0, 1, 0);
}

Organism* Organism_newPosition(char* name, float x, float y)
{
    Organism * self= malloc( sizeof(Organism) );
    Organism_construct(self, 0, name, 0, 0, 0, x, y);
    return self;
}

Organism * Organism_newAs(Organism* model)
{
    Organism * self= malloc( sizeof(Organism) );
    Organism_constructAs(self, model);
    return self;
}

void Organism_destroy( Organism * self )
{
    free( self->name );
    
    // Attributs:
    free( self->attrs );

    // Contents:
    for( int i= 0; i < self->size ; ++i )
        Organism_delete( self->cells[i] );
    free( self->cells );

    // Connectivity:
    for( int i = 0 ; i < self->capacity ; ++i )
    {
        free( self->links[i] );
    }
    free( self->links );
}

void Organism_delete( Organism * self )
{
    Organism_destroy(self);
    free( self );
}

//-----------------------------------//
//--    Standard Obj. method       --//
//-----------------------------------//

void Organism_copy( Organism * self, Organism * model )
{
    Organism_destroy(self);
    Organism_constructAs(self, model);
}

void Organism_print(Organism * self)
{
    char cellBuffer[1024];
    printf( "%s (%d):\n",
            self->name,
            self->size );
    
    for( int i= 0 ; i < self->size ; ++i )
    {
        int cardinality= self->links[i][0];
        sprintf( cellBuffer, "- %s [%.2f, %.2f] %#06x - %d[",
                (char*)(self->cells[i]->name),
                self->cells[i]->position.x,
                self->cells[i]->position.y,
                self->cells[i]->color,
                cardinality);
        printf("%s", cellBuffer);

        if( cardinality > 0 )
        {
            sprintf( cellBuffer, "%d", self->links[i][1] );
             printf("%s", cellBuffer);
            for( int e= 2 ; e <= cardinality ; ++e )
            {
                sprintf( cellBuffer, ", %d", self->links[i][e] );
                 printf("%s", cellBuffer);
            }
        }
        printf("]\n");
    }
}

char* Organism_str( Organism * self, char* buffer )
{
    char cellBuffer[1024];
    sprintf( buffer, "%s (%d):\n",
            self->name,
            self->size );
    
    for( int i= 0 ; i < self->size ; ++i )
    {
        int cardinality= self->links[i][0];
        sprintf( cellBuffer, "- %s [%.2f, %.2f] %#06x - %d[",
                (char*)(self->cells[i]->name),
                self->cells[i]->position.x,
                self->cells[i]->position.y,
                self->cells[i]->color,
                cardinality);
        strcat(buffer, cellBuffer);

        if( cardinality > 0 )
        {
            sprintf( cellBuffer, "%d", self->links[i][1] );
            strcat(buffer, cellBuffer);
            for( int e= 2 ; e <= cardinality ; ++e )
            {
                sprintf( cellBuffer, ", %d", self->links[i][e] );
                strcat(buffer, cellBuffer);
            }
        }
        strcat(buffer, "]\n");
    }
    return buffer;
}

char* Organism_strAttributs( Organism * self, char* str)
{
    char buffer[1014];
    strcpy(str, self->name);
    sprintf( buffer, " %d attributs", self->attrs_size );
    strcat(str, buffer);

    for( int i= 0 ; i < self->attrs_size ; ++i )
    {
        sprintf( buffer, " %d", self->attrs[i] );
        strcat(str, buffer);
    }

    return str;
}

void Organism_setName( Organism * self, char* name )
{   
    free( self->name );
    self->name= malloc( sizeof(char)*(strlen(name)+2) );
    strcpy( self->name, name );
}

/**
 * @brief    set the phisic of an Organism @param self
 */

void Organism_setPhisic(Organism* self, float x, float y, float radius, int color)
{
    self->position.x= x;
    self->position.y= y;
    self->shape= radius;
    self->color= color;
}

//-----------------------------------//
//--      attributs managment      --//
//-----------------------------------//

int Organism_attribute( Organism* self, int i )
{
    return self->attrs[i];
}


char* Organism_attributsStr( Organism * self, char* buffer )
{
    char cellBuffer[1024];
    sprintf( buffer, "%s %d attributs",
        self->name,
        self->attrs_size );

    for( int i= 0 ; i < self->attrs_size ; ++i )
    {
        sprintf( cellBuffer, " %d", self->attrs[i] );
        strcat(buffer, cellBuffer);
    }

    return buffer;
}

void Organism_attributsFromStr( Organism * self, char* str )
{
    int bound= strlen(str)+1;
    char buffer[ bound ];
    int attributs[ bound ];
    int count= 0;
    int isep= 0;
    int start= 0;

    // Piece Name:
    while( str[isep] != ' ' )
        ++isep;
    
    strncpy( buffer, str, isep );
    buffer[isep]= '\0';
    Organism_setName(self, buffer);

    // Read card Attribute:
    start= start+isep+1;
    while( start < bound )
    {
        // get the appropriate str
        isep= 0;
        while( str[start+isep] != ' ' && str[start+isep] != '\0' )
        {
            ++isep;
        }
        strncpy( buffer, str+start, isep );
        buffer[isep]= '\0';
        
        // Reccord the attribute
        attributs[count]= atoi(buffer);
        ++count;
        
        // Next attribute
        start= start+isep+1;
    }

    // Update the structure:
    free( self->attrs );
    self->size= count;
    self->attrs= malloc( sizeof(int)*self->size );
    for( int i= 0 ; i < self->size ; ++i )
    {
        self->attrs[i]= attributs[i];
    }
}

//-----------------------------------//
//--        cell selection         --//
//-----------------------------------//

Organism* Organism_cell( Organism* self, int i )
{
    return self->cells[i];
}

int Organism_extremCellIdTo( Organism* self, int id )
{
    // Security:
    if( self->size <= 1 )
        return id;

    // test cells from 0 to id
    int extrem= 0;
    if( extrem == id )
        extrem= 1;

    float dist2= Float2_distance2( self->cells[id]->position, self->cells[extrem]->position);

    // and all the other until id
    for( int candidate= extrem+1 ; candidate < id ; ++candidate )
    {
        float test2= Float2_distance2( self->cells[id]->position, self->cells[candidate]->position);
        if( test2 > dist2 )
        {
            extrem= candidate;
            dist2= test2;
        }
    }

    // test cells from id to the last ones
    for( int candidate= max(extrem+1, id+1) ; candidate < self->size ; ++candidate )
    {
        float test2= Float2_distance2( self->cells[id]->position, self->cells[candidate]->position);
        if( test2 > dist2 )
        {
            extrem= candidate;
            dist2= test2;
        }
    }
    return extrem;
}

//-----------------------------------//
//--      network managment        --//
//-----------------------------------//

int Organism_cellCardinality( Organism* self, int i )
{
    return self->links[i][0];
}

int Organism_linkTargetId(Organism* self, int iCell, int iLink)
{
    return self->links[iCell][iLink+1];
}

Organism* Organism_linkTarget(Organism* self, int iCell, int iLink)
{
    return self->cells[ Organism_linkTargetId(self, iCell, iLink) ];
}

int Organism_biconnect(Organism* self, int i1, int i2)
{
    Organism_connect(self, i2, i1);
    return Organism_connect(self, i1, i2);
}

int Organism_connect(Organism* self, int i1, int i2)
{
    int newCardinality= self->links[i1][0]+1;
    int* newLinks= malloc( sizeof(int)*(newCardinality+1) );
    newLinks[0]= newCardinality;
    
    // copy links smaler than i2
    int i= 1;
    while( i < newCardinality && self->links[i1][i] < i2 )
    {
        newLinks[i]= self->links[i1][i];
        ++i;
    }

    // add i2:
    newLinks[i]= i2;
    int idInLinks= i;
    ++i;

    // copy links higher than i2
    while( i <= newCardinality )
    {
        newLinks[i]= self->links[i1][i-1];
        ++i;
    }

    //Switch
    free( self->links[i1] );
    self->links[i1]= newLinks;

    return idInLinks;
}

bool Organism_isLink( Organism* self, int i1, int i2 )
{
    for( int i= 1; i <= self->links[i1][0] ; ++i )
    {
        if( self->links[i1][i] == i2 ){
            return true;
        }
    }
    return false;
}

//-----------------------------------//
//--      Organism managment       --//
//-----------------------------------//

int Organism_resizeCapacity(Organism* self, int newCapacity)
{
    // Remove and free execive Miniatures
    int count_destroy= 0;
    while( newCapacity < self->size )
    {
        Organism_delete( self->cells[self->size-1] );
        free( self->links[self->size-1] );
        ++count_destroy;
    }

    // create the new aray of Pieces:
    Organism** newArray= malloc( sizeof(Organism*)*newCapacity );
    for( int i= 0 ; i < self->size ; ++i )
        newArray[i]= self->cells[i];

    // create the new links:
    int** newLinks= malloc( sizeof(int*)*newCapacity );
    for( int i= 0 ; i < self->size ; ++i )
        newLinks[i]= self->links[i];
    for( int i= self->size ; i < newCapacity ; ++i )
    {
        newLinks[i]= malloc( sizeof(int)*1 );
        newLinks[i][0]= 0;
    }

    // clean and flip the arrays:
    free( self->cells );
    free( self->links );
    self->cells= newArray;
    self->links= newLinks;
    self->capacity= newCapacity;

    // report on the Miniatures destruction:
    return count_destroy;
}

Organism* Organism_addCell( Organism* self, Organism* aPiece)
{
    if( self->size == self->capacity )
        Organism_resizeCapacity( self, self->size+1 );
    
    self->cells[ self->size ]= aPiece;
    self->size= self->size+1;

    return self->cells[ self->size-1 ];
}

Organism* Organism_popCellAs( Organism* self, Organism* model )
{
    return Organism_addCell( self, Organism_newAs(model) );
}

void Organism_cellsAtRandom(Organism* self, int number, Organism* model)
{
    int expectedSize= self->size+number;
    float randomFactorX = self->shape*2 / RAND_MAX;
    float randomFactorY = self->shape*2 / RAND_MAX;

    if( expectedSize < self->capacity )
        Organism_resizeCapacity( self, expectedSize );
    
    for( int i=0 ; i < number ; ++i )
    {
        Organism* newOrganism= Organism_popCellAs( self, model );
        newOrganism->position.x = -(self->shape) + (rand() * randomFactorX );
        newOrganism->position.y = -(self->shape) + (rand() * randomFactorY );
    }
}

void Organism_nameCells( Organism* self, char* baseName )
{
    /** generate names **/
    char* pieceName= malloc( sizeof(char)*(strlen(baseName)+12) );
    int ten= min(10, self->size);
    for( int i= 0 ; i < ten ; ++i )
    {
        sprintf(pieceName, "%s-0%d", baseName, i);
        Organism_setName( self->cells[i], pieceName );
    }
    for( int i= 10 ; i < self->size ; ++i )
    {
        sprintf(pieceName, "%s-%d", baseName, i);
        Organism_setName( self->cells[i], pieceName );
    }
    free(pieceName);
}

int Organism_minDistance_it(Organism* self, float minDist)
{
    float minDist2= minDist*minDist;
    int correction= 0;
    for( int i= self->size-1; i > 0 ; --i )
    {
        Organism* piece1= self->cells[i];
        for( int ii= 0 ; ii < i ; ++ii )
        {
            Organism* piece2= self->cells[ii];
            float dist2= Float2_distance2( piece1->position, piece2->position );
            if( dist2 < minDist2 )
            {
                Float2_repultion( &(piece1->position),
                                    &(piece2->position),
                                    minDist - sqrtf( dist2 ) );
                    ++correction;
            }
        }
    }
    return correction;
}

int Organism_cellsAtMinDistance(Organism* self, float minDist)
{
    int count= 0;
    int correction= Organism_minDistance_it(self, minDist);
    while( correction > 0 && count < 10)
    {
        correction= Organism_minDistance_it(self, minDist);
        count+= 1;
    }
    return count;
}

Organism* Organism_removeCell( Organism* self, int iCell )
{
    Organism* aCell= self->cells[iCell];
    self->cells[iCell]= self->cells[ self->size-1 ];
    self->size= self->size-1;
    return aCell;
}

void Organism_deleteCell(Organism* self, int iCell)
{
    Organism* aCell= Organism_removeCell( self, iCell );
    Organism_delete(aCell);
}

//-----------------------------------//
//--       Pieces managment        --//
//-----------------------------------//

Organism* Organism_addPieceOn( Organism* self, int iOrganism, Organism* aPiece)
{
    return Organism_addCell( self->cells[iOrganism], aPiece );
}

//-----------------------------------//
//--      Tabletop managment       --//
//-----------------------------------//

//  Pieces connections :
int Organism_linksGabrielGraph(Organism* self)
{
    int connection= 0;
    for (int i = 0; i < self->size - 1; ++i)
    {
        Organism* cell1= self->cells[i];
        for (int ii = i+1; ii < self->size; ++ii)
        {
            Organism* cell2= self->cells[ii];
            int compteurPointDansLeCercle = 0;
            float xm = 0.5 * (cell1->position.x + cell2->position.x);
            float ym = 0.5 * (cell1->position.y + cell2->position.y);
            Float2 M = {xm, ym};
            float distNodeCenterOfij = Float2_distance( cell1->position, M);
            for (int k = 0; k < self->size; k++)
            {
                if ( Float2_distance(self->cells[k]->position, M) < distNodeCenterOfij - 0.05)
                {
                    compteurPointDansLeCercle++;
                }
            }
            if (compteurPointDansLeCercle == 0)
            {
                Organism_connect(self, i, ii);
                Organism_connect(self, ii, i);
                ++connection;
            }
        }
    }
    return connection;
}
