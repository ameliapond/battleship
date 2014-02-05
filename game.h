/* game.h */

typedef struct game{
	char g[9];
	int player1;
	int player2;
	int grille1[10][10];
	int grille2[10][10];
} game;

void initGame(int p1, int p2, game GAME){
	GAME.player1 = p1;
	GAME.player2 = p2;
}

int play(game GAME){
	return 0;
}

int tirer(int x, int y, game GAME){
	if (!(x < 0 || x > 10 )){
		return 0;
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
