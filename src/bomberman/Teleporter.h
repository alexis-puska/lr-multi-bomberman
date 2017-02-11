#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include "utils/Sprite.h"
#include "utils/GameConfig.h"
#include "utils/Sound.h"
#include <time.h>

#ifndef __MYCLASS_TELEPORTER
#define __MYCLASS_TELEPORTER

#define nbFrameTeleporter 3

class Teleporter {
	public:
		Teleporter(int index);
		~Teleporter();
		bool doSomething(SDL_Surface * surface);
		void drawHimself(SDL_Surface * surfaceToDraw, int frame);
		void addDestination(Teleporter * teleporter);
		void notifyArrival(int playerNumber);
		int getIndex();
	private:
		bool animate;
		int index;
		int playerToTeleporte;
		int indexDestination;
		int frameCounter;
		int offsetSprite;
		int nbFrameForAnimation;
		std::vector<Teleporter *> destination;
		bool activate[nbPlayer];
		void teleporte();
		void keepPlayer();
};

#endif