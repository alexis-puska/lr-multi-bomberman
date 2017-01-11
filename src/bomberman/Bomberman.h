#include <SDL2/SDL.h>
#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif

#include <stdio.h>

#include "Game.h"
#include "Cursor.h"
#include "Sprite.h"
#include "Sound.h"


enum bombermanStepEnum {
	home = 0,					//spashscreen
	PlayerTypeMenu, 		//type of Player CPU / HUMAN / OFF
	PlayerSpriteMenu,
	gameOptionMenu,			//Sudden death / Bad Bomber / CPU Level
	levelSelectionMenu,		// Select the grid design
	gameStep
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
		//function to evoid key repetition
		void keyPressed();

		unsigned short in_keystate[16];
		SDL_Surface * vout_buf;

		//position index for the cursor
		int cursorPosition;
		//the bomb cursor
		Cursor cursor;

		//refreshbuffer, for menu, if need to be full redraw
		bool refreshBuffer;

		//buffer for draw
		SDL_Surface *screenBuffer;

		//currentStep index to know wich menu is display
		bombermanStepEnum currentStep;

		//boolean set to true when a keystate change
		bool keychange[16];
		bool anyPlayerkeychange;
		unsigned short previousPlayerKeystate[16];

		/*
		 *
		 * GAME OPTION
		 *
		 */

		//Game Grid
		Game *game;

		//table for memorise player type
		//index 0 : 0 Human, 1 CPU, 2 OFF
		//index 1 : sprite used for player
		int playerType[16][2];
		/*
		 * 0 : SuddenDeath		0 off, 1 on
		 * 1 : Bad Bomber		0 off, 1 on
		 * 2 : CPU Level			[0-3]
		 * 3 : Time of a party	[-1: infiny  3-6]
		 */
		int gameOption[4];

		int levelIndex;

};
#endif
