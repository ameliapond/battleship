/* function.h */

/**
 * @Brief Contains functions definitions. 
 */
 
#include "game.h"

#define _DIM1			10
#define _DIM2			10

#define BUFFER_SIZE		256
#define TAILLE_MAX_NOM	256
#define NB_CLIENT_MAX 	10

#define FIRST			1
#define SECOND			2
#define WAITING_P1		0
#define WAITING_P2		1
#define PLAYER2FOUND	2
#define WAITING_COORD	3
#define WAITING_COORD2	4	// Player one is waiting for player 2 to set his ships.
#define PLAYING			5

/**
 * @Brief 	Displayer a table of pointers at players.
 * 
 */
void display_player_table(player* table[NB_CLIENT_MAX])
{
	int i = 0;
	while (table[i]!= NULL)
	{
		printf("\njoueur %d",i);
		display_player(table[i]);
		
		i++;
	}
}

/**
 * @Brief 	Displayes connected players name.
 * 
 */
void display_players(player* table[NB_CLIENT_MAX])
{
	int i = 0;
	printf("\n     PLAYERS \n__________________\n");
	while (table[i]!= NULL)
	{
		printf("\n** %s",table[i]->name);		
		i++;
	}
	printf("\n__________________\n\n");
}

/**
 * @Brief 	give the index of the actual cleint in the table of client.
 *
 */
int getCurrentClient(int sock, player* table[NB_CLIENT_MAX])
{
	int index = -1;
	int i=0;

	while ((table[i] != NULL) && (i < NB_CLIENT_MAX) && ((table[i]->adresse_ip) != sock))
	{
		i++;
	}
	if ((table[i] != NULL) && ((table[i]->adresse_ip) == sock) && (i < NB_CLIENT_MAX))
	{
		index = i;
	}
	return index;
}

/**
 * @Brief 	Give the first free index of the table of client
 *		give as parameter.
 */
int getFreeIndex(player *table[NB_CLIENT_MAX])
{
	int i 		= 0;
	int index	= -1;

	while((table[i] != NULL) && (i < NB_CLIENT_MAX)  )
	{
		i++;
	}
	if (table[i] == NULL && i < NB_CLIENT_MAX)
	{
		index = i;			
	}
	return index;
}

/**
 *@Brief	Display a table of pointers at integer.
 */
void display_pointer_int_table(int *tab, int size_tab)
{
	int i;
	for (i = 0; i < size_tab; i++)
	{
		printf("tab[%d]:%d\n",i,tab[i]);	
	}
}

/**
 * @Brief	Displays a two dimensions's integers table.
 */ 
void display_int_int_table(int tableau[_DIM1][_DIM2])
{
	int i, j;
	
	for (i = 0; i < _DIM1; i++)
	{
		for (j = 0; j < _DIM2; j++)
		{
			printf("%d ", tableau[i][j]);
		}
		printf("\n");
	}
}

/**
 * @Brief 	Initialize a matrice of integer.
 * @Warning	The matrice in parameter should have both
 *		dimensions declared to prevent an indetermined 
 *		behavior.
 */
void init_int_int_table(int table[_DIM1][_DIM2])
{	
	int i , j ;

	for (i = 0; i < _DIM1; i++)
	{
		for ( j = 0; j < _DIM2; j++)
		{
			table[i][j] = 0;
		}
	}
}

void init_char_table(char *tab, char c)
{
	int i;
	
	for (i = 0; i < SIZE_NAME; i++)
	{
		tab[i] = c;
	}
}

/** 
 * @Brief 				Add a client to the queue of clients if it isn't filled.
 * @Param	clients_ip	The client's IP that will be added to the queue. 
 * @Param	c_queue		The queue of all clients connected to the server.
 * @Param	*r_connect	Number of connections the server can still accept.
 * @Return				Return the index of c_queue containing the new added IP adress.
 * @Warning				Size of c_queue has been defined in macro named SIZE_OF_CLIENT.
 */
int add_to_queue(int client_ip, int *c_queue)
{
	int i = 0;
	while((i < NB_CLIENT_MAX) && (c_queue[i] != 0))
	{
		i++;
	}
	c_queue[i] = client_ip;
	return i;
}

/**
 * @Brief 				Return the index of a player waiting for a game play 
 *					inside de the table of players.
 * @Param	currentSocketID		Current client socket ID.
 * @Warning				The player's current socket ID is not returned.
 */
int waitingPlayer(player *table[10], int currentSocketID)
{
	int i=0;
	int index = -1;

	while(((table[i] != NULL ) 
		&& (i < NB_CLIENT_MAX)
		&& (table[i]->p_state != WAITING_P2)) 
		|| 
		((table[i] != NULL ) && (table[i]->adresse_ip == currentSocketID)))
	{
		i++;
	}
	if ((table[i] != NULL) && (i < NB_CLIENT_MAX))
	{
		index = i;
	}
	return index;
}
