#ifndef IS_OSX
#include <SDL2/SDL_image.h>

#else
#include <SDL2_image/SDL_image.h>
#endif

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <vector>
#include <algorithm>

#include "Player.h"
#include "ia/Brain.h"
#include "utils/Sound.h"
#include "utils/LevelService.h"
#include "animation/BurnWall.h"
#include "animation/BurnLouis.h"
#include "animation/SuddenDeathAnimation.h"
#include "animation/PopBonus.h"
#include "animation/BurnBonus.h"
#include "utils/Sprite.h"
#include "utils/GameConfig.h"

#ifndef __MYCLASS_GAME
#define __MYCLASS_GAME

enum keyPad {
	keyPadSelect = 1,
	keyPadL3 = 2,
	keyPadR3 = 4,
	keyPadStart = 8,
	keyPadUp = 16,
	keyPadRight = 32,
	keyPadDown = 64,
	keyPadLeft = 128,
	keyPadL2 = 256,
	keyPadR2 = 512,
	keyPadL1 = 1024,
	keyPadR1 = 2048,
	keyPadX = 4096,
	keyPadA = 8192,
	keyPadB = 16384,
	keyPadY = 32768
};

enum gameStateEnum {
	gameStart = 0, gamePause = 1, gameWait = 2, gameEnd = 3, generateResult = 4,
};

enum suddenDeathDirection {
	suddenDeathRight = 0, suddenDeathDown = 1, suddenDeathLeft = 2, suddenDeathUp = 3,
};

class Game {

	public:
		Game();
		Game(SDL_Surface * vout_buf, unsigned short * in_keystate);
		~Game();
		bool isAlive();
		bool isConfigured();
		bool isRequestStopGame();
		void tick();
		void startGame();
		void stopGame();
		void exitGame();
	private:
		//variable
		SDL_Thread * mainThread;
		SDL_Surface * screenBuffer;
		SDL_Surface * playerBombeExplode;
		SDL_Surface * overlay;
		SDL_Surface * overlayResult;

		//pointer in the libretro buffer
		SDL_Surface * vout_buf;

		//Game option variable
		int nbTickForGame;
		int nbTickForGameParam;
		bool badBomber;

		//functional variable
		int gameState;
		int nbPlayerAlive;
		int nbPlayerInGame;
		bool isThreadAlive;
		bool requestStopGame;
		bool configured;
		bool gameInitElement;

		//SUDDENT DEATH
		bool suddenDeath;
		int levelIndex;
		int variante;
		bool suddenDeathCase;
		int suddenDeathMinX;
		int suddenDeathMaxX;
		int suddenDeathMinY;
		int suddenDeathMaxY;
		int suddenDeathX;
		int suddenDeathY;
		int suddenDeathDirection;

		//keystate of player
		unsigned short * in_keystate;
		unsigned short * in_keystate_cpu;

		//containers of object that can be put in the grid
		std::vector<Player *> players;
		std::vector<Bombe *> bombes;
		std::vector<Explosion *> explosions;
		std::vector<BurnWall *> burnWalls;
		std::vector<SuddenDeathAnimation *> suddenDeathAnimations;
		std::vector<Brain *> brains;
		std::vector<BurnLouis *> louisBurns;
		std::vector<PopBonus *> PopBonusList;
		std::vector<BurnBonus *> BurnBonusList;

		/*
		 *	GRID PART
		 */
		//the grid generator
		Grid *grid;
		//table of free/occuped part of the grid
		int * tab;
		int * tabBonus;

		//function
		void copySurfaceToBackRenderer(SDL_Surface * src, SDL_Surface * dest, int x, int y);
		void mergeScreen(bool mergeResult);
		void generateHeader();
		void updateTimeDisplay();
		void drawResultOfGame();
		void updateHeaderPlayer(int i, int playerNumber);
		void InitElementOfGame();
		void redrawElement(int x, int y);
		void GenerateMineExplosion(int index, int direction);




		std::map<int, Rail *> rails;
		std::map<int, Button *> buttons;
		std::map<int, Trolley *> trolleys;
		std::map<int, Mine *> mines;
		std::map<int, Hole *> holes;
		std::map<int, Teleporter *> teleporters;
		void buttonDoSomething();
		void mineDoSomething();
		void holeDoSomething();
		void teleporterDoSomething();
		void initRails();
		void initButtons();
		void initHole();
		void initMine();
		void initTeleporter();
		void initTrolley();
		void redrawRail(int index);
		void redrawButton(int index);
		void redrawHole(int index);
		void redrawMine(int index);
		void redrawTeleporter(int index);
		void redrawTrolley(int index);

};
#endif
