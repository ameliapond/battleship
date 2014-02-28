#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "function.h"

void main(int argc, char **argv)
{
	game *GAME = (game*)malloc(sizeof(game));
	
	GAME->player1 = (player*)malloc(sizeof(player));
	GAME->player2 = (player*)malloc(sizeof(player));

	int p1 = 129234330;
	int p2 = 129234331;
	
	play(GAME, p1, p2);
}
