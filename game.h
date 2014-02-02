/* game.h */

typedef struct game{
	int player1,
	    player2;
	    struct grid grid1;
	    struct grid grid2;
} game;

void initGame(int p1, int p2, struct grid *GRID1, struct grid *GRID1, struct game *GAME);
