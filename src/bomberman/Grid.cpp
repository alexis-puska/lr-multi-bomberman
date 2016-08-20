#include "Grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Grid::init(){
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    ground = IMG_Load( world1_ground );
    wall = IMG_Load( world1_wall );
    brick = IMG_Load( world1_brick );
    theGrid = SDL_CreateRGBSurface(0, 1890, 1008, 32, rmask, gmask, bmask, amask);
    SDL_FillRect(theGrid, NULL, SDL_MapRGB(theGrid->format, 0, 204, 255));
}


Grid::Grid(){
    init();
}

Grid::Grid(int x, int y, int levelIndex)
{
    init();
    srand (time(NULL));
    sizeX = x;
    sizeY = y;
    lvl = levelIndex;
    tab = new int[sizeX * sizeY];
    generateGrid();
}

Grid::~Grid()
{
	free(theGrid);
}

SDL_Surface * Grid::getGrid(){
    generateGrid();
	return theGrid;
}


void Grid::setSize(int x, int y){
    sizeX = x;
    sizeY = y;
    tab = new int[sizeX * sizeY];
    generateGrid();
}


void Grid::generateGrid(){
    
    for(int i=0;i<sizeX;i++){
        for(int j=0;j<sizeY;j++){
            if( j == 0 || j == (sizeY-1) || i == 0 || i == (sizeX-1)|| (j%2 == 0 && i%2 == 0)){
                tab[i+(j*sizeX)] = 1;
            } else {
                /* generate secret number between 1 and 3: */
                if((rand() % 3 + 1)>=2){
                    tab[i+(j*sizeX)] = 2;
                } else {
                    tab[i+(j*sizeX)] = 0;
                }
            }
        }
    }

    
    SDL_Rect dstrect;
    SDL_Rect srcrect;
    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = 54;
    srcrect.h = 48;
    for(int i = 0; i < sizeX; i++){
        for(int j = 0; j < sizeY ; j++){
            dstrect.x = i * 54;
            dstrect.y = j * 48;
            dstrect.w = 54;
            dstrect.h = 48;
            SDL_BlitSurface(ground, &srcrect, theGrid, &dstrect);
            switch(tab[i+(j*sizeX)]){
                case 0:
                    SDL_BlitSurface(ground, &srcrect, theGrid, &dstrect);
                    break;
                case 1:
                    SDL_BlitSurface(wall, &srcrect, theGrid, &dstrect);
                    break;
                case 2:
                    SDL_BlitSurface(brick, &srcrect, theGrid, &dstrect);
                    break;
                default:
                    
                    break;
            }
   
        }
    }
}
