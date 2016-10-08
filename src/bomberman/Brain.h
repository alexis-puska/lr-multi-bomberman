#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_thread.h>
#else
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2/SDL_thread.h>
#endif

#include <stdio.h>

#ifndef __MYCLASS_BRAIN
#define __MYCLASS_BRAIN

#define sizeX 35
#define sizeY 21

enum brainKey {
	brainKeySelect = 1,
	brainKeyL3 = 2,
	brainKeyR3 = 4,
	brainKeyStart = 8,
	brainKeyUp = 16,
	brainKeyRight = 32,
	brainKeyDown = 64,
	brainKeyLeft = 128,
	brainKeyL2 = 256,
	brainKeyR2 = 512,
	brainKeyL1 = 1024,
	brainKeyR1 = 2048,
	brainKeyX = 4096,
	brainKeyA = 8192,
	brainKeyB = 16384,
	brainKeyY = 32768
};

class Brain {

	public:
		Brain(unsigned short * keystate, int tab[sizeX * sizeY], float * tabPlayerCoord, int nbPlayerConfig, int playerNumberControle, int posX, int posY);
		~Brain();
		void think();
	private:
		//keystate of the player taht will be command by this brain
		unsigned short * keystate;
		//tab representing the map of the game
		int * tab;
		//tab contains the players coordinate
		float * tabPlayerCoord;
		//nb player of the game (CPU or human)
		int nbPlayerConfig;
		int posX;
		int posY;
		int playerNumberControle;
		int nbTick;
};
#endif
