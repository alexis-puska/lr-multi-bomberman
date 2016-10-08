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
#include "Grid.h"

#ifndef __MYCLASS_BURNWALL
#define __MYCLASS_BURNWALL

#define sizeX 35
#define sizeY 21

class BurnWall {

	public:
		BurnWall(int posX, int posY, int index, SDL_Surface ** animation, int tab[sizeX * sizeY], int tabBonus[sizeX * sizeY]);
		~BurnWall();
		void tick(SDL_Surface * surfaceToDraw);
		bool canBeDelete();

	private:
		SDL_Surface ** animation;
		int * tab;
		int * tabBonus;
		int posX;
		int posY;
		int indexBurnWall;
		bool deleteAnimation;

		//for animation
		int frameCounter;
		int offsetSprite;
		int nbFrameForAnimation;
};
#endif
