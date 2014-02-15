#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "function.h"

void main(int argc, char **argv){

	game *GAME = (game*)malloc(sizeof(game)) ;
	
	char p1[20] = "129.234.33.0";
	char p2[20] = "129.234.33.1";

	play(GAME, p1, p2);

/*	int match_table[10][10];

	initStringGrille(GAME->plateau);
	
	setPlayerTable(GAME->player1.grille);
	
	initMatchTable(match_table);
	
	matchGrids_int_to_string(GAME->plateau, GAME->player1.grille, match_table);
	
	display_char_table(GAME->plateau);

	setShip(1,7,GAME->player1.grille);
	
	matchGrids_int_to_string(GAME->plateau, GAME->player1.grille, match_table);

	display_char_table(GAME->plateau);
*/
	
}
