#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include "utils/Sprite.h"
#include "utils/Sound.h"
#include "utils/GameConfig.h"

#include <time.h>

#ifndef __MYCLASS_MINE
#define __MYCLASS_MINE

#define nbCycle 200
#define nbFrameMine 10

enum mineTypeEnum {
	straight = 0, crooked = 1
};

class Mine {
	public:
		Mine(int index);
		~Mine();
		bool doSomething(SDL_Surface * surface);
		void drawHimself(SDL_Surface * surfaceToDraw, int offsetSpriteAnimation);
	private:
		int index;
		int type;
		int count;

		bool activate[nbPlayer];
		bool work;

		//animation
		int frameCounter;
		int offsetSprite;
		int nbFrameForAnimation;
};

#endif
