/* game.h */

/* @brief definit la structure du jeu.
 * @attrg[512] 		 representation en chaine de caractere du plateau de jeu. 
 * @attr player1 	 informations concernant le joueur 1. Pour l'instant son @IP.
 * @attr player2 	 informations concernant le joueur 1. Pour l'instant son @IP.
 * @attr grille1[10][10] plateau du jeu du point de vue du joueur 1.
 * @attr grille1[10][10] plateau du jeu du point de vue du joueur 2.
 * @warning 		 Les valeurs dans les grilles correspondent a leur contenu:
 *			 0 ocean.
 * 			 1 deja tiré ici.
 *			 2 bateau présent.
 */

#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include <string.h>

#define SIZE_PLATEAU 	350
#define START_MATCH 	 29
#define SHIP_NUMBER	  5

typedef struct game{
	char plateau[SIZE_PLATEAU];
	player player1;
	player player2;
	int tour;
} game;

/* @Brief 	Initialise un tableau de SIZE_PLATEAU caracteres avec une grille en ascii pour une partie
 * de bataille navale.
 */ 
void initStringGrille(char grille[SIZE_PLATEAU]){
	char battlefield[SIZE_PLATEAU] = "    1 2 3 4 5 6 7 8 9 10\n  1 . . . . . . . . . . \n  2 . . . . . . . . . . \n  3 . . . . . . . . . . \n  4 . . . . . . . . . . \n  5 . . . . . . . . . . \n  6 . . . . . . . . . . \n  7 . . . . . . . . . . \n  8 . . . . . . . . . . \n  9 . . . . . . . . . .\n 10 . . . . . . . . . . \n";
	int i;
	
	for ( i = 0; i < SIZE_PLATEAU; i ++){
		grille[i] = battlefield[i];
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
 * @Brief 	Initialise les joueur d'une partie en donnant leur ardresse IP.
 */ 	
void initGame(char p1[20], char p2[20], game *GAME){
	
	int i;

	for( i = 0; i < 20; i++)
	{
		GAME->player1.adresse_ip[i] = p1[i];
		GAME->player2.adresse_ip[i] = p2[i];
	}

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
int isPlaceable(int x, int y, int grille[10][10]){
	
	int place = 0;
	
	/* 
   	 *  If the coordinates are in range and if the position has never
	 *  been tageted and doesn't have any ship sailing on it.
	 */
	if ((x >= 0 && x <= 10) && (y >= 0 && y <= 10))
	{
		if ( grille[x][y] == 0 )
		{
			place = 1;
		}
	}
	return place;
}

/* @Description 	Place un bateau dans la grille du joueur correspondant..
 * @Param	x	x position of the boat.
 * @Param	y	y position of the boat.
 */
void setShip(int x, int y, int table[10][10]){ table[x-1][y-1] = 2; }

/* @Brief	Set the player given as parameter grid with ships it
 *		will place.
 */
void setShips(game *GAME, player p){

	int x, y, i = 0;
	
	while ( i < MAX_SHIP )
	{
		printf("coordonnees x: ");
		scanf("%d",&x);
		printf("coordonnees y: ");
		scanf("%d",&y);
		printf("p.grille[7][8]:%d\n",p.grille[7][8]);
		if (isPlaceable(x, y, p.grille))
		{
			setShip(x, y, p.grille);
			i++;
		}
		else
		{	
			printf("coordonees invalides!\n");
	
		}
	}
}

/*
 * @Brief 	Initialise la grille en mettant toutes les cases a 0 (ocean). 
 */
void initGrille(int grille[10][10]){
	int i;
	int j;
	for ( i = 0; i < 10; i++){
		for ( j = 0; j < 10; j++){
			grille[i][j] = 0;
		}
	}
}	

	
/* 
 * @Descrption	Actualise plateau en fonction de grille
 * @param 	plateau[] 		Tableau contenant la version chaine de caractere du champ de bataille.
 * @param	grille[][] 		Tableau d'entier contenant la configuratin de la map du joueur a qui elle appartient.
 * @param	matching_table[][]	Tableau d'entier contenant la position des elements de la map dans plateau[]
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
 * @Description Fait la correspondance entre le tableau
 * 		tab[10][10] du joueur et du plateau 
		plateau[char size] du jeu.
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
 * @Brief 	Gere le deroulement d'une partie.
 */
void play(game *GAME, char adrs_ip1[20], char adrs_ip2[20]){

	int match_table[10][10], i, x, y;
	char pseudo1[SIZE_NAME];	
	char pseudo2[SIZE_NAME];
	player p1,p2;

	/* remplissage de la chaine de caracteres representant le plateau de jeu */
	initStringGrille(GAME->plateau);
	
	/* Initialisation du tableau de correspondance plateau(char) et du tableau 10 x 10 des joueurs. */
	initMatchTable(match_table);
	
	/* Matching du plateau(char) et du tableau du joueur1 */
	matchGrids_int_to_string(GAME->plateau, GAME->player1.grille, match_table);
	display_char_table(GAME->plateau);
	
	/* Initialisation du joueur 1.*/
	printf("Saisissez votre pseudo: ");
	scanf("%s",pseudo1);
	initPlayer(p1, adrs_ip1,pseudo1);

	/* Initialisation du joueur 2. */
	printf("Saisissez votre pseudo: ");
	scanf("%s",pseudo2);
	initPlayer(p2, adrs_ip2,pseudo2);	

	GAME->player1 = p1;
	GAME->player2 = p2;
	
	/* La partie continue tant qu'aucun des joueurs n'a tous ses bateau coulé.*/ 
	while(GAME->player1.sailing_ship > 0 || GAME->player2.sailing_ship > 0)
	{	
		setShips(GAME, GAME->player1);
		setShips(GAME, GAME->player2);
		
		matchGrids_int_to_string(GAME->plateau, GAME->player1.grille, match_table);
		display_char_table(GAME->plateau);
		matchGrids_int_to_string(GAME->plateau, GAME->player1.grille, match_table);
		display_char_table(GAME->plateau);

		GAME->player1.sailing_ship = 0;
	}
}

/*	

   1 2 3 4 5 6 7 8 9 10
 1 . . . . . . . . . . 
 2 . . . . . . . x x . 
 3 . . o . . . . . . . 
 4 . . . . . . . . . . 
 5 . . . . . . . . . . 
 6 . . . . . . . . . . 
 7 . . . . o x . . . . 
 8 . 0 . . . . . . . . 
 9 . . . . . . . . . . 
10 . . . . . . . . . .

o : navire placé a flot. 
x : zone déjà déja tiré.
. : océan.
 
 table de correspondance entier -> chaine
 
 29  31  33  35  37  39  41  43  45  47
 54  56  58  60  62  64  66  68  70  72
 79  81  83  85  87  89  91  93  95  97
104 106 108 110 112 114 116 118 120 122
129 131 133 135 137 139 141 143 145 147
154 156 158 160 162 164 166 168 170 172
179 181 183 185 187 189 191 193 195 197
204 206 208 210 212 214 216 218 220 222
229 231 233 235 237 239 241 243 245 247  
253 255 257 259 261 263 265 267 269 271

*/
