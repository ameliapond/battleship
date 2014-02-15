#include <string.h>

#define _DIM1		10
#define _DIM2		10

/*
 * @Brief	Displays a two dimensions's integers table.
 */ 
void display_int_int_table(int tableau[_DIM1][_DIM2]){

	int i, j;
	
	for (i = 0; i < _DIM1; i++)
	{
		for (j = 0; j < _DIM2; j++)
		{
			printf("%d\n", tableau[i][j]);
		}
	}
}

/* 
 * @Brief	Display the table of charater given in parameter. 
 */
void display_char_table(char *table){

	int i;
	size_t size = strlen(table);

	for (i = 0; i < size; i++)
	{
		printf("%c",table[i]);
	}
}

/*
 * @Brief Set some value in the table *
 */
void setPlayerTable(int table[_DIM1][_DIM2]){
	
	int i , j ;

	for (i = 0; i < _DIM1; i++)
	{
		for ( j = 0; j < _DIM2; j++)
		{
			if ( i == j )
			{
				table[i][j] = 2;
			}
		}
	}
}
