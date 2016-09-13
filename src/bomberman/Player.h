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

#define sizeX 35
#define sizeY 21


#ifndef __MYCLASS_PLAYER
#define __MYCLASS_PLAYER
class Player
{
public:
	Player(unsigned short * in_keystateLibretro, bool isACpuPlayer, int indexSprite, float startPositionX, float startPositionY, int playerNumberLibretro, int tab[sizeX * sizeY]);
    ~Player();
    void doSomething(SDL_Surface * surfaceToDraw);
private:
	/*
	* VAR
	*/
	bool cpu;
	int playerNumber;
	int characterSpriteIndex;
	float posX;
	float posY;
	int playerState;
	
	//animation part
	int frameCounter;
	int offsetSprite;
	int nbFrameForAnimation;

	int louisType;
	
	//pointer to the grid element;
	int * tab;
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
    
    unsigned short * in_keystate;
	int previousDirection;
    /*
	* FUNCTION
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