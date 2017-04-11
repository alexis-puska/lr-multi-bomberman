#include <SDL2/SDL.h>
#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include <stdio.h>

#include "Game.h"
#include "animation/Cursor.h"
#include "utils/Sprite.h"
#include "utils/Sound.h"
#include "utils/LevelService.h"
#include "utils/GameConfig.h"
#include "utils/network/BomberNetServer.h"
#include "utils/network/ipify.h"

#define MAX_ADDRESSES 	10

enum bombermanStepEnum {
	home = 0,			//spashscreen
	gameMode,
	serverNumberOfClient,
	serverWaitForClient,
	clientNumberPlayerName,
	clientIpPort,
	PlayerTypeMenu, 		//type of Player CPU / HUMAN / OFF
	PlayerSpriteMenu,
	gameOptionMenu,		//Sudden death / Bad Bomber / CPU Level
	levelSelectionMenu,	// Select the grid design
	gameStep,
	clientViewStep
};

enum gameOptionEnum {
	suddenDeathOption = 0, badBomberOption = 1, cpuLevelOption = 2, timeLevelOption = 3
};

#ifndef __MYCLASS_BOMBERMAN
#define __MYCLASS_BOMBERMAN
class Bomberman {

	public:
		Bomberman(SDL_Surface * vout_bufLibretro);
		~Bomberman();
		//function call for every frame
		void tick(unsigned short in_keystateLibretro[16]);
		//function to copy some surface into an other
		void copySurfaceToBackRenderer(SDL_Surface * src, SDL_Surface * dest, int x, int y);

	private:
		void drawPlayerTypeMenu();
		void drawPlayerSpriteMenu();
		void drawGameOptionMenu();
		void drawLevelSelectionMenu();


		void drawSelectGameModeMenu();
		void drawServerConfigurationMenu();
		void drawServerWaitForClient();
		void drawClientConfigurationMenu();
		void drawClientIpMenu();

		//function to evoid key repetition
		void keyPressed();

		unsigned short in_keystate[16];
		unsigned short in_keystate_over_net[16];
		SDL_Surface * vout_buf;

		//position index for the cursor
		int cursorPosition;
		//the bomb cursor
		Cursor cursor;


		bool error;
		char errorString[255];


		//refreshbuffer, for menu, if need to be full redraw
		bool refreshBuffer;
		bool refreshWanIp;
		char addr[256];
		int nbLocalAddr;
		char localAddr[MAX_ADDRESSES][16];
		IPaddress addresses[MAX_ADDRESSES];

		//buffer for draw
		SDL_Surface *screenBuffer;

		//currentStep index to know wich menu is display
		bombermanStepEnum currentStep;

		//boolean set to true when a keystate change
		bool keychange[16];
		bool anyPlayerkeychange;
		unsigned short previousPlayerKeystate[16];

		/******************
		 * GAME OPTION
		 ******************/
		Game *game;
};
#endif
