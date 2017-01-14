#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include <stdio.h>

#include "../utils/Sprite.h"

#ifndef __MYCLASS_BURNLOUIS
#define __MYCLASS_BURNLOUIS

#define sizeX 35
#define sizeY 21

class BurnLouis {

	public:
		BurnLouis(float posX, float posY);
		~BurnLouis();
		void tick(SDL_Surface * surfaceToDraw);
		bool canBeDelete();

	private:
		float posX;
		float posY;
		bool deleteAnimation;

		//for animation
		int frameCounter;
		int offsetSprite;
		int nbFrameForAnimation;
};
#endif
