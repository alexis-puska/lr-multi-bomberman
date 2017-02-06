#include <SDL2/SDL.h>
#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "utils/Sprite.h"
#include "utils/LevelService.h"
#include "utils/GameConfig.h"
#include "trolley/Rail.h"
#include "trolley/Button.h"

#ifndef __MYCLASS_GRID
#define __MYCLASS_GRID

#define sizeX 35
#define sizeY 21

enum gridElementEnum {
	emptyElement = 0, explosionElement = 1, brickElement = 2, bombeElement = 3, wallElement = 4, suddenDeathElement = 5,
};

enum bombeTypeEnum {
	normalBombeType = 0, radioBombeType = 1, powerBombeType = 2, bubbleBombeType = 3,
};

enum bonusEnum {
	noBonus = -1,
	deathBonus = 0,
	rollerBonus = 1,
	fireBonus = 2,
	fireMaxBonus = 3,
	bombeBonus = 4,
	radioBombeBonus = 5,
	kickBonus = 6,
	gloveBonus = 7,
	bubbleBonus = 8,
	powerBombeBonus = 9,
	getaBonus = 10,
	ghostBonus = 11,
	eggBonus = 12,
	shieldBonus = 13,
	lineOfBombeBonus = 14
};

enum malusEnum {
	noMalus = -1,
	diarheeMalus = 0,
	constipationMalus = 1,
	speedBombeMalus = 2,
	slowBombeMalus = 3,
	slowDownMalus = 4,
	speedUpMalus = 5,
	switchPlayerMalus = 6,
	miniBombeMalus = 7,
	invDirectionMalus = 8,
};

enum kickDirectionEnum {
	kickNone = -1, kickOnRight = 0, kickOnLeft = 1, kickOnUp = 2, kickOnDown = 3,
};

class Grid {

	public:
		Grid();
		Grid(int tab[sizeX * sizeY], int tabBonus[sizeX * sizeY]);
		~Grid();

		//configure the grid and build it
		void configure(int levelNumber);
		void generateGrid();
		void resetSurface();

		void burnABrick(int posX, int posY);
		void burnBonus(int posX, int posY);
		void placeNewDeathMalus();
		void placeSuddenDeathWall(int x, int y);
		int playerDeadNeedBonus(int bonusIndex);
		void buttonDoSomething();

		//get Image to render in retroarch
		SDL_Surface * getGroundLayer();
		SDL_Surface * getSkyLayer();
		SDL_Surface * getBricksLayer();
		SDL_Surface * getWallSprite();

	private:
		//Layers to draw the game
		SDL_Surface *ground;
		SDL_Surface *brickShadow;
		SDL_Surface *skyFixe;
		std::map<int, Rail *> rails;
		std::map<int, Button *> buttons;

		//table of free/occuped part of the grid
		int * tab;
		int * tabBonus;

		std::vector<int> emptyCase;
		std::vector<int> notEmptyCase;

		//level index to load the texture and level template
		int lvl;
		int var;

		void init();
		void initRails();
		void initButtons();
		void redrawRail(int index);
		void redrawButton(int index);


};
#endif
