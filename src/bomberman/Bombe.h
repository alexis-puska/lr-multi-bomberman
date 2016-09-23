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
#include "Grid.h"

#ifndef __MYCLASS_BOMBE
#define __MYCLASS_BOMBE

#define sizeX 35
#define sizeY 21

class Bombe
{

public:
	Bombe(int Strenght, int posXX, int posYY, int type, int numPlayerSetTheBombe, int nbTickBeforeExplostion, SDL_Surface ** miscSheet, int table[sizeX * sizeY]);
	~Bombe();
	bool isExplode();
	bool isPowerBombe();
	
	void explode();
	void explodeNow();
	void pushBomb(int direction);
	void tick(SDL_Surface * surfaceToDraw);

	int getStrenght();
	int getPlayer();
	int getPosX();
	int getPosY();
	int getCase();


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