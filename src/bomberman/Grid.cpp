#include "Grid.h"

Grid::Grid(int sizeX, int sizeY, int levelIndex)
{
    sizeX = sizeX;
    sizeY = sizeY;
    levelIndex = levelIndex;
    int tab[sizeX][sizeY];
}

Grid::~Grid()
{
	free(image);
}

SDL_Surface * Grid::getScreen(){
	//SDL_Surface * img = IMG_Load( background );
	return image;
}





void Grid::generateGrid();
