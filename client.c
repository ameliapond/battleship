#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#define BUFFER_SIZE		256

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
	int 		socket_descriptor, 	/* descripteur de socket */
			longueur, 		/* longueur d'un buffer utilisé */
			state = 0;		/* Client's state for the game */
    	sockaddr_in 	adresse_locale; 	/* adresse de socket local */
    	hostent *	ptr_host; 		/* info sur une machine hote */
	servent *	ptr_service; 		/* info sur service never used*/    	
	char 		buffer[BUFFER_SIZE];
	char *		prog;   		/* nom du programme never used */    	  	
	char *		host; 			/* nom de la machine distante */
    	char *		mesg = NULL;		/* message envoyé */	
				
	if (argc != 2) 
	{
		perror("usage : client <adresse-serveur>");
		exit(1);
    	}
    	
	host = argv[1];   
	mesg = (char*)malloc(BUFFER_SIZE * sizeof(char));
	mesg[0] = '0';
	mesg[1] = '\0';

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
		//printf("reponse du serveur :  \n");
		write(1,buffer,longueur);
    	}
	
    	close(socket_descriptor);
	while(1)
	{  
		// Get the caractere typed by player.
	    	fgets(mesg,10,stdin);
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
			write(1,buffer,longueur);
    		}
    		close(socket_descriptor);  
	}
  	exit(0);
}
