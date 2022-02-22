#ifndef HACKAGAMES_ORGANISM_H
#define HACKAGAMES_ORGANISM_H

struct Str_Organism {
    int type; //! a type id
    char* name; //! a name to the organism
    int owner; //player owner of the organism
    unsigned int color; //! color in format 0xRRGGBBAA
    Float2 position;
    float shape;
    //Polygon* shape;
    //Dop4* box;
    
    // Attributs:
    int attrs_size;
    int * attrs;
    
    // Contents:
    int capacity, size; // capacity of pieces and effective number of pieces.
    struct Str_Organism ** cells; // pieces at this position.
    
    // Connectivity:
    int** links;
};
typedef struct Str_Organism Organism;

//-----------------------------------//
//--   Constructor / Destructor    --//
//-----------------------------------//

/**  CONSTRUCT
 * 
 * @brief     Construc all the element of an empty Organism.
 * @param     self an empty Organism not yet constructed.
 * @param     attrs_size number of attributs.
 * @param     neighbours_size number of neibours.
 * @param     capacity potential number of pieces.
 * @param     self an empty Organism not yet constructed.
 * @return    The pointer to the new Tabletop.
 */
void Organism_construct(Organism * self,  int type, char* name, int owner, int attrs_size, float x, float y, int capacity);
void Organism_constructAs(Organism* self, Organism* model);

/**  NEW
 * 
 * @brief     Allocate the memory to store a Node
 * @return    The pointer to the new Tabletop.
 */
Organism * Organism_new(int type, char* name, int owner, int attrs_size, int capacity);
Organism* Organism_newAs(Organism* model);
Organism* Organism_newBasic(char* name); // a new cell: one attribut and 1 capacity of cells
Organism* Organism_newPosition(char* name, float x, float y);


/**  DISTROY
 * 
 * @brief    distroy the elements of a cell.
 * @param    self the Organism not to distroy.
 */
void Organism_destroy( Organism * self );


/**  DELETE
 * 
 * @brief    distroy and delete a cell.
 * @param    self the Organism not to delete.
 */
void Organism_delete( Organism * self );



//-----------------------------------//
//--    Standard Obj. method       --//
//-----------------------------------//

/**
 * @brief    Copy an Organism @param self from another @param model.
 */
void Organism_copy( Organism * self, Organism * model );

/**
 * @brief    print an Organism @param self on a @param buffer.
 */
void Organism_print( Organism * self );
char* Organism_str( Organism * self, char* buffer );
char* Organism_strAttributs( Organism * self, char* buffer );

/**
 * @brief    set the name of an Organism @param self as @param name.
 */
void Organism_setName( Organism* self, char* name );

/**
 * @brief    set the phisic of an Organism @param self
 */
void Organism_setPhisic(Organism* self, float x, float y, float radius, int color);


//-----------------------------------//
//--      attributs managment      --//
//-----------------------------------//

/**
 * @brief    get the value of attribut @param i of an Organism @param self
 */
int Organism_attribute( Organism* self, int i );

char* Organism_attributsStr( Organism * self, char* buffer );
void Organism_attributsFromStr( Organism * self, char* str );


//-----------------------------------//
//--        cell selection         --//
//-----------------------------------//

Organism* Organism_cell( Organism* self, int i );
int Organism_extremCellIdTo( Organism* self, int i );

//-----------------------------------//
//--      network managment        --//
//-----------------------------------//

int Organism_cellCardinality( Organism* self, int i );
int Organism_linkTargetId(Organism* self, int i, int l);
Organism* Organism_linkTarget(Organism* self, int i, int l);
int Organism_connect(Organism* self, int i1, int i2);
int Organism_biconnect(Organism* self, int i1, int i2);
bool Organism_isLink( Organism* self, int i1, int i2 );

//-----------------------------------//
//--      Organism managment       --//
//-----------------------------------//

Organism* Organism_addCell( Organism* self, Organism* aCell);
Organism* Organism_popCellAs( Organism* self, Organism* aModel );
void Organism_nameCells( Organism * self, char* baseName );
int Organism_resizeCapacity(Organism* self, int capacity);
void Organism_cellsAtRandom(Organism* self, int number, Organism* model);
int Organism_cellsAtMinDistance(Organism* self, float minDist);
void Organism_deleteCell(Organism* self, int iCell);

//-----------------------------------//
//--       Pieces managment        --//
//-----------------------------------//

Organism* Organism_addPieceOn( Organism* self, int iOrganism, Organism* aPiece);

//-----------------------------------//
//--      Tabletop managment       --//
//-----------------------------------//

//  Pieces connections :
int Organism_linksGabrielGraph(Organism* self);
//int Organism_pieceskMeans(Tabletop* self);

// Tools :
//float Tabletop_distance2( Tabletop* self, int id1, int id2);
//int Tabletop_nextNode( Tabletop* self, int id );
//int Tabletop_extremNodeTo( Tabletop* self, int id );

#endif