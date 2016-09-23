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
#include "Bombe.h"
#include "Grid.h"

#define sizeX 35
#define sizeY 21





#ifndef __MYCLASS_PLAYER
#define __MYCLASS_PLAYER
class Player
{
public:
	Player(unsigned short * in_keystateLibretro, bool isACpuPlayer, int indexSprite, float startPositionX, float startPositionY, int playerNumberLibretro, int tab[sizeX * sizeY], int tableBonus[sizeX * sizeY], SDL_Surface ** bombeSpriteGame, Grid * gridParam);
    ~Player();
    void doSomething(SDL_Surface * surfaceToDraw);

    /*
	* Game Function
	*/
	Bombe * addBombe();
	bool wantPutBombe();
	//check for the end of game
	bool isAlive();
	void winTheGame();
	
	void ABombeExplode();
	bool triggerPowerBombe();
	void releaseTrigger();
	void ABombeIsSet();
	void foundABonus(int bonusIndex);
	int getPlayerNumber();
	bool walkOnWall();
    
private:
	/*
	* VAR
	*/
	
	//Game variable
	float posX;
	float posY;
	int playerState;
	int NbBombeMax;
	int NBBombeRemaining;
	float playerSpeed;
	bool haveGlovePower;
	bool ghostModePower;

	
	bool triggerBombe;
	bool putABombe;
	int flameStrengh;
	int bombeType;	
	int invincibleTime;
	
	//Object variable
	bool cpu;
	int playerNumber;
	int characterSpriteIndex;
	

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
	
	void getAMalusBonus();
};
#endif