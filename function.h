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

game	jeux[NB_CLIENT_MAX/2];

player* newPlayers[NB_CLIENT_MAX];

/*
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

/*
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

/*
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

/*
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

/*
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

/* 
 * @Brief 			Add a client to the queue of clients if it isn't filled.
 * @Param	clients_ip	The client's IP that will be added to the queue. 
 * @Param	c_queue		The queue of all clients connected to the server.
 * @Param	*r_connect	Number of connections the server can still accept.
 * @Return			Return the index of c_queue containing the new added IP adress.
 * @Warning			Size of c_queue has been defined in macro named SIZE_OF_CLIENT.
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

/*
 * @Brief 	Return the index of a player waiting for a game play 
 *			inside de the table of players.
 */
int waitingPlayer(player *table[10])
{
	int i=0;
	int index = -1;

	while((table[i] != NULL ) && (i < NB_CLIENT_MAX) && (table[i]->p_state != WAITING_P2))
	{
		i++;
	}
	if ((table[i] != NULL) && (i < NB_CLIENT_MAX))
	{
		index = i;
	}
	return index;
}

/* 
 * @Brief 			Handle the communication between clients and server 
 * @Param	nouv_socket_descriptor		ID of the socket communication
 * @Param	adress_client 	Adress of the client's new incomming connection.
 * @Param	*jeux		List of the game running on the server.
 */
void traitement (int nouv_socket_descriptor) 
{
	player *new_player;
	int	longueur;
	int	i;
	int	return_value 	= -1;
	int 	coord[2]	= {-1,-1};
	int 	position;
	int 	index;
	int 	indexOpponent	=-1;	
	char	c_state[2] 	= {'0','0'};
	char	buffer[BUFFER_SIZE];

	newPlayers[0] = (player*)malloc(sizeof(player));
	initPlayer(newPlayers[0], 221531, "Penelope",-1);
	newPlayers[0]->grille[1][1] = SAILING_SHIP;
	newPlayers[0]->grille[2][2] = SAILING_SHIP;
	newPlayers[0]->p_state = WAITING_P2;
	newPlayers[0]->sailing_ship = MAX_SHIP;
	jeux[0].g_state = WAITING_P2;
	jeux[0].player1 = (player*)malloc(sizeof(player));
	jeux[0].player1 = newPlayers[0];
	jeux[0].player1->gamePosition[0] = 0;
	jeux[0].player1->gamePosition[1] = FIRST;
	
	longueur = read(nouv_socket_descriptor, buffer, sizeof(buffer));

	if (longueur < 0) 
	{
		printf("Error. read(nouv_socket_descriptor,buffer,sizeof(buffer)) returns a negative value.\n");
	}

	// If the player connect himself for the first time to the server.
	else if((!strncmp(buffer,"00",2)) && (!isAvailable(jeux)))
	{	
		strcpy(buffer,"01Connexion reussie.\n\n\nBienvenue dans BATTLESHIP MASTER ALMA 1\n\n\nEntrez votre pseudo: \0");
		write(nouv_socket_descriptor,buffer,strlen(buffer)+1);
	}

	// If the name has been set.
	else if(!strncmp(buffer,"01",2))
	{
		printf("state 1 reached\n");
		// A new player is created.
		if( position = getFreeIndex(newPlayers) != -1)
		{
			// Dynamic memory allocation for the player in the table of players
			newPlayers[position] = (player*)malloc(sizeof(player));
			char pseudo[SIZE_NAME];
			strncpy(pseudo,buffer+2,SIZE_NAME);
			initPlayer(newPlayers[position], nouv_socket_descriptor, pseudo, -1);
			newPlayers[position]->p_state = SETTING_SHIP;	
			
			// set the position of player in list of game.	
			newPlayers[position]->gamePosition[0] = waiting_game(jeux);
			printf("state 1.1 reached\n");
			// If there are remaining space in the table of game play.
			if ( newPlayers[position]->gamePosition[0] != -1)
			{
				printf("state 1.1.1 reached\n");
				if (jeux[newPlayers[position]->gamePosition[0]].g_state == WAITING_P2)
				{
					printf("state 1.1.1.1 reached\nindexOponent: %d\n",waitingPlayer(newPlayers));
					jeux[newPlayers[position]->gamePosition[0]].player2 = (player*)malloc(sizeof(player));
					// Set the new player in place "player2" in the game play.
					newPlayers[position]->gamePosition[1] = SECOND;
					newPlayers[position]->p_state = PLAYER2FOUND;			
					jeux[newPlayers[position]->gamePosition[0]].player2 = newPlayers[position];					
					jeux[newPlayers[position]->gamePosition[0]].g_state = PLAYER2FOUND;
					strcpy(buffer,"02Vous avez été enregistré dans la file d'attente.\nEn attente d'un autre joueur...\n");
					write(nouv_socket_descriptor,buffer,strlen(buffer)+1);
				}
				
				// Set the new player in place "player1" in the game play.
				else if(jeux[newPlayers[position]->gamePosition[0]].g_state == WAITING_P1)
				{
					printf("state 1.1.1.2 reached\n");
					jeux[newPlayers[position]->gamePosition[0]].player1 = (player*)malloc(sizeof(player));
					// Set the new player in place "player1" in the game play.
					newPlayers[position]->gamePosition[1] = FIRST;
					newPlayers[position]->p_state =	WAITING_P2;
					jeux[newPlayers[position]->gamePosition[0]].player1 = newPlayers[position];		
					jeux[newPlayers[position]->gamePosition[0]].g_state = WAITING_P2;
					strcpy(buffer,"02Vous avez été enregistré dans la file d'attente.\nEn attente d'un autre joueur...\n");
					write(nouv_socket_descriptor,buffer,strlen(buffer)+1);
				}
			}
			// If all the game play are already playing.
			else
			{
				strcpy(buffer,"--Le serveur recoit actuellement trop de connexion simultanées.\n Veuillez réessayer plus tard\n");
				write(nouv_socket_descriptor,buffer,strlen(buffer)+1);
			}				
	    }
	}
	// If the server found another player available.
	else if((!strncmp(buffer,"02",2)) && (indexOpponent = waitingPlayer(newPlayers) != -1))
	{			
		printf("state 2 reached\n");
		strcpy(buffer,"03Un adversaire a été trouvé!\nVeuillez placez vos navires:\n");
		write(nouv_socket_descriptor,buffer,strlen(buffer)+1);	

		if ((index = getCurrentClient(nouv_socket_descriptor, newPlayers)) != -1)
		{
			printf("state 2.1 reached\n");
			jeux[index].g_state == WAITING_COORD;
    	}
	}
	// If the players are positioning their ships.
	else if( (!strncmp(buffer,"03",2)) && (index = getCurrentClient(nouv_socket_descriptor, newPlayers) != -1))
	{
		printf("state 3 reached\n");
		if (newPlayers[index]->sailing_ship < MAX_SHIP)
		{
			printf("state 3.1 reached\n");
		// entering coordinates and placing the ships			
			strcpy(buffer,"04x: \0");
			write(nouv_socket_descriptor,buffer,strlen(buffer)+1);
		}
	}
	else if(!strncmp(buffer,"04",2))
	{
		printf("state 4 reached\n");
		// index shouldn't be equals to -1.
		index = getCurrentClient(nouv_socket_descriptor, newPlayers);
		
		// adding x coordinate
		c_state[0] = buffer[2];
		c_state[1] = buffer[3]; 
		newPlayers[index]->tmp[0] = atoi(c_state);
		strcpy(buffer,"05y: \0");			
		write(nouv_socket_descriptor,buffer,strlen(buffer)+1);	
    }
	else if(!strncmp(buffer,"05",2))
	{			
		printf("state 5 reached\n");
		index = getCurrentClient(nouv_socket_descriptor, newPlayers);
		
		// adding y coordinate
		c_state[0] = buffer[2];
		c_state[1] = buffer[3]; 
		newPlayers[index]->tmp[1] = atoi(c_state);
		
		if (isPlaceable(newPlayers[index]->tmp[0],newPlayers[index]->tmp[1],newPlayers[index]))
		{
			printf("state 5.1 reached\n");
			// next ship.
			newPlayers[index]->sailing_ship++;
			setShip(newPlayers[index]->tmp[0],newPlayers[index]->tmp[1],newPlayers[index]);	
			if (newPlayers[index]->sailing_ship == MAX_SHIP)
			{			
				printf("state 5.1.1 reached\n");
				strcpy(buffer,"06\0");
				write(nouv_socket_descriptor,buffer,strlen(buffer)+1);
			}
			else if (newPlayers[index]->sailing_ship < MAX_SHIP)
			{
				printf("state 5.1.2 reached\n");
				strcpy(buffer,"03\0");
				write(nouv_socket_descriptor,buffer,strlen(buffer)+1);
			}
		}
		else if (!isPlaceable(newPlayers[index]->tmp[0],newPlayers[index]->tmp[1],newPlayers[index]))
		{	
			printf("state 5.2 reached\n");
			strcpy(buffer,"03Les coordonnees indiquees sont invalides...\nVeuillez saisir a nouveau.\n\0");
			write(nouv_socket_descriptor,buffer,strlen(buffer)+1);
		}
	}
	else if ((!strncmp(buffer,"06",2)))
	{
		printf("state 6 reached\n");
		index = getCurrentClient(nouv_socket_descriptor, newPlayers);
		strcpy(buffer,"07Les navires ont ete places.\nDebut de la partie\n\0");
		jeux[index].g_state = PLAYING;
		write(nouv_socket_descriptor,buffer,strlen(buffer)+1);
	}
	// Finally if the state is "--" the game is over.
	else if(!strncmp(buffer,"07",2))
	{
		printf("state 7 reached\n");
		strcpy(buffer,"08Voulez vous recommencer une partie?\n");
		write(nouv_socket_descriptor,buffer,strlen(buffer)+1);	
	}
	else
	{	
		printf("state 8 reached\n");
		strcpy(buffer,"08ERREUR : Un probleme est survenu pendant la partie.\nDéconnexion.\n");
		write(nouv_socket_descriptor,buffer,strlen(buffer)+1);	
	}
	return;
}
/*
void * traitement(void* soc)
{
	int* tmp = (int*)soc;
	int nouv_socket_descriptor = *tmp;
	renvoi(nouv_socket_descriptor);
	close(nouv_socket_descriptor);
	return NULL;
}
*/
