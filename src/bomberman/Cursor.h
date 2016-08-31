#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>

const static char *cursorSprite = "./resources/sprite/other/Cursor.png";

#ifndef __MYCLASS_J
#define __MYCLASS_J
class Cursor
{

public:
	Cursor();
    ~Cursor();
    SDL_Surface * getCurrentFrame();
    
private:
	int position;
	int frame;
	SDL_Surface **cursor;
};
#endif