/* game.h */

#include <stdio.h>
#include <stdlib.h>
#include "player.h"

#ifndef STRING_H
#include <string.h>
#endif

#define SIZE_PLATEAU 	350
#define START_MATCH 	29
#define SHIP_NUMBER		2
#define NB_CLIENT_MAX 	10

/* @brief definit la structure du jeu.
 * @attr	plateau[]		Game ASCII map.
 * @attr	*player1 		Pointer toward an instance of the first player.
 * @attr 	*player2 		Pointer toward an instance of the second player.
 * @attr 	tour			Game turn.			 
 * @attr	g_state			Equal 0 if the game is not running and 1 otherwise.
 * @warning 		 		The integers values in the grid match the following meanings:
 *			 		0 ocean/sea.
 * 			 		1 Have been shot already.
 *			 		2 Ship sailing there.
 */
typedef struct game{
	int tour;   	
	char plateau[SIZE_PLATEAU];		
	player *player1;
	player *player2;
	int g_state;
} game;

/* 
 * @Brief 	Check if all the games in the table are running. 
 * @Param	jeux	The table of game to check .
 * @Return	0	If all the games in the table aren't running ( i.e. haven't two player initialized).
 *		1	If the table is completely filled with game which are running.
 */
int isAvailable(game jeux[NB_CLIENT_MAX/2])
{
	int i = 0;
	int filled = 1;
	
	while ((filled != 0) && (i < NB_CLIENT_MAX/2))
	{
		if ( jeux[i].g_state != 2)
		{
			filled = 0;
		}
		i++;
	}
	return filled;			 
}

/* @Brief 			Check if a game is available in the list of game in parameter.
 * @Param	listGame	Pointer at the list of games.		 
 * @Param	size_listGame	Size of the list of games.
 * @Return	-1		If the game table is filled. Otherwise return the index of 
 *				the first game which is not running. (i.e waiting for a first player
 				or waiting for a second player ).
 * @Warning			Return the first available game's index found.
 *		
 */	
int waiting_game(game games[NB_CLIENT_MAX/2])
{
	int i = 0;

	while((i < NB_CLIENT_MAX/2)  &&  (games[i].g_state == 2))
	{	
		i++;
	}
	if (i == (NB_CLIENT_MAX/2))
	{ 
		return -1;
	}
	else
	{
		return i;
	}
}

/* 
 * @Brief	Display the table of charater given in parameter. 
 */
void display_char_table(char *table){

	int i;
	size_t size = strlen(table);

	for (i = 0; i < size; i++)
	{
		printf("%c",table[i]);
	}
}

/* 
 * @Brief	Set the characters's table with the ASCII model of the battlefield. 
 */ 
void initStringGrille(char grille[SIZE_PLATEAU]){
	char battlefield[SIZE_PLATEAU] = "    1 2 3 4 5 6 7 8 9 10\n  1 . . . . . . . . . . \n  2 . . . . . . . . . . \n  3 . . . . . . . . . . \n  4 . . . . . . . . . . \n  5 . . . . . . . . . . \n  6 . . . . . . . . . . \n  7 . . . . . . . . . . \n  8 . . . . . . . . . . \n  9 . . . . . . . . . .\n 10 . . . . . . . . . . \n";
	int i;
	
	for ( i = 0; i < SIZE_PLATEAU; i ++){
		grille[i] = battlefield[i];
	}
}

/*
 * @Brief 	Set the IP players's IP adress.
 * @Param 	p1	Player 1 IP adress.
 * @Param 	p2	Player 2 IP adress.
 */ 	
void initGame(int p1, int p2, game *GAME)
{
	GAME->player1->adresse_ip = p1;
	GAME->player2->adresse_ip = p2;
	GAME->tour = 0;
}


/* 
 * @Brief 	Check if a ship is placeable to the position 
 * @Param	x 		x position to check.
 * @Param	y 		y position to check.
 * @Param	grille[][]	grille du joueur.	
 * @Return	1 if the position is valide to set a boat,
 *		0 else.
 */
int isPlaceable(int x, int y, player *p){
	
	int place = 0;
	
	/* 
   	 *  If the coordinates are in range and if the position has never
	 *  been tageted and doesn't have any ship sailing on it.
	 */
	if ((x >= 0 && x <= 10) && (y >= 0 && y <= 10))
	{
		if ( p->grille[x-1][y-1] == 0 )
		{
			place = 1;
		}
	}
	return place;
}

/* @Description 	Set a ship in the player's grid.
 * @Param	x	x position of the boat.
 * @Param	y	y position of the boat.
 * @Param	p	Pointer toward a struct player.
 */
void setShip(int x, int y, player *p){ p->grille[x-1][y-1] = 2; }

/* @Brief	Set the player given as parameter grid with ships it
 *		will place.
 */
void setShips(game *GAME, player *p){

	int x, y, i = 0;
	
	printf("%s, placez vos navire.\n", p->name);
	
	/* This loop should be outside to use the client-server communication properly */
	while ( i < MAX_SHIP )
	{
		printf("navire %d x: ",i+1);
		scanf("%d",&x);
		printf("navire %d y: ",i+1);
		scanf("%d",&y);
		
		if (isPlaceable(x, y, p))
		{
			setShip(x, y, p);
			i++;
		}
		else
		{	
			printf("coordonees invalides!\n");
		}
	}
}

/*
 * @Brief 	Initialize the grid by filling it with zeros. 
 */
void initGrille(int grille[10][10])
{
	int i;
	int j;
	for ( i = 0; i < 10; i++){
		for ( j = 0; j < 10; j++){
			grille[i][j] = 0;
		}
	}
}	

	
/* 
 * @Descrption		Updates plateau[] with grille[]. 
 * @param 		plateau[] 		Table containing the ASCII version of the battlefield.
 * @param		grille[][] 		Contains the map of the corresponding player. 
 * @param		matching_table[][]	Contains the positions of characters to match with the player's grid.
 */ 
void matchGrids_int_to_string(char plateau[SIZE_PLATEAU], int grille[10][10], int matching_table[10][10]){

	int i, j;

	for( i = 0; i < 10 ; i++)
	{
		for(j = 0; j < 10; j++)
		{			
			switch(grille[i][j])
			{		
				case 0: plateau[matching_table[i][j]] = '.'; break; 
				case 1: plateau[matching_table[i][j]] = '0'; break;
				case 2: plateau[matching_table[i][j]] = 'x'; break;		
				default:break;
			}
		}
	}
}

/*
 * @Description 	Initializes the position of the usefull characters 
 *			in the ASCII version of the battlefield with their 
 *			relative position in a table[10][10].
 */
void initMatchTable(int table[10][10]){
	
	int 	i, 
		j,
		_match = START_MATCH;
	
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			table[i][j] = _match;
			_match += 2;

		}
		if ( i < 8 ){
			_match += 5;
		}
		else{ 
			_match += 4;
		}
	}
}

/*
 * @Brief 
 * @Param 	x 	x position to verify if it's possible to shot.
 * @Param	y	y position to verify if it's possible to shot.
 *			Positions are typing by the player.
 * @Param	player	Pointer toward the player that will get shot.
 * @Return	1	If the position can be shot ( i.e. if it's in range and
 *			has never been shot before ).
 *		0 else.
 */
int isStrikable(int x, int y, player *p){

	int strikable = 0;
	
	/* 
   	 *  If the coordinates are in range and if the position has never
	 *  been shot before.
	 */
	if ((x >= 0 && x <= 10) && (y >= 0 && y <= 10))
	{
		if ( p->grille[x-1][y-1] != 1 )
		{
			strikable = 1;
		}
	}
	return strikable;
}

/*
 * @Brief Allows to player one to fire in the area given as parameter.
 * @Param 	x	x position to fire.
 * @Param 	y 	y position to fire.
 * @Param 	p2 	A pointer toward the player that will get shot. 
 * @Return	0  	Shot in the middle of the ocean...nothing's happen.
 *		1	Touché ! coulé ! OS ( One Shot ) The ship which was at this 
 *			position doesn't exist anymore...
 */
int strike(int x, int y, player *p){
	
	int result_of_strike = 0;
	
	/* If the area selected have a ship */
	if ( p->grille[x-1][y-1] == 2)
	{
		result_of_strike = 1;
		p->grille[x-1][y-1] = 1;
	}
	return result_of_strike;
}

/*
 * @Brief 	Manage the game play.
 */
void play(game *GAME, int adrs_ip1, int adrs_ip2){

	int	match_table[10][10], 
		strike_result = 0,		
		leave_game = 0,	
		game_turn = 1,				// game_turn = 1 -> player1 plays else player2 plays. 
		i, 
		x, 
		y;		
	char	pseudo1[SIZE_NAME],	
		pseudo2[SIZE_NAME];		
	player	*p1 = (player*)malloc(sizeof(player)),
		*p2 = (player*)malloc(sizeof(player));
	 	
		
	/* Fill the ASCII table of the battlefield */
	initStringGrille(GAME->plateau);
	
	/* Initialisation du tableau de correspondance plateau(char) et du tableau 10 x 10 des joueurs. */
	initMatchTable(match_table);
	
	/* Matches the ASCII table with the player one grid */
	matchGrids_int_to_string(GAME->plateau, GAME->player1->grille, match_table);
	display_char_table(GAME->plateau);
	
	/* Initialize the first player. */
	printf("Saisissez votre pseudo: ");
	scanf("%s",pseudo1);
	initPlayer(p1, adrs_ip1, pseudo1, -1);

	/* Initialize the second player. */
	printf("Saisissez votre pseudo: ");
	scanf("%s",pseudo2);
	initPlayer(p2, adrs_ip2, pseudo2, -1);	

	GAME->player1 = p1;
	GAME->player2 = p2;
	
	/* Players put down their ships */
	setShips(GAME, GAME->player1);
	setShips(GAME, GAME->player2);

	/* The game plays until a player get it's all ships destroyed */ 
	while(!(GAME->player1->sailing_ship == 0) && !(GAME->player2->sailing_ship == 0))
	{	
		switch (game_turn)
		{
			case 1: /* Player 1 turn game */
			
				/* matching ASCII battlefield version with the table of the first player. */
				matchGrids_int_to_string(GAME->plateau, GAME->player1->grille, match_table);
				sleep(2);				
				system("clear");
				display_char_table(GAME->plateau);
				
				/* Typing of the shot area's coordinates */
				printf("%s ,choisissez une zone de tir: \nx: ", GAME->player1->name);
				scanf("%d", &x);
				printf("y: ");
				scanf("%d", &y);
			
				while(!isStrikable(x, y, GAME->player2))
				{
					/* Typing of the shot area's coordinates */
					printf("%s, zone de tir invalide!: \nx: ", GAME->player1->name);
					scanf("%d", &x);
					printf("y: ");
					scanf("%d", &y);
				}
								
				switch (strike(x, y, GAME->player2))
				{
					case 0:
						printf("raté\n");break;
					case 1:
						printf("Touché!\n");
						GAME->player2->sailing_ship--;
						printf("Il reste a votre adversaire %d navire a flot!\n", GAME->player2->sailing_ship);
						break;
					default:
						printf("Error strike's fail!");break;
				}

				/* Hand on the game turn to the second player */
				game_turn--;
				break;
				
			default : /* Player 2 turn game */

				/* matching ASCII battlefield version with the table of the second player. */
				matchGrids_int_to_string(GAME->plateau, GAME->player2->grille, match_table);
				sleep(2);
				system("clear");
 
				display_char_table(GAME->plateau);

				/* Typing of the shot area's coordinates */				
				printf("%s ,choisissez une zone de tir: \nx: ", GAME->player2->name);
				scanf("%d", &x);
				printf("y: ");
				scanf("%d", &y);

				while(!isStrikable(x, y, GAME->player1))
				{
					/* Typing of the shot area's coordinates */
					printf("%s, zone de tir invalide!: \nx: ", GAME->player2->name);
					scanf("%d", &x);
					printf("y: ");
					scanf("%d", &y);
				}
								
				switch (strike(x, y, GAME->player1))
				{
					case 0:
						printf("raté\n");
						break;
					case 1:
						printf("Touché!\n");
						GAME->player1->sailing_ship--;
						printf("Il reste a votre adversaire %d navire a flot!\n", GAME->player1->sailing_ship);
						break;
					default:
						printf("Error strike's fail!");
						break;
				}
				/* Hand on the game turn to the first player */
				game_turn++;
				break;
		}/* end switch(game_turn)*/
	}/* end while */
	
	/* 
	 * @Brief 	This message should be print to the two client's screen. 
	 */
	if (GAME->player1->sailing_ship == 0)
	{
		printf("%s wins!\n",GAME->player1->name);
	}
	else
	{
		printf("%s wins!\n",GAME->player2->name);
	}
}/* end play */
