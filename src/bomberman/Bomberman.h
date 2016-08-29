#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include <stdio.h>
#include "Grid.h"



#ifndef __MYCLASS_H
#define __MYCLASS_H
class Bomberman
{

public:
	Bomberman();
    ~Bomberman();
    SDL_Surface * getScreen();	
    void tick(unsigned short *in_keystate, SDL_Surface * vout_buf);
    void swapBuffer();
	void copySurfaceToBackRenderer(SDL_Surface * toCopy, int x, int y);
    void copySurfaceToBackRenderer(SDL_Surface * toCopy);
private:
	SDL_Surface *image0;
	SDL_Surface *image1;
    int currentImage;
    Grid grid;
};
#endif