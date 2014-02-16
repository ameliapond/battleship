/* player.h */

#include <string.h>

#define MAX_SHIP	2
#define DIM1		10
#define DIM2		10
#define SIZE_NAME	10

typedef struct player{
	char *name;
	int adresse_ip[20];
	int sailing_ship;
	int grille[10][10];
} player;

/*
 * @Brief Fill the cells of an integer table of 10 x 10 with 0.
 */
void initTable(int table[10][10]){
	
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
 */ 
void initPlayer(player *p, char adr_ip[20], char *nom){
	
	int i;

	p->name =  (char*)malloc(strlen(nom) * sizeof(char));

	for( i = 0; i < 20; i++)
	{
		p->adresse_ip[i] = adr_ip[i];
	}

	for( i = 0; i < strlen(nom); i++)
	{
		p->name[i] = nom[i];
	}

	p->sailing_ship = MAX_SHIP;

	initTable(p->grille);
}
