/* serveur.c */

/*----------------------------------------------
Serveur à lancer avant le client
------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h> 	/* pour les sockets */
#include <sys/socket.h>
#include <netdb.h> 		/* pour hostent, servent */
#include <string.h> 		/* pour bcopy, ... */  
#include <pthread.h>		/* pour les threads */
#include "game.h"

#define TAILLE_MAX_NOM 256

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

void traitement(int sock){
	char buffer[256];
	char message[60] = "connexion reussie.\nEn attente d'un autre joueur...\n";
	int longueur;
	const char *connexion = "connexion\0";
 	int i;
  
    	if ((longueur = read(sock, buffer, sizeof(buffer))) > 0){
		for (i=0;i<10;i++){
			printf("%c",buffer[i]);
		}    		
		if (strcmp((const char*)buffer,connexion)){ 		/* strcmp return 0 when the stings are equals... */
    			write(sock,message,strlen(message)+1);
			printf("test serveur\n");
    		}
	}
}

void main(int argc, char **argv){
	    int 		socket_descriptor, 		/* descripteur de socket */
				nouv_socket_descriptor, 	/* [nouveau] descripteur de socket */
				longueur_adresse_courante; 	/* longueur d'adresse courante d'un client */
	    sockaddr_in 	adresse_locale, 		/* structure d'adresse locale*/
				adresse_client; 		/* adresse client courant */
	    hostent*		ptr_hote; 			/* les infos recuperees sur la machine hote */
	    servent*		ptr_service; 			/* les infos recuperees sur le service de la machine */
	    char 		machine[TAILLE_MAX_NOM+1]; 	/* nom de la machine locale */
	    game 		GAME[5];			/* parties en cours ( 5 simultanées maximum */
	    int 		listeAttente[10]=		/* liste des joueurs en attente de partie ( 10 joueur max ) */
				{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};		

	    gethostname(machine,TAILLE_MAX_NOM);		/* recuperation du nom de la machine */

	    /* recuperation de la structure d'adresse en utilisant le nom */
	    if ((ptr_hote = gethostbyname(machine)) == NULL) {
			perror("erreur : impossible de trouver le serveur a partir de son nom.");
			exit(1);
	    }   
	    
	    /* initialisation de la structure adresse_locale avec les infos recuperees */			
	    /* copie de ptr_hote vers adresse_locale */    
	    bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
	    adresse_locale.sin_family		= ptr_hote->h_addrtype; 	/* ou AF_INET */
	    adresse_locale.sin_addr.s_addr	= INADDR_ANY; 			/* ou AF_INET */
	    adresse_locale.sin_port = htons(5000);
	    
	    /* creation de la socket */
	    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			perror("erreur : impossible de creer la socket de connexion avec le client.");
			exit(1);
	    }
	    /* association du socket socket_descriptor à la structure d'adresse adresse_locale */
	    if ((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
			perror("erreur : impossible de lier la socket a l'adresse de connexion.");
			exit(1);
	    }
	    
	    /* initialisation de la file d'ecoute */
	    listen(socket_descriptor,10); /* nombre max de client simultané 10 */

	    /* attente des connexions et traitement des donnees recues */
	    while(1) {
	    
			longueur_adresse_courante = sizeof(adresse_client);
			/* adresse_client sera renseigné par accept via les infos du connect */
			if ((nouv_socket_descriptor = accept(socket_descriptor, (sockaddr*)(&adresse_client), &longueur_adresse_courante))< 0){
				perror("erreur : impossible d'accepter la connexion avec le client.");
				exit(1);
			}
				
			/* ajout du joueur dans la liste d'attente */
			listeAttente[0] = nouv_socket_descriptor;

			/* traitement du message */		
			traitement(nouv_socket_descriptor);			
			close(nouv_socket_descriptor);	
	    }
}
