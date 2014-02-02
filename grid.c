#include "grid.h"
/*
 *@brief initilize a cell of the grid with a value.
 *@param Xcoord x coordinate of the grid
 *@param Ycoord y coordinate of the grid
 *@param grid the grid that contains the battlefield
 *@param value the value that will be affected to to a cell in the grid
 *@warning value can take only three value 
 *	0 no boat
 *	1 position already shooted
 *	2 there is  a boat here.
 */

int setCell(int Xcoord,int Ycoord,struct grid GRID, int value){
	GRID.battleField[Xcoord][Ycoord] = value; 
}


/*

+-------------------+
|o|_|_|_|_|_|_|_|_|_|
|o|_|_|_|_|_|_|_|_|_|
|o|_|_|_|_|_|_|_|_|_|
|o|_|_|_|_|_|_|_|_|_|
|o|_|_|_|_|_|_|_|_|_|
|o|_|_|_|_|_|_|o|_|_|
|o|_|_|o|o|_|o|_|_|_|
|o|_|_|_|_|_|_|_|_|_|
|o|_|_|_|_|_|_|_|_|_|
|o|_|_|_|_|_|_|_|_|_|
|o|_|_|_|_|_|_|_|_|_|
+-------------------+

*/
