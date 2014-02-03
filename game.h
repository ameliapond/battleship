/* game.h */

typedef struct game{
	int player1;
	int player2;
	int grille1[10][10];
	int grille2[10][10];
}

void initGame(int p1, int p2, game GAME){
	GAME.player1 = p1;
	GAME.player2 = p2;
}	    
