/*******************************************************************************************
*
*   HACKAGAME
*   Copyright (c) 2021-2022 Guillaume Lozenguez - Institut Mines-Telecom
*
********************************************************************************************/

#include "dependancy.h"
#include "hackagames.h"

#include <errno.h> 
#include <unistd.h>     //close 
#include <arpa/inet.h>  //close 
#include <sys/poll.h>   //poll
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h>

#define TRUE   1 
#define FALSE  0 
#define PORT 14001

// Constructor / Destructor
Game * Game_new(Organism * tabletop, int nbPlayer)
{
    Game * self = malloc( sizeof(Game) );
    self->status= GAME_INITIALIZING;
    self->tabletop= tabletop;

    //Serveur parameters
    self->port= PORT;
    self->address.sin_family = AF_INET;  
    self->address.sin_addr.s_addr = INADDR_ANY;
    self->address.sin_port = htons( self->port );
    self->nbPlayer= nbPlayer;
    self->scores= malloc( sizeof(float)*(self->nbPlayer+1) );
    self->sockets= malloc( sizeof(int)*(self->nbPlayer+1) );
    self->turn= 0;

    //initialise all player data socket to 0, a color...
    for (int i = 0; i <= self->nbPlayer; i++)
    {
        self->sockets[i] = 0;
        self->scores[i] = 0;
    }

    return self;
}

void Game_delete(Game * self)
{
    Organism_delete( self->tabletop );
    free( self->sockets );
    free( self );
}

//Initialization:
void Game_resetTabletop(Game * self, Organism * tabletop)
{
    Organism_delete( self->tabletop );
    self->tabletop= tabletop;
}
void game_switchPlayers(Game * self)
{
    float sco1= self->scores[1];
    int sock1= self->sockets[1];
    
    for( int ip= 1 ; ip < self->nbPlayer ; ++ip )
    {
        self->scores[ip]= self->scores[ip+1];
        self->sockets[ip]= self->sockets[ip+1];
    }

    self->scores[self->nbPlayer]= sco1;
    self->sockets[self->nbPlayer]= sock1;
}

// Game Engine : 
void Game_startServeur(Game* self)
{
    // Update parameters
    self->address.sin_port = htons( self->port );

    //create a master socket 
    if( (self->sockets[0] = socket( self->address.sin_family, SOCK_STREAM , 0)) == 0)  
    {
        perror("socket failed");  
        exit(EXIT_FAILURE);
    }
    
    //set master socket to allow multiple connections , 
    //this is just a good habit, it will work without this
    int t= 1; 
    if( setsockopt( self->sockets[0], SOL_SOCKET, SO_REUSEADDR, (char *)&t, sizeof(t) ) < 0 )  
    {
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }

    //bind the socket
    if( bind( self->sockets[0], (struct sockaddr *)&(self->address), sizeof(self->address) ) < 0 )  
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("HackaGames waiting on port %d \n", self->port);

    //try to specify maximum of 3 pending connections for the master socket 
    if (listen( self->sockets[0], 3 ) < 0)  
    {  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }

    self->status= GAME_RUNNING;
}

void Game_waitOnPlayer( Game * self )
{
    int full= 0;
    fd_set readfds;

    while ( !full )
    {
        //clear the socket set
        FD_ZERO(&readfds);  

        //add master socket to set 
        FD_SET( self->sockets[0], &readfds );  
        int max_sd = self->sockets[0];  
             
        //add child sockets to set (read list)
        for (int i = 1 ; i <= self->nbPlayer ; ++i)  
        {  
            //socket descriptor
            int sd = self->sockets[i]; 
            if(sd > 0) FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
     
        //wait for an activity on one of the sockets, timeout is NULL, 
        //so wait indefinitely
        int activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        
        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");  
        }
    
        //Incoming connection on master socket:
        if (FD_ISSET(self->sockets[0], &readfds))
        {
            struct sockaddr_in address;
            address.sin_family = AF_INET;  
            address.sin_addr.s_addr = INADDR_ANY;  
            address.sin_port = htons( self->port );
            socklen_t addressLen= sizeof( address );

            int new_socket = accept( self->sockets[0], (struct sockaddr *)&address, &addressLen );
            if ( new_socket < 0 )  
            {
                perror("accept");  
                exit(EXIT_FAILURE);  
            }
            
            //send new connection greeting message
            char *message = "Info: Welcome on HackaGames\n";
            if( send(new_socket, message, strlen(message), 0) != (ssize_t)strlen(message) )
            {
                perror("send of welcom mesage");
            }
            
            //add new socket to array of sockets 
            for( int i = 1; i <= self->nbPlayer; ++i )  
            {  
                //if position is empty 
                if( self->sockets[i] == 0 )  
                {  
                    self->sockets[i] = new_socket;

                    printf( "New player-%d (socket: %d, ip: %s, port : %d)\n",
                    i, new_socket ,
                    inet_ntoa( address.sin_addr ),
                    ntohs( address.sin_port ) );

                    break;
                }  
            }
        }
        
        //else its some IO operation on some other socket
        for ( int i = 1; i <= self->nbPlayer; i++)  
        {  
            int sd = self->sockets[i];
                 
            if (FD_ISSET( sd , &readfds))  
            {  
                //Check if it was for closing , and also read the 
                //incoming message
                char buffer[1024];
                int valread = read( sd , buffer, 1024);
                if (valread == 0)  
                {  
                    //Somebody disconnected , get his details and print 
                    getpeername( sd, (struct sockaddr*)&(self->address), (socklen_t*)sizeof(self->address) );  
                    printf("Host disconnected , ip %s , port %d \n", inet_ntoa(self->address.sin_addr) , ntohs(self->address.sin_port));
                    
                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    self->sockets[i] = 0;  
                }
                    
                //Echo back the message that came in 
                else 
                { 
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    buffer[valread] = '\0';  
                    send(sd , buffer , strlen(buffer) , 0 );  
                }  
            }
        }
        // Stop waiting ?
        full= 1;
        for( int i= 1 ; i <= self->nbPlayer ; ++i )
        {
            if ( self->sockets[i] == 0 ) full= 0;
        }
    }
}

void Game_start(Game* self )
{
    self->status= GAME_INITIALIZING;
    Game_startServeur(self);
    Game_waitOnPlayer(self);
    self->status= GAME_RUNNING;
}

void Game_stop(Game * self )
{
    close( self->sockets[0] );
    self->status= GAME_END;
}

// Interactions :
void Game_sendMsgTo( Game* self, char* msg, int playerID )
{
    //send new connection greeting message
    if( send(self->sockets[playerID], msg, strlen(msg), 0) != (ssize_t)strlen(msg) )
    {
        perror("sending error");
    }
}

void Game_sendWakeUpTo( Game* self, int playerID )
{
    //send new connection greeting message
    char buffer[64], mesage[1024]= "";
    Organism* ttop= self->tabletop;
    sprintf(mesage,
        "Player: %d %d\nTabletop: %d\n",
        self->nbPlayer, playerID,
        ttop->size
    );
    Game_sendMsgTo(self, mesage, playerID);
    for( int i= 0 ; i < ttop->size ; ++i )
    {
        int card= Organism_cellCardinality(ttop, i);
        sprintf( mesage, "Node: %d %d edges:", i, card );
        for( int e= 0 ; e < card ; ++e )
        {
            sprintf( buffer, " %d", Organism_linkTargetId(ttop, i, e) );
            strcat(mesage, buffer);
        }
        strcat(mesage, "\n");

        Game_sendMsgTo(self, mesage, playerID);
    }
}

void Game_sendGameTo( Game* self, int playerID )
{
    char mesage[1024], buffer[64];

    // Count the number of miniature on the tabletop
    int count_miniatures= 0;
    for( int iNode= 0 ; iNode < self->tabletop->size ; ++iNode )
    {
        count_miniatures= count_miniatures + Organism_cell( self->tabletop, iNode )->size;
    }

    // Report that number
    sprintf( mesage, "Player: %d %d", playerID, self->nbPlayer);
    for( int iPlayer= 1 ; iPlayer <= self->nbPlayer ; ++iPlayer )
    {
        sprintf( buffer, " %2f", self->scores[iPlayer] );
        strcat(mesage, buffer);
    }
    strcat(mesage, "\n");
    Game_sendMsgTo(self, mesage, playerID);

    sprintf( mesage, "Game: %d %d\n", self->turn, count_miniatures );
    Game_sendMsgTo(self, mesage, playerID);

    // For each miniatures
    for( int iNode= 0 ; iNode < self->tabletop->size ; ++iNode )
    {
        Organism* cell= Organism_cell( self->tabletop, iNode );
        for( int iMinion= 0 ; iMinion < cell->size ; ++iMinion )
        {
            // report the miniature elements.
            Organism* minion=  Organism_cell( cell, iMinion );
            sprintf( mesage, "Piece: %d ", iNode );
            Organism_attributsStr( minion, buffer );
            strcat(mesage, buffer);
            strcat(mesage, "\n");
            Game_sendMsgTo(self, mesage, playerID);
        }
    }
}

void Game_sendEndTo( Game* self, int playerID )
{
    char mesage[1024];
    float scorePlayer= self->scores[playerID];

    // Player end result:
    int end= 1; //winner
    for( int i= 1 ; i <= self->nbPlayer ; ++i )
    {
        if ( i != playerID && self->scores[i] == scorePlayer )
            end= 0;
    }
    for( int i= 1 ; i <= self->nbPlayer ; ++i )
    {
        if ( i != playerID && self->scores[i] > scorePlayer )
            end= -1;
    }

    // Report that number
    sprintf( mesage, "End: %d\n", end);
    Game_sendMsgTo(self, mesage, playerID);
}

char* Game_getMessageFromPlayer( Game* self, int playerID, char* buffer )
{
    int sd = self->sockets[playerID];
    int valread = read( sd , buffer, 1024);
    if (valread == 0)  
    {  
        //Somebody disconnected , get his details and print 
        getpeername( sd, (struct sockaddr*)&(self->address), (socklen_t*)sizeof(self->address) );  
        printf("Host disconnected , ip %s , port %d \n", inet_ntoa(self->address.sin_addr) , ntohs(self->address.sin_port));
        
        //Close the socket and mark as 0 in list for reuse
        close( sd );
        self->sockets[playerID] = 0;

        //Secure the buffer
        buffer[0]= '\0';
    }
    //Echo back the message that came in 
    else 
    { 
        //set the string terminating NULL byte on the end 
        int end= valread-1;
        while( buffer[end] == ' ' || buffer[end] == '\n' || buffer[end] == '\r')
            --end;
        buffer[end+1] = '\0';
    }
    return buffer;
}

void Game_requestPlayer( Game* self, int playerID, Organism* anAction )
{
    char buffer[1024];

    // Trash mesage stack
    struct pollfd fds[1];
    fds[0].fd= self->sockets[playerID];
    fds[0].events= POLLIN;
    if( poll( fds, 1, 0) )
    {
        // Trash pending mesages (noise)
        Game_getMessageFromPlayer( self, playerID, buffer );
    }

    // Activate the player
    Game_sendMsgTo(self, "Your-turn:\n", playerID);

    // Wait for the answer
    Game_getMessageFromPlayer( self, playerID, buffer );

    // Generate the card
    Organism_attributsFromStr(anAction, buffer);
}

// Miniatures managment :
Organism* Game_addPiece( Game* self, int nodeID, Organism* aPiece )
{
    Organism* piece= Organism_addCell( self->tabletop->cells[nodeID], aPiece );
    return piece;
}

Organism* Game_popPieceAs( Game* self, int nodeID, Organism* aModel )
{
    Organism* piece= Organism_popCellAs( self->tabletop->cells[nodeID], aModel );
    return piece;
}

// Game Viewer :

