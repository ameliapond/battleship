#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h> /* pour les sockets */
#include <sys/socket.h>
#include <netdb.h> /* pour hostent, servent */
#include <string.h> /* pour bcopy, ... */  
#include <pthread.h>

#include "function.h"

#define TAILLE_MAX_NOM 256

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

game	jeux[NB_CLIENT_MAX/2];

player* newPlayers[NB_CLIENT_MAX];

pthread_mutex_t lock;

/** 
 * @Brief 							Handle the communication between clients and server 
 * @Param	nouv_socket_descriptor	ID of the socket communication
 * @Param	adress_client 			Adress of the client's new incomming connection.
 * @Param	*jeux					List of the game running on the server.
 */
void renvoi (int nouv_socket_descriptor) 
{
	player *new_player;
	int	longueur;
	int	i;
	int	return_value 	= -1;
	int 	coord[2]	= {-1,-1};
	int 	position;
	int 	index;
	int 	indexOpponent	=-11;	
	char	c_state[2] 	= {'0','0'};
	char	buffer[BUFFER_SIZE];

	longueur = read(nouv_socket_descriptor, buffer, sizeof(buffer));
	// displaying of the players connected.
	display_players(newPlayers);

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
			pthread_mutex_lock(&lock);
			
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
					printf("state 1.1.1.1 reached");
					jeux[newPlayers[position]->gamePosition[0]].player2 = (player*)malloc(sizeof(player));

					// Set the new player in place "player2" in the game play.
					newPlayers[position]->gamePosition[1] = SECOND;
					newPlayers[position]->p_state = PLAYER2FOUND;			
					jeux[newPlayers[position]->gamePosition[0]].player2 = newPlayers[position];					
					jeux[newPlayers[position]->gamePosition[0]].g_state = PLAYER2FOUND;
					strcpy(buffer,"02Vous avez été enregistré dans la file d'attente.\nEn attente d'un autre joueur...\n");
					write(nouv_socket_descriptor,buffer,strlen(buffer)+1);
					display_player_table(newPlayers);
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
					display_player_table(newPlayers);
				}
			}

			// If all the game play are already playing.
			else
			{
				strcpy(buffer,"--Le serveur recoit actuellement trop de connexion simultanées.\n Veuillez réessayer plus tard\n");
				write(nouv_socket_descriptor,buffer,strlen(buffer)+1);
			}
			pthread_mutex_unlock(&lock);				
	    }
	}

	// If the server found another player available.
	else if((!strncmp(buffer,"02",2)) && ((indexOpponent = waitingPlayer( newPlayers, nouv_socket_descriptor)) != -1))
	{			
		printf("state 2 reached\n");
		strcpy(buffer,"03Un adversaire a été trouvé!\nVeuillez placez vos navires:\n");
		write(nouv_socket_descriptor,buffer,strlen(buffer)+1);	

		if ((index = getCurrentClient(nouv_socket_descriptor, newPlayers)) != -1)
		{
			printf("state 2.1 reached\n");
			pthread_mutex_lock(&lock);
			jeux[index].g_state == WAITING_COORD;
			pthread_mutex_unlock(&lock);
    		}
	}

	else if((!strncmp(buffer,"02",2)) && ((indexOpponent = waitingPlayer( newPlayers, nouv_socket_descriptor )) == -1))
	{
		//check if there is a new client every 3 seconds"
		pthread_mutex_lock(&lock);
		sleep(3);		
		strcpy(buffer,"02En attente d'un autre joueur...\n");
		write(nouv_socket_descriptor,buffer,strlen(buffer)+1);
		display_player_table(newPlayers);
		pthread_mutex_unlock(&lock);
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
		pthread_mutex_lock(&lock);
		c_state[0] = buffer[2];
		c_state[1] = buffer[3]; 
		newPlayers[index]->tmp[0] = atoi(c_state);
		strcpy(buffer,"05y: \0");			
		write(nouv_socket_descriptor,buffer,strlen(buffer)+1);	
		pthread_mutex_unlock(&lock);
    	}
	else if(!strncmp(buffer,"05",2))
	{			
		printf("state 5 reached\n");
		index = getCurrentClient(nouv_socket_descriptor, newPlayers);
		
		// adding y coordinate
		pthread_mutex_lock(&lock);
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
		pthread_mutex_unlock(&lock);
	}
	else if ((!strncmp(buffer,"06",2)))
	{
		pthread_mutex_lock(&lock);
		printf("state 6 reached\n");
		index = getCurrentClient(nouv_socket_descriptor, newPlayers);
		strcpy(buffer,"07Les navires ont ete places.\nDebut de la partie\nA votre tour!\nChoississez une cible:\0");
		jeux[index].g_state = PLAYING;
		write(nouv_socket_descriptor,buffer,strlen(buffer)+1);
		pthread_mutex_unlock(&lock);
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

void * traitement(void* soc)
{
	int* tmp = (int*)soc;
	int nouv_socket_descriptor = *tmp;
	renvoi(nouv_socket_descriptor);
	close(nouv_socket_descriptor);
	return NULL;
}

/***************************************************************************************************************************
*								      MAIN						   *
***************************************************************************************************************************/

main(int argc, char **argv) 
{ 
   	int 	socket_descriptor, 		/* descripteur de socket */
		nouv_socket_descriptor, 	/* [nouveau] descripteur de socket */
		longueur_adresse_courante; 	/* longueur d'adresse courante d'un client */
   		sockaddr_in adresse_locale, 	/* structure d'adresse locale*/
		adresse_client_courant; 	/* adresse client courant */
   		hostent* ptr_hote; 		/* les infos recuperees sur la machine hote */
   		servent* ptr_service; 		/* les infos recuperees sur le service de la machine */
   	char 	machine[TAILLE_MAX_NOM+1]; 	/* nom de la machine locale */

	/* Initialization of the mutex that helping to schedule concurent access*/    
	if (pthread_mutex_init(&lock, NULL) != 0)
    	{
    	    printf("\n mutex init failed\n");
    	    return 1;
    	}

   	gethostname(machine,TAILLE_MAX_NOM); 
   	if ((ptr_hote = gethostbyname(machine)) == NULL) 
	{
		perror("erreur : impossible de trouver le serveur a partir de son nom.");
		exit(1);
   	}
   
   	bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
   	adresse_locale.sin_family = ptr_hote->h_addrtype; /* ou AF_INET */
   	adresse_locale.sin_addr.s_addr = INADDR_ANY; /* ou AF_INET */

   	adresse_locale.sin_port = htons(5000);
   
  	printf("numero de port pour la connexion au serveur : %d \n",
  	ntohs(adresse_locale.sin_port));
   
   	if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0)
 	{
		perror("erreur : impossible de creer la socket de connexion avec le client.");
		exit(1);
   	}

   if ((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) 
	{
		perror("erreur : impossible de lier la socket a l'adresse de connexion.");
		exit(1);
   	}
   
   	listen(socket_descriptor,5);

   	for(;;) 
	{   
		longueur_adresse_courante = sizeof(adresse_client_courant);
		if ((nouv_socket_descriptor =
									accept(socket_descriptor,
      								(sockaddr*)(&adresse_client_courant),
      								&longueur_adresse_courante))
									< 0) 
		{
			perror("erreur : impossible d'accepter la connexion avec le client.");
			exit(1);
		}

		pthread_t nouveauClient;
		
		if(pthread_create(&nouveauClient, NULL, traitement, (int*)&nouv_socket_descriptor))
		{
			perror(">> Erreur lors de la creation du thread");
			return(1);
		}
   }
	
   close(socket_descriptor);  
   pthread_mutex_destroy(&lock);
}
/* TODO Il semblerait que les joueur ne sont pas ajouté au tableau de jeux. a réglé absolument. */
