#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include <stdio.h>

#include "../Grid.h"
#include "../utils/Sprite.h"
#include "../utils/network/BomberNetServer.h"

#ifndef __MYCLASS_PopBonus
#define __MYCLASS_PopBonus

#define sizeX 35
#define sizeY 21

class PopBonus {

	public:
		PopBonus(int posX, int posY);
		PopBonus(int index);
		~PopBonus();
		void tick(SDL_Surface * surfaceToDraw);
		bool canBeDelete();

	private:
		int posX;
		int posY;
		int idx;
		bool deleteAnimation;

		//for animation
		int frameCounter;
		int offsetSprite;
		int nbFrameForAnimation;
};
#endif
