#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif
#include <map>
#include <stdio.h>
#include "../utils/Sprite.h"
#include "../utils/GameConfig.h"
#include "Trolley.h"
#include "../utils/network/BomberNetServer.h"

#ifndef __MYCLASS_BUTTON
#define __MYCLASS_BUTTON

#define sizeX 	35
#define sizeY 	21
#define nbPlayer	16

class Button {
	public:
		Button(int index, std::map<int, Trolley *> * trolleys);
		~Button();
		bool doSomething(SDL_Surface * surfaceToDraw);
		void drawHimself(SDL_Surface * surfaceToDraw);
		void switching(SDL_Surface * surfaceToDraw);
	private:
		int index;
		bool pressed;
		bool activate[nbPlayer];
		std::map<int, Trolley *> * trolleys;
};

#endif