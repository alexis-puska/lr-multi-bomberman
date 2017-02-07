#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include "utils/Sprite.h"
#include "utils/GameConfig.h"

#ifndef __MYCLASS_HOLE
#define __MYCLASS_HOLE

#define nb	2

class Hole {
	public:
		Hole(int index);
		~Hole();
		bool doSomething();
		void drawHimself(SDL_Surface * surfaceToDraw);
	private:
		int index;
		int position;
		bool activate[nbPlayer];
};

#endif