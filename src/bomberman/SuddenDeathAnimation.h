#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include <stdio.h>
#include "Grid.h"
#include "Sprite.h"

#ifndef __MYCLASS_SUDDEN_DEATH_ANIMATION
#define __MYCLASS_SUDDEN_DEATH_ANIMATION

#define sizeX 35
#define sizeY 21

class SuddenDeathAnimation {

	public:
		SuddenDeathAnimation(int posX, int posY, int levelIndex, int tab[sizeX * sizeY], Grid * grid);
		~SuddenDeathAnimation();
		void tick(SDL_Surface * surfaceToDraw);
		bool canBeDeleted();
	private:
		int levelIndex;
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
