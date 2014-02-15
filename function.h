/* function.h */

/* 
 * @Brief Contains some usefull table handling functions. 
 */

#define _DIM1	10
#define _DIM2	10

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
