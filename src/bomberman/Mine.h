#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include <math.h>
#include "utils/Sprite.h"
#include "utils/Sound.h"
#include "utils/GameConfig.h"
#include "utils/network/BomberNetServer.h"

#include <time.h>

#ifndef __MYCLASS_MINE
#define __MYCLASS_MINE

#define nbCycle 			200
#define nbCycleEnd		215
#define nbFrameMine 		10
#define straightNbFrame		4
#define crookedNbframe 		8

enum mineTypeEnum {
	straight = 0, crooked = 1
};

enum mineExplosionDirectionEnum {
	mineUp = 0,
	mineRight = 1,
	mineDown = 2,
	mineLeft = 3
};

class Mine {
	public:
		Mine(int index);
		~Mine();
		bool doSomething(SDL_Surface * surface);
		void drawHimself(SDL_Surface * surfaceToDraw, int offsetSpriteAnimation);
		int getFirstDirection();
		int getSecondDirection();
		int getIndex();
	private:
		int index;
		int type;
		int count;

		int first;
		int second;

		int channelSound;

		bool activate[nbPlayer];
		bool work;

		//animation
		int frameCounter;
		int offsetSprite;
		int nbFrameForAnimation;
};

#endif
