#include <SDL2/SDL.h>
#ifndef IS_OSX
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_ttf.h>
#else
	#include <SDL2_image/SDL_image.h>
    #include <SDL2_ttf/SDL_ttf.h>
#endif

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <vector>
#include <algorithm>

#include "Grid.h"
#include "Player.h"
#include "BurnWall.h"

#ifndef __MYCLASS_GAME
#define __MYCLASS_GAME

enum keyPad{
	keyPadSelect	= 1,
	keyPadL3		= 2,
	keyPadR3		= 4,
	keyPadStart		= 8,
	keyPadUp		= 16,
	keyPadRight		= 32,
	keyPadDown		= 64,
	keyPadLeft		= 128,
	keyPadL2		= 256,
	keyPadR2		= 512,
	keyPadL1		= 1024,
	keyPadR1		= 2048,
	keyPadX			= 4096,
	keyPadA			= 8192,
	keyPadB			= 16384,
	keyPadY			= 32768	
	};

class Game
{

public:
	Game();
	Game(int levelIndexInformation, int playerInformation[16][2], int gameOption[4], SDL_Surface * vout_bufLibretro, unsigned short * in_keystateLibretro);
    ~Game();
	bool isAlive();
	bool isConfigured();
	bool isRequestStopGame();
	void tick();
	void startGame();
	void stopGame();
	void exitGame();
private:
	//Font for Header and time remaining and pause message
    TTF_Font* fragileBombersFont;
    
	//variable
	SDL_Thread * mainThread;
	SDL_Surface * screenBuffer;
	SDL_Surface * playerBombeExplode;
	SDL_Surface * overlay;
	
	//spite for animation
	SDL_Surface ** explosionSprite;
	SDL_Surface ** bombeSprite;
	SDL_Surface ** bonusSprite;
	SDL_Surface ** eggsSprite;
	SDL_Surface ** burnWallSprite;
	//sprite for header
	SDL_Surface ** headerPlayerSprite;
	
	
	
	//pointer in the libretro buffer
	SDL_Surface * vout_buf;
	
	//Game option variable
	int levelIndex;
	int cpuLevel;
	int nbTickForGame;
	bool suddenDeath;
	bool badBomber;
	//functional variable
	bool gamePause;
	bool gameStart;
	bool isThreadAlive;
	bool requestStopGame;
	bool configured;
	

	//keystate of player
	unsigned short * in_keystate;
	
	//containers of object that can be put in the grid
	std::vector<Player *>players;
	std::vector<Bombe *>bombes;
	std::vector<Explosion *> explosions;
	std::vector<BurnWall *> burnWalls;
	
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
	void mergeScreen();
	void generateHeader();
	void updateTimeDisplay();
	
};
#endif