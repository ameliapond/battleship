/* player.h */

#include <string.h>

#define MAX_SHIP	2
#define DIM1		10
#define DIM2		10
#define SIZE_NAME	10

/* 
 * @Brief
 * @attr 	name 		Player's name.
 * @attr 	adress_ip	IP adress of the Player's client.
 * @attr	sailing_ship 	Player's ships that still sailling.
 * @attr	grille[][]	Player's map of the battlefield.
 */
typedef struct player{
	char *name;
	int adresse_ip;
	int sailing_ship;
	int grille[10][10];
} player;

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
void initPlayer(player *p, int adr_ip, char *nom)
{
	int i;
	p->name =  (char*)malloc(strlen(nom) * sizeof(char));
	p->adresse_ip = adr_ip;

	for( i = 0; i < strlen(nom); i++)
	{
		p->name[i] = nom[i];
	}
	p->sailing_ship = MAX_SHIP;
	initTable(p->grille);
}
