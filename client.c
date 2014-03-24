	#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "function.h"

typedef struct sockaddr 	sockaddr;
typedef struct sockaddr_in 	sockaddr_in;
typedef struct hostent 		hostent;
typedef struct servent 		servent;
/* 
 * @Warning	The following numbers indicates the several states of the client.
 *		0 The client is not yet connected to the server.
 *		1 The client is connected and sending the player's nickname.	
 *		2 
 *		3
 *		4
 *		5
 *		6
 *		7
 *		8
 *		9
 *		10
 *		11
 *		12
 *		13
 */
int main(int argc, char **argv) 
{  
	
    	sockaddr_in 	adresse_locale; 	/* adresse de socket local */
    	hostent *	ptr_host; 		/* info sur une machine hote */
	servent *	ptr_service; 		/* info sur service never used*/    	
	char 		buffer[BUFFER_SIZE];
	char *		prog;   		/* nom du programme never used */    	  	
	char *		host; 			/* nom de la machine distante */
    	char *		mesg = NULL;		/* message envoyé */	
	char		char_tmp_state[2];		/* temporary buffer helping to handle the states */
	int 		socket_descriptor, 	/* descripteur de socket */
			int_tmp_state,		/* temporary integer helping to handle the states */	
			longueur,i; 		/* longueur d'un buffer utilisé */
	if (argc != 2) 
	{
		perror("usage : client <adresse-serveur>");
		exit(1);
    	}
    	
	host = argv[1];   
	mesg = (char*)malloc(BUFFER_SIZE * sizeof(char));
	
	mesg[0] = '0';
	mesg[1] = '0';
	mesg[2] = '\0';
	
	if ((ptr_host = gethostbyname(host)) == NULL) 
	{
		perror("erreur : impossible de trouver le serveur a partir de son adresse.");
		exit(1);
    	}
	// copie caractere par caractere des infos de ptr_host vers adresse_locale 
	bcopy((char*)ptr_host->h_addr, (char*)&adresse_locale.sin_addr, ptr_host->h_length);
	adresse_locale.sin_family = AF_INET; // ou ptr_host->h_addrtype; 
	adresse_locale.sin_port = htons(5000);
	// creation de la socket 
	if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		perror("erreur : impossible de creer la socket de connexion avec le serveur.");
		exit(1);
	}
    	// tentative de connexion au serveur dont les infos sont dans adresse_locale 
    	if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) 
	{
		perror("erreur : impossible de se connecter au serveur.");
		exit(1);
    	}

    	//envoi du message vers le serveur 
    	if ((write(socket_descriptor, mesg, strlen(mesg))) < 0) 
	{
		perror("erreur : impossible d'ecrire le message destine au serveur.");
		exit(1);
	}      
	// lecture de la reponse en provenance du serveur 
	while((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) 
	{
		//Display the server answer
		write(1,buffer+2,longueur-1);
    	}
	
    	close(socket_descriptor);
	while(strncmp(buffer,"--",2))
	{
		// if the client is not waiting for an other client connection.
		if (strncmp(buffer,"02",2)!=0)
		{	
			fgets(mesg+2,SIZE_NAME,stdin);
			mesg[0] = buffer[0];
	    		mesg[1] = buffer[1];
		}

		mesg[0] = buffer[0];
	    	mesg[1] = buffer[1];
		
		if ((ptr_host = gethostbyname(host)) == NULL) 
		{
			perror("erreur : impossible de trouver le serveur a partir de son adresse.");
			exit(1);
	 	}
		// copie caractere par caractere des infos de ptr_host vers adresse_locale 
		bcopy((char*)ptr_host->h_addr, (char*)&adresse_locale.sin_addr, ptr_host->h_length);
		adresse_locale.sin_family = AF_INET; // ou ptr_host->h_addrtype;
		adresse_locale.sin_port = htons(5000);
		// creation de la socket 
		if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		{
			perror("erreur : impossible de creer la socket de connexion avec le serveur.");
			exit(1);
		}
    		// tentative de connexion au serveur dont les infos sont dans adresse_locale 
    		if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) 
		{
			perror("erreur : impossible de se connecter au serveur.");
			exit(1);
    		}
    		//envoi du message vers le serveur 
    		if ((write(socket_descriptor, mesg, strlen(mesg))) < 0) 
		{
			perror("erreur : impossible d'ecrire le message destine au serveur.");
			exit(1);
		}      
	    	// lecture de la reponse en provenance du serveur 
	    	while((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) 
		{
			// Displays the server answer 			
			write(1,buffer+2,longueur-2);
    		}
    		close(socket_descriptor);  
	}
  	exit(0);
}
