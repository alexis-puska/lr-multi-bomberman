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
#include "bombe.h"

#define sizeX 35
#define sizeY 21


#ifndef __MYCLASS_PLAYER
#define __MYCLASS_PLAYER
class Player
{
public:
	Player(unsigned short * in_keystateLibretro, bool isACpuPlayer, int indexSprite, float startPositionX, float startPositionY, int playerNumberLibretro, int tab[sizeX * sizeY], SDL_Surface ** bombeSpriteGame);
    ~Player();
    void doSomething(SDL_Surface * surfaceToDraw);

    /*
	* Game Function
	*/
	Bombe * addBombe();
	bool wantPutBombe();
	void ABombeExplode();
	void ABombeIsSet();
	void takeAnEgg();
	void takeGlove();
	void ghostMode();
	void flameUp();
	void speedUp();
	void speedDown();
	void powerBombe();
	void bubleBombe();
	void radioBombe();
	void bombeNbUp();
    
    
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
	bool bubbleBombePower;
	bool haveGlovePower;
	bool ghostModePower;
	bool powerBombePower;
	bool radioBombePower;
	bool putABombe;
	int flameStrengh;
	
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
	SDL_Surface ** bombeSprite;
	
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
	void drawNormal(SDL_Surface * surfaceToDraw);
	void drawOnLouis(SDL_Surface * surfaceToDraw);	
	void drawWithBombe(SDL_Surface * surfaceToDraw);
	void drawThrowBombe(SDL_Surface * surfaceToDraw);
	void drawBurning(SDL_Surface * surfaceToDraw);
	void drawLouisBurning(SDL_Surface * surfaceToDraw);
	void drawVictory(SDL_Surface * surfaceToDraw);
	void drawCrying(SDL_Surface * surfaceToDraw);
	
	
	
};
#endif