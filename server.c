#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h> 	/* pour les sockets */
#include <sys/socket.h>
#include <netdb.h> 		/* pour hostent, servent */
#include <string.h> 		/* pour bcopy, ... */  
#include <pthread.h>
#include "game.h"
#include "function.h"

#define TAILLE_MAX_NOM	256
#define NB_CLIENT_MAX 	10

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

main(int argc, char **argv) 
{  
	int		socket_descriptor, 		/* descripteur de socket */
			nouv_socket_descriptor, 	/* [nouveau] descripteur de socket */
			longueur_adresse_courante, 	/* longueur d'adresse courante d'un client */
			*clients_queue =  NULL;		/* Queue containing the clients's IP adresses. */
    	sockaddr_in 	adresse_locale, 		/* structure d'adresse locale*/
			adresse_client_courant; 	/* adresse client courant */
    	hostent*	ptr_hote; 			/* les infos recuperees sur la machine hote */
    	servent*	ptr_service; 			/* recuperees sur le service de la machine*/
    	char 		machine[TAILLE_MAX_NOM+1]; 	/* nom de la machine locale */
	pthread_t	client_t;
	
	// recuperation du nom de la machine 
    	gethostname(machine,TAILLE_MAX_NOM);		
	// Initialization of list of clients IP adresses.	
	clients_queue = malloc(NB_CLIENT_MAX * sizeof(int));
	/* recuperation de la structure d'adresse en utilisant le nom */
	if ((ptr_hote = gethostbyname(machine)) == NULL) 
    	{
		perror("erreur : impossible de trouver le serveur a partir de son nom.\n");
		exit(1);
    	}
    	// initialisation de la structure adresse_locale avec les infos recuperees 			
    	// copie de ptr_hote vers adresse_locale 
    	bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
    	adresse_locale.sin_family		= ptr_hote->h_addrtype; 	/* ou AF_INET */
   	adresse_locale.sin_addr.s_addr	= INADDR_ANY; 			/* ou AF_INET */
    	adresse_locale.sin_port = htons(5000);
    	/*ntohs(ptr_service->s_port)*/
    	printf("numero de port pour la connexion au serveur : %d \n", ntohs(adresse_locale.sin_port));
    	/* creation de la socket */
    	if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		perror("erreur : impossible de creer la socket de connexion avec le client.\n");
		exit(1);
    	}
    	/* association du socket socket_descriptor à la structure d'adresse adresse_locale */
    	if ((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) 
	{
		perror("erreur : impossible de lier la socket a l'adresse de connexion.\n");
		exit(1);
    	}
    	/* initialisation de la file d'ecoute */
    	listen(socket_descriptor,10);
    	/* attente des connexions et traitement des donnees recues */
    	for(;;) 
	{
		longueur_adresse_courante = sizeof(adresse_client_courant);
		/* adresse_client_courant sera renseigné par accept via les infos du connect */	
		if ((nouv_socket_descriptor = accept(socket_descriptor, (sockaddr*)(&adresse_client_courant),
			       &longueur_adresse_courante))< 0) 
		{
			perror("erreur : impossible d'accepter la connexion avec le client.\n");
			exit(1);
		}
		// traitement du message
		traitement(nouv_socket_descriptor, 
		adresse_client_courant.sin_addr.s_addr, 
		clients_queue);
		close(nouv_socket_descriptor);		
    	}
}
