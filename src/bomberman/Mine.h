#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include "utils/Sprite.h"
#include "utils/Sound.h"
#include "utils/GameConfig.h"

#ifndef __MYCLASS_MINE
#define __MYCLASS_MINE

#define nb	2

class Mine {
	public:
		Mine(int index);
		~Mine();
		bool doSomething();
		void drawHimself(SDL_Surface * surfaceToDraw);
	private:
		int index;
		int type;
		int countdown;

		bool activate[nbPlayer];
};

#endif