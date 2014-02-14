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
#define SIZE_PLATEAU 350

typedef struct game{
	char plateau[SIZE_PLATEAU];
	char player1[20];
	char player2[20];
	int grille1[10][10];
	int grille2[10][10];
} game;

/* Initialise un tableau de 300 caractère avec une grille en ascii pour une partie
 * de bataille navale.$
 */ 
void initStringGrille(char grille[SIZE_PLATEAU]){
	char battlefield[SIZE_PLATEAU] = "    1 2 3 4 5 6 7 8 9 10\n  1 . . . . . . . . . . \n  2 . . . . . . . . . . \n  3 . . . . . . . . . . \n  4 . . . . . . . . . .\n  5 . . . . . . . . . . \n  6 . . . . . . . . . .\n  7 . . . . . . . . . . \n  8 . . . . . . . . . .\n  9 . . . . . . . . . . \n 10 . . . . . . . . . . \n";
	int i;
	for ( i = 0; i < SIZE_PLATEAU; i ++){
		grille[i] = battlefield[i];
	}
}

/*
 * @Description Initialise les joueur d'une partie en donnant leur ardresse IP.
 */ 	
void initGame(char p1[20], char p2[20], game *GAME){
	int i, j;
	
	for( i = 0; i < 20; i++){
		GAME->player1[i] = p1[i];
		GAME->player2[i] = p2[i];
	}
}

int play(game GAME){
	return 0;
}


/*
 * Initialise la grille en mettant toutes les cases a 0 (ocean). 
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

/* Renvoie 1 si le placement d'un bateau est possible, 
 * -1 si les coordonnees indiquees sont hors de la grille
 * 0 si il y a deja un navire a ces coordonnees.
 */
int navirePlacable(int x, int y, int grille[10][10]){
	int place; /* indique le type d'erreur pour le placement du bateau */
	
	/* coordinate out of range */
	if ((x >= 0 && x <= 10) && (y >= 0 && y <= 10)){
		place = -1;
	}
	/* Cell already used */
	if ( grille[x][y] == 2 ){
		place = 0;
	}
	/* ocean */
	else if(grille[x][y] == 0){
		place = 1;
	}
	return place;
}

/* 
 * affiche la grille du jeu 
 */
void afficheGrille(char grille[SIZE_PLATEAU]){
	int i;
	for (i = 0; i < SIZE_PLATEAU; i++){
		printf("%c",grille[i]);
	}
}	

/* 
 * Display the table of size 20 given in parameter. 
 */
void afficheJoueur(char joueur[20]){
	int i;
	for (i = 0; i < 20; i++){
		printf("%c",joueur[i]);
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
*/
