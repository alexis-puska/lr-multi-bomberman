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
#include "Explosion.h"

#ifndef __MYCLASS_BOMBE
#define __MYCLASS_BOMBE

enum bombeTypeEnum{
	normalBombeType	= 0,
	radioBombeType	= 1,
	powerBombeType	= 2,
	bubbleBombeType	= 3,
};

#define sizeX 35
#define sizeY 21





class Bombe
{

public:
	Bombe(int Strenght, int posXX, int posYY, int type, int numPlayerSetTheBombe, int nbTickBeforeExplostion, SDL_Surface ** miscSheet, int table[sizeX * sizeY]);
    ~Bombe();
    bool isExplode();
    void explode();
    int getStrenght();
    int getPlayer();
    int getPosX();
    int getPosY();
    int getCase();
    bool isPowerBombe();
    void pushBomb(int direction);
	void tick(SDL_Surface * surfaceToDraw);

private:
	SDL_Surface ** animation;
	int * tab;

	int nbTickExplode;
	int posX;
	int posY;
	int strenght;
	int player;
	int bombeType;

	//for animation	
	int frameCounter;
	int offsetSprite;
	int nbFrameForAnimation;
	
};
#endif