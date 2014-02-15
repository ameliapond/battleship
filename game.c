#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "function.h"

void main(int argc, char **argv){

	game *GAME = (game*)malloc(sizeof(game)) ;
	
	char p1[20] = "129.234.33.0";
	char p2[20] = "129.234.33.1";
	int match_table[10][10];

	initStringGrille(GAME->plateau);
	
	setPlayerTable(GAME->grille1);
	
	initMatchTable(match_table);
	
	matchGrids_int_to_string(GAME->plateau, GAME->grille1, match_table);
	
	afficheGrille(GAME->plateau);
}
