#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "../include/libretro_common.h"



#ifndef __MYCLASS_H
#define __MYCLASS_H
class Bomberman
{

public:
	Bomberman();
    ~Bomberman();
    void getScreen();	
private:
	//SDL_Surface *image;
	
};
#endif