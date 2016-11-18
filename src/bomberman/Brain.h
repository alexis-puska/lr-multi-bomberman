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
#include <time.h>
#include "AStar.h"
#include "BFS.h"

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

enum brainMove {
	none = -1, down = 0, up = 1, left = 2, right = 3
};

enum brain1step {
	lvl1CheckCanDropBomb = 0,	
	lvl1DropBomb = 1,
	lvl1GoSecure = 2,
	lvl1WaitBombeExplode = 3,
	lvl1FindNearWall = 4, 
	lvl1WalkToNearWall = 5
};

class Brain {

	public:
		Brain(unsigned short * keystate, int tab[sizeX * sizeY], float * tabCord, int nbPlayer, int playerNumber, int cpuLevel);
		~Brain();
		void think();
		
		
	private:
		//keystate of the player taht will be command by this brain
		unsigned short * keystate;
		//tab representing the map of the game
		int * tab;
		//tab contains the players coordinate
		float * tabCord;
		//nb player of the game (CPU or human)
		int nbPlayer;
		int playerNumber;
		AStar * astar;
		BFS * bfs;
		int targetPlayer;
		int currentIndex;
		int prevDir;
		int cpuLevel;
		int idxOwnBombe;

		int brainStep;

		int objectifIndex;
		
		
		int findNearPlayer();
		void level1();
		void level2();
		void level3();
		void level4();
		void level5();
		float calcDistance(int x1, int y1, int x2, int y2);
		int walkToObjectif(int objectif);

};
#endif
