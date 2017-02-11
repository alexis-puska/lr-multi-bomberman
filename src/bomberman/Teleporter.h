#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include "utils/Sprite.h"
#include "utils/GameConfig.h"
#include "utils/Sound.h"

#ifndef __MYCLASS_TELEPORTER
#define __MYCLASS_TELEPORTER

#define nbFrameTeleporter		7
#define nbFrameTeleporterAnimation	5

class Teleporter {
	public:
		Teleporter(int index);
		~Teleporter();
		bool doSomething();
		bool isUsed();
		void teleporte();
		void drawHimself(SDL_Surface * surfaceToDraw);
	private:
		int index;

		int posAnimation;

		bool activate[nbPlayer];
};

#endif