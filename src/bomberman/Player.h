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
//	bool dead;
//	bool burn;
//	bool onLouis;
	int playerNumber;
	int characterSpriteIndex;
	float posX;
	float posY;
	
	int frameCounter;
	int offsetSprite;
	
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
    
    
    unsigned short * in_keystate;
	int previousDirection;
    /*
	* FUNCTION
	*/

};
#endif