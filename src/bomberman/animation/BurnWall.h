#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include <stdio.h>

#include "../Grid.h"
#include "../utils/Sprite.h"
#include "../utils/network/BomberNetServer.h"

#ifndef __MYCLASS_BURNWALL
#define __MYCLASS_BURNWALL

#define sizeX 35
#define sizeY 21

class BurnWall {

	public:
		BurnWall(int posX, int posY, int index, int tab[sizeX * sizeY]);
		BurnWall(int posX, int posY, int index);
		~BurnWall();
		void tick(SDL_Surface * surfaceToDraw);
		bool canBeDelete();

	private:
		int * tab;
		int levelIndex;
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
