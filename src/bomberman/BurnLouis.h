#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include <stdio.h>

#include "Sprite.h"

#ifndef __MYCLASS_BURNLOUIS
#define __MYCLASS_BURNLOUIS

#define sizeX 35
#define sizeY 21

class BurnLouis {

	public:
		BurnLouis(int posX, int posY);
		~BurnLouis();
		void tick(SDL_Surface * surfaceToDraw);
		bool canBeDelete();

	private:
		int posX;
		int posY;
		bool deleteAnimation;

		//for animation
		int frameCounter;
		int offsetSprite;
		int nbFrameForAnimation;
};
#endif