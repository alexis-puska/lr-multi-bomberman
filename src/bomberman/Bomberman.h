#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "../include/libretro_common.h"

#include <stdio.h>



#ifndef __MYCLASS_H
#define __MYCLASS_H
class Bomberman
{

public:
	Bomberman();
    ~Bomberman();
    SDL_Surface * getScreen();	
private:
	SDL_Surface *image;
	
};
#endif