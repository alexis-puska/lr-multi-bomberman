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
#include "Explosion.h"
#include "Grid.h"

#ifndef __MYCLASS_SUDDEN_DEATH_ANIMATION
#define __MYCLASS_SUDDEN_DEATH_ANIMATION

#define sizeX 35
#define sizeY 21

class SuddenDeathAnimation {

	public:
		SuddenDeathAnimation(int posXX, int posYY, SDL_Surface * wallSprite, int table[sizeX * sizeY], Grid * gridGame);
		~SuddenDeathAnimation();
		void tick(SDL_Surface * surfaceToDraw);
		bool canBeDeleted();
	private:
		SDL_Surface * sprite;
		int * tab;
		Grid * grid;
		int posX;
		int posY;
		int offset;
		bool canDelete;

		//for animation	
		int frameCounter;
};
#endif
