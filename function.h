/* function.h */

/* 
 * @Brief Contains functions definitions. 
 */
#include "game.h"

#define _DIM1		10
#define _DIM2		10
#define BUFFER_SIZE	256
#define TAILLE_MAX_NOM	256
#define NB_CLIENT_MAX 	10

game	jeux[NB_CLIENT_MAX/2];

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
 * @Brief 			Handle the communication between clients and server 
 * @Param	sock		ID of the socket communication
 * @Param	adress_client 	Adress of the client's new incomming connection.
 * @Param	*jeux		List of the game running on the server.
 */
void traitement (int sock) 
{
	char buffer[BUFFER_SIZE];
    	int longueur;
    	int return_value = -1;
	int i;

    	if (longueur = read(sock, buffer, sizeof(buffer)) <= 0) 
	{
		printf("Error. read(sock,buffer,sizeof(buffer)) returns a negative value.\n");
	}

	// If the player connect himself for the first time to the server.
	else if((!strncmp(buffer,"00",2)) && (!isAvailable(jeux)))
	{	
		strcpy(buffer,"01Connexion reussie.\nBienvenue dans BATTLESHIP MASTER ALMA 1\nEntrez votre pseudo: ");
		write(sock,buffer,strlen(buffer)+1);
	}

	// If the name has been set.
	else if(!strncmp(buffer,"01",2))
	{
		// A new player is created.
		player *new_player = (player*)malloc(sizeof(player));
		char pseudo[SIZE_NAME];
		strncpy(pseudo,buffer+2,SIZE_NAME);
		initPlayer(new_player, sock, pseudo);
		return_value = waiting_game(jeux);

		// If there are remaining space in the table of game play.
		if (return_value != -1)
		{
			if (jeux[return_value].g_state == 1)
			{
				jeux[return_value].player2 = (player*)malloc(sizeof(player));
				jeux[return_value].player2 = new_player;
				jeux[return_value].g_state = 2;
			}
			else
			{
				jeux[return_value].player1 = (player*)malloc(sizeof(player));
				jeux[return_value].player1 = new_player;		
				jeux[return_value].g_state = 1;
			}
			strcpy(buffer,"02Vous avez été enregistré dans la file d'attente.\nEn attente d'un autre joueur...\n");
			write(sock,buffer,strlen(buffer)+1);
		}
		// If all the game play are already playing.
		else
		{
			strcpy(buffer,"--Le serveur recoit actuellement trop de connexion simultanées.\n Veuillez réessayer plus tard\n");
			write(sock,buffer,strlen(buffer)+1);
		}				
    	}
	// If the server found another player available.
	else if(!strncmp(buffer,"02",2)/*&& waiting_p un autre joueur est disponible dans la liste des jeux */)
	{			
		strcpy(buffer,"03Un adversaire a été trouvé...\nDébut de la partie.\nPlacez vos navires:\nx: \0");
		write(sock,buffer,strlen(buffer)+1);	
    	}
	// If the party started.
	else if(!strncmp(buffer,"03",2))
	{			
		strcpy(buffer,"04y: \0");
		write(sock,buffer,strlen(buffer)+1);	
    	}
	// Finally if the state is "--" the game is over.
	else if(!strncmp(buffer,"04",2))
	{
		strcpy(buffer,"--Voulez vous recommencer une partie?\n");
		write(sock,buffer,strlen(buffer)+1);	
	}
}
