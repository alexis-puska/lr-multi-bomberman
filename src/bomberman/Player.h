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
#include "Bombe.h"
#include "Grid.h"

#define sizeX 35
#define sizeY 21





#ifndef __MYCLASS_PLAYER
#define __MYCLASS_PLAYER
class Player
{
public:
	Player(unsigned short * in_keystateLibretro, bool isACpuPlayer, int indexSprite, float startPositionX, float startPositionY, int playerNumberLibretro, int tab[sizeX * sizeY], int tableBonus[sizeX * sizeY], SDL_Surface ** bombeSpriteGame, Grid * gridParam, float * tabPlayerCoord, int nbPlayerConfigGame);
    ~Player();
    void doSomething(SDL_Surface * surfaceToDraw);

    /*
	* Game Function
	*/
	Bombe * addBombe();
	
	bool wantPutBombe();
	bool triggerPowerBombe();
	bool walkOnWall();
	bool isAlive();
	
	void winTheGame();
	void ABombeExplode();
	void releaseTrigger();
	void ABombeIsSet();
	void foundABonus(int bonusIndex);
	
	int getPlayerNumber();
	
    
private:
	/*
	* VAR
	*/
	
	//Object variable
	bool cpu;
	int playerNumber;
	int nbPlayerConfig;
	int characterSpriteIndex;
	
	//Game variable
	float posX;
	float posY;
	float playerSpeed;
	int playerState;
	int nbTickMalus;
	int playerMalus;
	float previousSpeedValue;
	int previousBombeNumber;
	
	bool ghostModePower;

	//bombe variable
	bool triggerBombe;
	bool putABombe;
	int flameStrengh;
	int bombeType;
	int NbBombeMax;
	int NBBombeRemaining;
	
	//louis bunring time
	int invincibleTime;
	
	//direction variable
	unsigned short * in_keystate;
	int previousDirection;
	
	//animation part
	int frameCounter;
	int offsetSprite;
	int nbFrameForAnimation;
	int louisType;
	
	//pointer to the grid element;
	int * tab;
	int * tabBonus;
	float * tabPlayerCoord;
	SDL_Surface ** bombeSprite;
	Grid * grid;
	
	//image of differente sprite of player
    SDL_Surface **playerSpriteWalk;
	SDL_Surface **playerSpriteWalkBomb;
	SDL_Surface **playerSpriteThrowBomb;
	SDL_Surface **playerSpriteOnLouis;
	SDL_Surface **playerSpriteVictory;
	SDL_Surface **playerSpriteAngry;
	SDL_Surface **playerSpriteBurn;
	SDL_Surface **louisSprite;
	SDL_Surface **louisSpriteBurn;
	SDL_Surface *louisMergebuffer;
    
    /*
	* FUNCTION For draw player
	*/
	void drawNormal(SDL_Surface * surfaceToDraw, bool animate);
	void drawOnLouis(SDL_Surface * surfaceToDraw, bool animate);	
	void drawWithBombe(SDL_Surface * surfaceToDraw, bool animate);
	void drawThrowBombe(SDL_Surface * surfaceToDraw, bool animate);
	void drawBurning(SDL_Surface * surfaceToDraw, bool animate);
	void drawLouisBurning(SDL_Surface * surfaceToDraw, bool animate);
	void drawVictory(SDL_Surface * surfaceToDraw, bool animate);
	void drawCrying(SDL_Surface * surfaceToDraw, bool animate);
	
	//malus function
	void getAMalusBonus();
	void releaseMalus();
	int findIndexPlayer();
};
#endif