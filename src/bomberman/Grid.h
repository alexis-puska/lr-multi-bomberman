#include <SDL2/SDL.h>
#ifndef IS_OSX
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif
#include <vector>


#ifndef __MYCLASS_GRID
#define __MYCLASS_GRID

#define sizeX 35
#define sizeY 21

#include "../include/level.h"

enum gridElementEnum{
	emptyElement		= 0,
	explosionElement	= 1,
	brickElement		= 2,
	bombeElement		= 3,
	wallElement			= 4,
};

enum bombeTypeEnum{
	normalBombeType	= 0,
	radioBombeType	= 1,
	powerBombeType	= 2,
	bubbleBombeType	= 3,
};

enum bonusEnum{
	noBonus			=-1,
	deathBonus		= 0,
	rollerBonus		= 1,
	fireBonus		= 2,
	fireMaxBonus	= 3,
	bombeBonus		= 4,
	radioBombeBonus	= 5,
	kickBonus		= 6,
	gloveBonus		= 7,
	bubbleBonus		= 8,
	powerBombeBonus	= 9,
	getaBonus		= 10,
	ghostBonus		= 11,
	eggBonus		= 12,
};

enum malusEnum{
	noMalus				= -1,
	diarheeMalus		= 0,
	constipationMalus	= 1,
	speedBombeMalus		= 2,
	slowBombeMalus		= 3,
	slowDownMalus		= 4,
	speedUpMalus		= 5,
	switchPlayerMalus	= 6,
	miniBombeMalus		= 7,
	invDirectionMalus	= 8,
};


class Grid
{

	public:
	Grid();
	Grid(int levelIndex, int table[sizeX * sizeY], int tableBonus[sizeX * sizeY], SDL_Surface ** bonusSpriteGame);
	~Grid();
	
	//configure the grid and build it
	void configure(int levelNumber);
	void generateGrid();
	void resetSurface();
	
	void burnABrick(int posX, int posY);
	void burnBonus(int posX, int posY);
	void placeNewDeathMalus();
	
	//get Image to render in retroarch
	SDL_Surface * getGroundLayer();
	SDL_Surface * getSkyLayer();
	SDL_Surface * getBricksLayer();
	
	private:
	
	/*Texture for the level
	Load from AllLevel.png file
	each level is a block of 432 x 384 px
	Texture is on the left in 270 x 384 px block
	Sky texture is on the right block size 162 * 288 px
	Each texture size is 54 x 48 px
	*/
	SDL_Surface **textures;
	SDL_Surface **skys;
	SDL_Surface **bonusSprite;
	
	//Layers to draw the game
	SDL_Surface *ground;
	SDL_Surface *brickShadow;
	SDL_Surface *skyFixe;
	
	
	//table of free/occuped part of the grid
	int * tab;
	int * tabBonus;
	
	std::vector<int> emptyCase;
	std::vector<int> notEmptyCase;
	
	//level index to load the texture and level template
	int lvl;
	
	void init();

};
#endif