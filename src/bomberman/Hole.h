#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include "utils/Sprite.h"
#include "utils/Sound.h"
#include "utils/GameConfig.h"
#include "utils/network/BomberNetServer.h"

#ifndef __MYCLASS_HOLE
#define __MYCLASS_HOLE

#define nbWalkOn 3

class Hole {
	public:
		Hole(int index);
		~Hole();
		bool doSomething(SDL_Surface * surfaceToDraw);
		void drawHimself(SDL_Surface * surfaceToDraw);
		int getIndex();
	private:
		int index;
		int position;
		bool activate[nbPlayer];
};

#endif