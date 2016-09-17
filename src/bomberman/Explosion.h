#ifndef IS_OSX
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
    #include <SDL2/SDL_thread.h>
#else
    #include <SDL2_image/SDL_image.h>
    #include <SDL2_ttf/SDL_ttf.h>
    #include <SDL2/SDL_thread.h>
#endif

#include <stdio.h>

#ifndef __MYCLASS_EXPLOSION
#define __MYCLASS_EXPLOSION

#define sizeX 35
#define sizeY 21

class Explosion
{

public:
	Explosion(int posXX, int posYY, int index, SDL_Surface ** miscSheet, int table[sizeX * sizeY], int tableBonus[sizeX * sizeY]);
    ~Explosion();
    void tick(SDL_Surface * surfaceToDraw);
    bool canBeDelete();

private:
	SDL_Surface ** animation;
	int * tab;
	int * tabBonus;
	int posX;
	int posY; 
	int indexExplosion;
	bool deleteAnimation; 

	//for animation
	int frameCounter;
	int offsetSprite;
	int nbFrameForAnimation;
};
#endif