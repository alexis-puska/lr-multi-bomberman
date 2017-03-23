#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include <stdio.h>

#include "../Grid.h"
#include "../utils/Sprite.h"
#include "../utils/network/BomberNetServer.h"

#ifndef __MYCLASS_BURNBonus
#define __MYCLASS_BURNBonus

#define sizeX 35
#define sizeY 21

class BurnBonus {

	public:
		BurnBonus(int posX, int posY);
		~BurnBonus();
		void tick(SDL_Surface * surfaceToDraw);
		bool canBeDelete();

	private:
		int posX;
		int posY;
		int index;
		bool deleteAnimation;

		//for animation
		int frameCounter;
		int offsetSprite;
		int nbFrameForAnimation;
};
#endif
