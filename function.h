#define _dim1	10
#define _dim2	10

/*
 * @Brief	Displays a two dimensions's integers table.
 */ 
void display_int_table(int tableau[_dim1][_dim2]){

	int i, j;
	
	for (i = 0; i < _dim1; i++)
	{
		for (j = 0; j < _dim2; j++)
		{
			printf("%d\n", tableau[i][j]);
		}
	}
}

/*
 * @Brief Set some value in the table *
 */
void setPlayerTable(int table[_dim1][_dim2]){
	
	int i , j ;

	for (i = 0; i < _dim1; i++)
	{
		for ( j = 0; j < _dim2; j++)
		{
			if ( i == j )
			{
				table[i][j] = 2;
			}
		}
	}
}
