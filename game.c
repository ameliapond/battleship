#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void main(int argc, char **argv){

	game *GAME = (game*)malloc(sizeof(game)) ;
	
	char p1[20] = "129.234.33.0";
	char p2[20] = "129.234.33.1";
	
	initStringGrille(GAME->plateau);
	afficheGrille(GAME->plateau);
	initGame(p1,p2,GAME);
	afficheJoueur(GAME->player1);
}
