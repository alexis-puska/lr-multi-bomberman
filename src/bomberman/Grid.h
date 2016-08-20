#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "../include/libretro_common.h"

#include <stdio.h>



#ifndef __MYCLASS_H
#define __MYCLASS_H
class Bomberman
{

public:
	Grid(int sizeX, int sizeY, int levelIndex);
    ~Grid();
    SDL_Surface * getGrid();
private:
	SDL_Surface *grid;
	
};
#endif