/* function.h */

/* 
 * @Brief Contains functions definitions. 
 */

#define _DIM1		10
#define _DIM2		10
#define BUFFER_SIZE	256
#define TAILLE_MAX_NOM	256
#define NB_CLIENT_MAX 	10

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
 * @Brief 	Check the filling of the table of client connected 
 * @Param	table	The table of integer to check 
 * @Return	0	If the table is not completely filled with clients IP adress.
 *		1	If the table is completely filled with clients IP adress.
 */
int isFilled(int table[NB_CLIENT_MAX])
{
	int i = 0;
	int filled = 1;
	for (i=0; i < NB_CLIENT_MAX; i++)
	{
		if ( table[i] == 0)
		{
			filled = 0;
		}
	}
	return filled;			 
}
	 
/* 
 * @Brief 			Handle the communication between clients and server 
 * @Param	sock		ID of the socket communication
 * @Param	adress_client 	Adress of the client's new incomming connection.
 * @Param	c_queue		Clients	List of clients connected at the same time on the server
 * @Param	*r_con		Points at the number of remaining connections of the server.
 */
void traitement (int sock, int adress_client, int c_queue[NB_CLIENT_MAX]) 
{
	char buffer[BUFFER_SIZE];
    	int longueur;

    	if (longueur = read(sock, buffer, sizeof(buffer)) <= 0) 
	{
		printf("Unexpected Error. read(sock,buffer,sizeof(buffer)) returns a negative value.\n");
	}
	// If the player connect himself for the first time to the server.
	else if((!strncmp(buffer,"00",2)) && (!isFilled(c_queue)))
	{	
		strcpy(buffer,"01Connexion reussie.\nBienvenue dans BATTLESHIP MASTER ALMA 1\nEntrez votre pseudo: ");
		write(sock,buffer,strlen(buffer)+1);
		// Adding client's IP adress in the table of client.
		add_to_queue(adress_client, c_queue);			
	}
	// If the name has been set.
	else if(!strncmp(buffer,"01",2))
	{			
		strcpy(buffer,"02Vous avez été enregistré dans la file d'attente.\nEn attente d'un autre joueur...\n");
		write(sock,buffer,strlen(buffer)+1);	
    	}
	// If the server found another player available.
	else if(!strncmp(buffer,"02",2))
	{			
		strcpy(buffer,"03Un adversaire a été trouvé...\nDébut de la partie.\nPlacez vos navires:\nx: ");
		write(sock,buffer,strlen(buffer)+1);	
    	}
	// If the party started.
	else if(!strncmp(buffer,"03",2))
	{			
		strcpy(buffer,"04y: ");
		write(sock,buffer,strlen(buffer)+1);	
    	}
	// Finally if the state is "--" the game is over.
	else
	{
		strcpy(buffer,"--exit!\n");
		write(sock,buffer,strlen(buffer)+1);	
	}
}
