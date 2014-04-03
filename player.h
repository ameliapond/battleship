/* player.h */

#include <string.h>

#define MAX_SHIP	2
#define DIM1		10
#define DIM2		10
#define SIZE_NAME	10

#define SAILING_SHIP	2
#define CELL_SHOTED	1
#define OCEAN		0
#define WAITING_START   4
#define SETTING_SHIP	3

/* 
 * @Brief
 * @attr 	name 		Player's name.
 * @attr 	adress_ip	Actualy contain the socket descriptor linked with the ip address.
 * @attr	sailing_ship 	Player's ships that still sailling.
 * @attr	grille[][]	Player's map of the battlefield.
 * @attr	p_state		Player's game state. Say if the player is playing
 * 				or waiting for a party. 0 = waiting, 1 = playing.
 * @attr	tmp[2]		Contains typed coordinates before validation.
 * @attr	gamePosition[2]	Contains the position of the player in the table 
 *				of games and inside the game play (i.e player 1 or 2).
 *				First dimenstion equals to the position in the table of games.
 *				Second dimension equals to the position inside a game (player 1 or 2).
 */
typedef struct player{
	char name[SIZE_NAME];
	int adresse_ip;
	int sailing_ship;
	int grille[10][10];
	int p_state;
	int tmp[2]; 
	int gamePosition[2];
} player;

void display_player(player *p)
{
	printf("name %s\nadresse_ip: %d\nsailing_ship: %d\np_state: %d\ngameposition[0]: %d\ngameposition[0]: %d\n",
	p->name, p->adresse_ip, p->sailing_ship, p->p_state, p->gamePosition[0], p->gamePosition[1]);
}
/*
 * @Brief Fill the cells of an integer table of 10 x 10 with 0.
 */
void initTable(int table[10][10])
{
	int i , j ;

	for (i = 0; i < DIM1; i++)
	{
		for ( j = 0; j < DIM2; j++)
		{
			table[i][j] = 0;
		}
	}
}

/* 
 * @Brief Set the player's attributes to start a new game.
 * @Param
 * @Param
 * @Param
 */ 
void initPlayer(player *p, int adr_ip, char nom[SIZE_NAME], int initNumber)
{
	int i;
	p->adresse_ip = adr_ip;

	for( i = 0; i < SIZE_NAME; i++)
	{
		p->name[i] = nom[i];
	}
	p->sailing_ship = 0;
	initTable(p->grille);
	p->p_state = -1;
	p->tmp[0] = -1;
	p->tmp[1] = -1;
}
