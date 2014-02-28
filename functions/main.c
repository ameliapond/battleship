#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#define SIZE	2

int isFilled(int table[SIZE])
{
	int i = 0;
	int filled = 1;
	for (i=0; i < SIZE; i++)
	{
		if ( table[i] == 0)
		{
			filled = 0;
		}
	}
	return filled;			 
}

void display_pointer_int_table(int *tab, int size_tab)
{
	int i;
	for (i = 0; i < size_tab; i++)
	{
		printf("tab[%d]:%d\n",i,tab[i]);
	}
}

int main(int argc, char** argv)
{
	char str[] ="0stateconnexion";
  	char * pch;
  	printf ("Splitting string \"%s\" into tokens:\n",str);
  	pch = strtok (str,"þ");
  	while (pch != NULL)
 	{
    		printf ("%s\n",pch);
    		pch = strtok (NULL, " ,.-");
  	}
  	return 0;
}
